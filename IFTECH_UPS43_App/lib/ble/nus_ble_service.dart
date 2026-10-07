import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

import 'ble_constants.dart';
import 'screen_capture.dart';

/// Parsed UPS nominal rating from `rating` CLI.
class UpsRatingConfig {
  const UpsRatingConfig({
    required this.kva,
    required this.batV,
    required this.inV,
    required this.outV,
  });

  final double kva;
  final int batV;
  final int inV;
  final int outV;
}

/// BLE notify 를 `***` 에코가 올 때까지 모은다. base64 는 로그에 넣지 않는다.
class _RxWait {
  _RxWait({this.onProgress});

  final void Function(ScreenCaptureProgress progress)? onProgress;
  final StringBuffer _buf = StringBuffer();
  final Completer<String> done = Completer<String>();
  String _carry = '';
  int _screCount = 0;
  int? _bandsTotal;
  bool _finished = false;
  String? _lastLabel;

  String get text => _buf.toString();
  bool get finished => _finished;

  void add(String chunk) {
    _buf.write(chunk);
    final window = _carry + chunk;
    var start = 0;
    while (true) {
      final i = window.indexOf('SCRE', start);
      if (i < 0) break;
      if (i + 4 > _carry.length) _screCount++;
      start = i + 4;
    }
    if (_bandsTotal == null && window.contains('BANDS=')) {
      final m = RegExp(r'BANDS=(\d+)').firstMatch(text);
      final n = int.tryParse(m?.group(1) ?? '');
      if (n != null && n > 0) _bandsTotal = n;
    }
    // `***` 는 base64에 없다. 결과 마커 뒤의 에코에서만 끝낸다.
    if (!_finished && window.contains('***')) {
      _finished = _resultEchoReached();
    }
    _carry = window.length <= 3 ? window : window.substring(window.length - 3);

    final label = _bandsTotal == null
        ? '화면을 그리는 중…'
        : '받는 중 $_screCount / $_bandsTotal';
    if (_lastLabel != label) {
      _lastLabel = label;
      onProgress?.call(
        ScreenCaptureProgress(bandsDone: _screCount, bandsTotal: _bandsTotal ?? 0),
      );
    }
  }

  bool _resultEchoReached() {
    final s = text;
    final doneAt = s.lastIndexOf('SCR DONE');
    final errAt = s.lastIndexOf('SCR ERR');
    final errorAt = s.lastIndexOf('ERROR:');
    var mark = doneAt;
    if (errAt > mark) mark = errAt;
    if (errorAt > mark) mark = errorAt;
    if (mark < 0) return false;
    return s.indexOf('***', mark) >= 0;
  }
}

enum BleConnectionState {
  disconnected,
  scanning,
  connecting,
  connected,
  error,
}

/// BLE Nordic UART Service client for IFTECH UPS ESP32.
class NusBleService extends ChangeNotifier {
  BluetoothDevice? _device;
  BluetoothCharacteristic? _rx;
  BluetoothCharacteristic? _tx;
  StreamSubscription<List<int>>? _txSub;
  StreamSubscription<BluetoothConnectionState>? _connSub;
  StreamSubscription<List<ScanResult>>? _scanSub;

  BleConnectionState _state = BleConnectionState.disconnected;
  String? _error;
  final List<ScanResult> _scanResults = [];
  final StringBuffer _log = StringBuffer();
  final List<String> _logLines = [];
  String? _deviceSsid;
  String? _devicePass;
  UpsRatingConfig? _deviceRating;
  int _wifiConfigVersion = 0;
  int _ratingConfigVersion = 0;
  String? _deviceFwVersion;
  String? _fwUpdateBase;
  String? _fwMeta;
  String? _fwJsonUrl;
  String? _serverLatest;
  bool _updateAvailable = false;
  String? _fwCheckError;
  Future<void> _cliChain = Future<void>.value();
  _RxWait? _rxWait;

  BleConnectionState get state => _state;
  String? get error => _error;
  List<ScanResult> get scanResults => List.unmodifiable(_scanResults);
  List<String> get logLines => List.unmodifiable(_logLines);
  String get logText => _log.toString();
  BluetoothDevice? get device => _device;
  bool get isConnected => _state == BleConnectionState.connected;

  /// Last SSID reported by device (`ssid` / `pass` CLI reply).
  String? get deviceSsid => _deviceSsid;

  /// Last password reported by device. Empty string means open AP.
  String? get devicePass => _devicePass;

  /// Bumps when device SSID/PASS lines are parsed (for UI autofill).
  int get wifiConfigVersion => _wifiConfigVersion;

  /// Last UPS rating from `rating` CLI.
  UpsRatingConfig? get deviceRating => _deviceRating;

  /// Bumps when rating lines are parsed.
  int get ratingConfigVersion => _ratingConfigVersion;

  String? get deviceFwVersion => _deviceFwVersion;
  String? get fwUpdateBase => _fwUpdateBase;
  String? get fwMeta => _fwMeta;
  String? get fwJsonUrl => _fwJsonUrl;
  String? get serverLatest => _serverLatest;
  bool get updateAvailable => _updateAvailable;
  bool get fwIsLatest {
    if (_updateAvailable) return false;
    if (_deviceFwVersion == null || _serverLatest == null) return false;
    return !_isNewerVersion(_deviceFwVersion, _serverLatest) &&
        !_isNewerVersion(_serverLatest, _deviceFwVersion);
  }
  String? get fwCheckError => _fwCheckError;

  Future<bool> ensureAdapterOn() async {
    if (await FlutterBluePlus.isSupported == false) {
      _setError('이 기기는 BLE를 지원하지 않습니다.');
      return false;
    }
    final adapterState = await FlutterBluePlus.adapterState.first;
    if (adapterState != BluetoothAdapterState.on) {
      try {
        await FlutterBluePlus.turnOn();
      } catch (_) {
        _setError('블루투스를 켜 주세요.');
        return false;
      }
    }
    return true;
  }

  Future<void> startScan({Duration timeout = const Duration(seconds: 8)}) async {
    if (!await ensureAdapterOn()) return;

    final keepConnected = _state == BleConnectionState.connected;
    await stopScan();
    _scanResults.clear();
    _state = BleConnectionState.scanning;
    _error = null;
    notifyListeners();

    try {
      await FlutterBluePlus.startScan(
        timeout: timeout,
        androidUsesFineLocation: true,
        androidLegacy: true,
      );

      _scanSub = FlutterBluePlus.scanResults.listen((results) {
        final filtered = results.where((r) {
          final name = r.device.platformName;
          return name.startsWith(BleConstants.deviceNamePrefix);
        }).toList();

        // Also keep unnamed devices that advertise NUS service
        for (final r in results) {
          final hasNus = r.advertisementData.serviceUuids.any(
            (u) => u.str.toUpperCase() == BleConstants.serviceUuid.toUpperCase(),
          );
          if (hasNus && !filtered.any((f) => f.device.remoteId == r.device.remoteId)) {
            filtered.add(r);
          }
        }

        _scanResults
          ..clear()
          ..addAll(filtered);
        // Prefer strongest RSSI first
        _scanResults.sort((a, b) => b.rssi.compareTo(a.rssi));
        notifyListeners();
      });

      await FlutterBluePlus.isScanning.where((v) => v == false).first;
    } catch (e) {
      _setError('스캔 실패: $e');
    } finally {
      if (_state == BleConnectionState.scanning) {
        _state = keepConnected
            ? BleConnectionState.connected
            : BleConnectionState.disconnected;
        notifyListeners();
      }
    }
  }

  Future<void> stopScan() async {
    await _scanSub?.cancel();
    _scanSub = null;
    try {
      await FlutterBluePlus.stopScan();
    } catch (_) {}
  }

  Future<void> connect(BluetoothDevice device) async {
    await stopScan();
    await Future<void>.delayed(const Duration(milliseconds: 1200));
    final sameDevice = _device?.remoteId == device.remoteId &&
        _state == BleConnectionState.connected;
    if (sameDevice) {
      notifyListeners();
      return;
    }
    await disconnect(notify: false);

    _device = device;
    _state = BleConnectionState.connecting;
    _error = null;
    _appendLog('연결 중: ${_displayName(device)}');
    notifyListeners();

    try {
      _connSub = device.connectionState.listen((s) {
        if (s == BluetoothConnectionState.disconnected &&
            _state == BleConnectionState.connected) {
          _appendLog('연결이 끊겼습니다.');
          _clearChars();
          _state = BleConnectionState.disconnected;
          notifyListeners();
        }
      });

      try {
        await device.clearGattCache();
      } catch (_) {}

      // Android FBP 기본 mtu=512는 ESP32 연결 직후 끊기는 경우가 있다.
      // 타임아웃 직후 재시도는 폰 GATT를 더 꼬이게 하므로 한 번만 시도한다.
      await device.connect(
        timeout: const Duration(seconds: 20),
        autoConnect: false,
        mtu: null,
      );
      try {
        await device.requestMtu(517);
      } catch (e) {
        _appendLog('MTU 요청 생략: $e');
      }
      await Future<void>.delayed(const Duration(milliseconds: 400));

      final services = await device.discoverServices();
      BluetoothService? nus;
      for (final s in services) {
        if (s.uuid.str.toUpperCase() == BleConstants.serviceUuid.toUpperCase()) {
          nus = s;
          break;
        }
      }
      if (nus == null) {
        throw Exception('Nordic UART 서비스를 찾을 수 없습니다.');
      }

      for (final c in nus.characteristics) {
        final id = c.uuid.str.toUpperCase();
        if (id == BleConstants.rxUuid.toUpperCase()) {
          _rx = c;
        } else if (id == BleConstants.txUuid.toUpperCase()) {
          _tx = c;
        }
      }

      if (_rx == null || _tx == null) {
        throw Exception('RX/TX 캐릭터리스틱을 찾을 수 없습니다.');
      }

      await _tx!.setNotifyValue(true);
      _txSub = _tx!.onValueReceived.listen((bytes) {
        if (bytes.isEmpty) return;
        final text = utf8.decode(bytes, allowMalformed: true);
        final wait = _rxWait;
        if (wait != null) {
          wait.add(text);
          if (wait.finished && !wait.done.isCompleted) {
            _rxWait = null;
            wait.done.complete(wait.text);
          }
          return;
        }
        _appendLog(text, fromDevice: true);
      });

      _state = BleConnectionState.connected;
      _appendLog('연결됨. 명령을 전송할 수 있습니다.');
      notifyListeners();
      Future.delayed(const Duration(milliseconds: 250), () async {
        await fetchStoredWifi();
        await fetchFwInfo();
      });
    } catch (e) {
      _appendLog('연결 실패: $e');
      await disconnect(notify: false);
      _setError('연결 실패: $e\n폰 블루투스를 껐다 켜 보세요.');
    }
  }

  Future<void> disconnect({bool notify = true}) async {
    _failRxWait(StateError('연결이 끊겼습니다.'));
    await _txSub?.cancel();
    _txSub = null;
    await _connSub?.cancel();
    _connSub = null;
    _clearChars();
    try {
      await _device?.disconnect();
    } catch (_) {}
    _device = null;
    _deviceSsid = null;
    _devicePass = null;
    _deviceRating = null;
    _deviceFwVersion = null;
    _fwUpdateBase = null;
    _fwMeta = null;
    _fwJsonUrl = null;
    _serverLatest = null;
    _updateAvailable = false;
    _fwCheckError = null;
    _state = BleConnectionState.disconnected;
    if (notify) {
      _appendLog('연결 해제');
      notifyListeners();
    }
  }

  Future<T> _cli<T>(Future<T> Function() job) {
    final next = _cliChain.then((_) => job());
    _cliChain = next.then((_) {}).catchError((Object _) {});
    return next;
  }

  void _failRxWait(Object error) {
    final wait = _rxWait;
    _rxWait = null;
    if (wait != null && !wait.done.isCompleted) {
      wait.done.completeError(error);
    }
  }

  Future<void> _writeRaw(String cmd) async {
    final bytes = utf8.encode(cmd);
    const chunk = 160;
    for (var i = 0; i < bytes.length; i += chunk) {
      final end = (i + chunk < bytes.length) ? i + chunk : bytes.length;
      await _rx!.write(bytes.sublist(i, end), withoutResponse: false);
    }
  }

  /// `***` 에코까지 장치 응답을 모은다. 캡처 본문은 로그에 남기지 않는다.
  Future<String> _transact(
    String command, {
    Duration timeout = const Duration(seconds: 8),
    void Function(ScreenCaptureProgress progress)? onProgress,
  }) async {
    if (!isConnected || _rx == null) {
      throw ScreenCaptureException('장치가 연결되지 않았습니다.');
    }
    final wait = _RxWait(onProgress: onProgress);
    _rxWait = wait;
    try {
      await _writeRaw('$command\r\n');
      _appendLog('> $command');
      return await wait.done.future.timeout(timeout);
    } finally {
      if (identical(_rxWait, wait)) _rxWait = null;
    }
  }

  /// 현재 LVGL 화면을 PNG로 받는다. 펌웨어 명령은 `scr`.
  Future<ScreenCaptureResult> captureScreen({
    void Function(String status)? onProgress,
  }) {
    return _cli(() => _captureNow(onProgress: onProgress));
  }

  Future<ScreenCaptureResult> _captureNow({
    void Function(String status)? onProgress,
  }) async {
    try {
      onProgress?.call('화면을 그리는 중…');
      final text = await _transact(
        'scr',
        timeout: const Duration(seconds: 180),
        onProgress: (progress) => onProgress?.call(progress.label),
      );
      final shot = await decodeScreenCapture(text);
      _appendLog('화면 캡처 ${shot.width}x${shot.height}');
      return shot;
    } catch (e) {
      _appendLog('화면 캡처 실패: $e');
      rethrow;
    }
  }

  /// Send a CLI command. Appends CR+LF if missing (ESP32 expects \\r or \\n).
  /// By default clears the log console so only this command's traffic is shown.
  Future<void> sendCommand(String command, {bool clearFirst = true}) {
    return _cli(() => _sendCommandNow(command, clearFirst: clearFirst));
  }

  Future<void> _sendCommandNow(String command, {bool clearFirst = true}) async {
    if (!isConnected || _rx == null) {
      _setError('장치가 연결되지 않았습니다.');
      return;
    }
    var cmd = command.trim();
    if (cmd.isEmpty) return;
    if (!cmd.endsWith('\r') && !cmd.endsWith('\n')) {
      cmd = '$cmd\r\n';
    }

    if (clearFirst) {
      _log.clear();
      _logLines.clear();
    }

    try {
      final bytes = utf8.encode(cmd);
      // Chunk for BLE ATT payload (leave room under MTU)
      const chunk = 160;
      for (var i = 0; i < bytes.length; i += chunk) {
        final end = (i + chunk < bytes.length) ? i + chunk : bytes.length;
        await _rx!.write(bytes.sublist(i, end), withoutResponse: false);
      }
      _appendLog('> ${command.trim()}');
    } catch (e) {
      _appendLog('전송 실패: $e');
      _setError('전송 실패: $e');
    }
  }

  /// Query device UPS nominal rating (`rating` with no argument).
  Future<void> fetchStoredRating() async {
    if (!isConnected) return;
    await sendCommand('rating');
  }

  Future<void> setRating({
    required double kva,
    required int batV,
    required int inV,
    required int outV,
  }) async {
    final kvaText = kva == kva.roundToDouble()
        ? '${kva.toInt()}'
        : kva.toStringAsFixed(1);
    await sendCommand('rating $kvaText $batV $inV $outV', clearFirst: true);
    await Future.delayed(const Duration(milliseconds: 400));
    await sendCommand('rating', clearFirst: false);
  }

  Future<void> setWifi({required String ssid, required String password}) async {
    // 한 번의 사용자 동작이므로 로그만 한 번 비우고 ssid/pass를 이어서 표시
    await sendCommand('ssid $ssid', clearFirst: true);
    await Future.delayed(const Duration(milliseconds: 300));
    if (password.trim().isEmpty) {
      await sendCommand('pass none', clearFirst: false);
    } else {
      await sendCommand('pass $password', clearFirst: false);
    }
  }

  /// Query device for currently stored SSID/PASS (`ssid` with no argument).
  Future<void> fetchStoredWifi() async {
    if (!isConnected) return;
    await sendCommand('ssid');
  }

  Future<void> fetchFwInfo() async {
    if (!isConnected) return;
    try {
      await sendCommand('version', clearFirst: false);
      await Future<void>.delayed(const Duration(milliseconds: 400));
      await sendCommand('fw', clearFirst: false);
      await Future<void>.delayed(const Duration(milliseconds: 800));
      _scanLogForFwInfo();
      _applyFwUrlFallback();
      await refreshServerFirmware();
    } catch (e) {
      _fwCheckError = '확인 실패: $e';
      _updateAvailable = false;
      _appendLog('펌웨어 확인 실패: $e');
    }
  }

  void _scanLogForFwInfo() {
    for (final line in _logLines) {
      _parseFwInfoLine(line);
    }
  }

  void _applyFwUrlFallback() {
    if (_fwJsonUrl != null && _fwJsonUrl!.isNotEmpty) return;
    var base = (_fwUpdateBase ?? BleConstants.fwUpdateBase).trim();
    while (base.endsWith('/')) {
      base = base.substring(0, base.length - 1);
    }
    final meta = (_fwMeta != null && _fwMeta!.isNotEmpty)
        ? _fwMeta!
        : BleConstants.fwUpdateMeta;
    _fwUpdateBase = base;
    _fwMeta = meta;
    _fwJsonUrl = '$base/$meta';
  }

  Future<void> refreshServerFirmware() async {
    final url = _fwJsonUrl;
    if (url == null || url.isEmpty) {
      _fwCheckError = '업데이트 경로 없음';
      notifyListeners();
      return;
    }
    try {
      final uri = Uri.parse(url);
      final client = HttpClient();
      client.connectionTimeout = const Duration(seconds: 8);
      try {
        final req = await client.getUrl(uri);
        final res = await req.close().timeout(const Duration(seconds: 8));
        if (res.statusCode != 200) {
          _fwCheckError = '서버 HTTP ${res.statusCode}';
          _updateAvailable = false;
          notifyListeners();
          return;
        }
        var body = await utf8.decodeStream(res);
        if (body.startsWith('\uFEFF')) {
          body = body.substring(1);
        }
        final decoded = jsonDecode(body);
        if (decoded is! Map) {
          _fwCheckError = 'JSON 형식 오류';
          notifyListeners();
          return;
        }
        _serverLatest = decoded['latest']?.toString();
        _updateAvailable = _isNewerVersion(_deviceFwVersion, _serverLatest);
        _fwCheckError = null;
        _appendLog(
          _updateAvailable
              ? '서버 ${_serverLatest} > 장비 ${_deviceFwVersion} (업데이트 있음)'
              : (fwIsLatest
                  ? '서버 ${_serverLatest} = 장비 ${_deviceFwVersion} (최신 버전)'
                  : '서버 ${_serverLatest ?? '-'} / 장비 ${_deviceFwVersion ?? '-'} (비교 불가)'),
        );
      } finally {
        client.close(force: true);
      }
    } catch (e) {
      _fwCheckError = '서버 확인 실패: $e';
      _updateAvailable = false;
      _appendLog('펌웨어 서버 확인 실패 ($_fwJsonUrl): $e');
      notifyListeners();
    }
  }

  bool _isNewerVersion(String? current, String? server) {
    List<int>? parse(String? v) {
      if (v == null) return null;
      final m = RegExp(r'(\d+)\.(\d+)\.(\d+)').firstMatch(v.trim());
      if (m == null) return null;
      return [
        int.parse(m.group(1)!),
        int.parse(m.group(2)!),
        int.parse(m.group(3)!),
      ];
    }

    final c = parse(current);
    final s = parse(server);
    if (c == null || s == null) return false;
    for (var i = 0; i < 3; i++) {
      if (s[i] > c[i]) return true;
      if (s[i] < c[i]) return false;
    }
    return false;
  }

  void clearLog() {
    _log.clear();
    _logLines.clear();
    notifyListeners();
  }

  void _clearChars() {
    _rx = null;
    _tx = null;
  }

  void _setError(String message) {
    _error = message;
    _state = BleConnectionState.error;
    notifyListeners();
  }

  void _appendLog(String text, {bool fromDevice = false}) {
    final cleaned = text.replaceAll('\r\n', '\n').replaceAll('\r', '\n');
    var wifiUpdated = false;
    var ratingUpdated = false;
    for (final line in cleaned.split('\n')) {
      if (line.isEmpty && fromDevice) continue;
      _logLines.add(line);
      _log.writeln(line);
      if (fromDevice && _parseWifiConfigLine(line)) {
        wifiUpdated = true;
      }
      if (fromDevice && _parseFwInfoLine(line)) {
        wifiUpdated = true;
      }
      if (fromDevice && _parseRatingConfigLine(line)) {
        ratingUpdated = true;
      }
    }
    // Cap log size
    while (_logLines.length > 500) {
      _logLines.removeAt(0);
    }
    if (wifiUpdated) {
      _wifiConfigVersion++;
    }
    if (ratingUpdated) {
      _ratingConfigVersion++;
    }
    notifyListeners();
  }

  /// Parse `SSID : xxx` / `PASS : yyy` / `PASS : (open)` from ESP32 CLI.
  bool _parseWifiConfigLine(String line) {
    final trimmed = line.trim();
    final ssidMatch = RegExp(r'^SSID\s*:\s*(.*)$', caseSensitive: false)
        .firstMatch(trimmed);
    if (ssidMatch != null) {
      _deviceSsid = ssidMatch.group(1)?.trim() ?? '';
      return true;
    }
    final passMatch = RegExp(r'^PASS\s*:\s*(.*)$', caseSensitive: false)
        .firstMatch(trimmed);
    if (passMatch != null) {
      final raw = passMatch.group(1)?.trim() ?? '';
      if (raw == '(open)' || raw.toLowerCase() == 'open') {
        _devicePass = '';
      } else {
        _devicePass = raw;
      }
      return true;
    }
    return false;
  }

  bool _parseFwInfoLine(String line) {
    final trimmed = line.trim();
    var hit = false;
    final ver = RegExp(r'VERSION\s*:\s*(\d+\.\d+\.\d+)', caseSensitive: false)
        .firstMatch(trimmed);
    if (ver != null) {
      _deviceFwVersion = ver.group(1);
      hit = true;
    }
    final upd = RegExp(r'UPDATE\s*:\s*(\S+)', caseSensitive: false)
        .firstMatch(trimmed);
    if (upd != null) {
      _fwUpdateBase = upd.group(1)?.trim();
      hit = true;
    }
    final meta = RegExp(r'META\s*:\s*(\S+)', caseSensitive: false)
        .firstMatch(trimmed);
    if (meta != null) {
      _fwMeta = meta.group(1)?.trim();
      hit = true;
    }
    final json = RegExp(r'FWJSON\s*:\s*(\S+)', caseSensitive: false)
        .firstMatch(trimmed);
    if (json != null) {
      _fwJsonUrl = json.group(1)?.trim();
      hit = true;
    }
    return hit;
  }

  /// Parse `KVA : 10.0 kVA`, `BAT : 192 V`, `IN  : 220 V`, `OUT : 220 V`.
  bool _parseRatingConfigLine(String line) {
    final trimmed = line.trim();
    final kvaMatch =
        RegExp(r'^KVA\s*:\s*([0-9]+(?:\.[0-9]+)?)', caseSensitive: false)
            .firstMatch(trimmed);
    if (kvaMatch != null) {
      final kva = double.tryParse(kvaMatch.group(1) ?? '');
      if (kva != null) {
        _deviceRating = UpsRatingConfig(
          kva: kva,
          batV: _deviceRating?.batV ?? 192,
          inV: _deviceRating?.inV ?? 220,
          outV: _deviceRating?.outV ?? 220,
        );
        return true;
      }
    }
    final batMatch =
        RegExp(r'^BAT\s*:\s*(\d+)', caseSensitive: false).firstMatch(trimmed);
    if (batMatch != null) {
      final v = int.tryParse(batMatch.group(1) ?? '');
      if (v != null) {
        final prev = _deviceRating;
        _deviceRating = UpsRatingConfig(
          kva: prev?.kva ?? 10,
          batV: v,
          inV: prev?.inV ?? 220,
          outV: prev?.outV ?? 220,
        );
        return true;
      }
    }
    final inMatch =
        RegExp(r'^IN\s*:\s*(\d+)', caseSensitive: false).firstMatch(trimmed);
    if (inMatch != null) {
      final v = int.tryParse(inMatch.group(1) ?? '');
      if (v != null) {
        final prev = _deviceRating;
        _deviceRating = UpsRatingConfig(
          kva: prev?.kva ?? 10,
          batV: prev?.batV ?? 192,
          inV: v,
          outV: prev?.outV ?? 220,
        );
        return true;
      }
    }
    final outMatch =
        RegExp(r'^OUT\s*:\s*(\d+)', caseSensitive: false).firstMatch(trimmed);
    if (outMatch != null) {
      final v = int.tryParse(outMatch.group(1) ?? '');
      if (v != null) {
        final prev = _deviceRating;
        _deviceRating = UpsRatingConfig(
          kva: prev?.kva ?? 10,
          batV: prev?.batV ?? 192,
          inV: prev?.inV ?? 220,
          outV: v,
        );
        return true;
      }
    }
    return false;
  }

  String _displayName(BluetoothDevice d) {
    final n = d.platformName;
    return n.isNotEmpty ? n : d.remoteId.str;
  }

  @override
  void dispose() {
    stopScan();
    disconnect(notify: false);
    super.dispose();
  }
}
