import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../ble/nus_ble_service.dart';
import '../ble/wifi_presets.dart';
import '../widgets/ota_upgrade_flow.dart';
import '../widgets/wifi_scan_sheet.dart';
import 'log_screen.dart';
import 'scan_screen.dart';

class ControlScreen extends StatefulWidget {
  const ControlScreen({super.key});

  @override
  State<ControlScreen> createState() => _ControlScreenState();
}

class _ControlScreenState extends State<ControlScreen> {
  final _ssidCtrl = TextEditingController();
  final _passCtrl = TextEditingController();
  final _cmdCtrl = TextEditingController();
  bool _obscurePass = true;
  int _lastWifiVersion = -1;
  bool _wifiFieldsTouched = false;
  bool _wifiExpanded = true;
  bool _quickCmdExpanded = false;
  String _wifiPresetId = kWifiPresets.first.id;
  bool _applyingWifiFields = false;
  bool _logOpening = false;
  bool _connectOpening = false;
  bool _fwChecking = false;

  @override
  void initState() {
    super.initState();
    _applyPreset(kWifiPresets.first, markTouched: false);
    _ssidCtrl.addListener(_onWifiFieldEdited);
    _passCtrl.addListener(_onWifiFieldEdited);
    WidgetsBinding.instance.addPostFrameCallback((_) {
      final ble = context.read<NusBleService>();
      _syncWifiFields(ble, force: true);
      if (ble.isConnected) ble.fetchStoredWifi();
    });
  }

  @override
  void dispose() {
    _ssidCtrl.removeListener(_onWifiFieldEdited);
    _passCtrl.removeListener(_onWifiFieldEdited);
    _ssidCtrl.dispose();
    _passCtrl.dispose();
    _cmdCtrl.dispose();
    super.dispose();
  }

  void _onWifiFieldEdited() {
    if (_applyingWifiFields) return;
    _wifiFieldsTouched = true;
    final matched = matchWifiPreset(
      _ssidCtrl.text.trim(),
      passOpen: _passCtrl.text.trim().isEmpty,
      pass: _passCtrl.text,
    );
    final nextId = matched?.id ?? kWifiPresetCustomId;
    if (nextId != _wifiPresetId && mounted) {
      setState(() => _wifiPresetId = nextId);
    }
  }

  void _applyPreset(WifiPreset preset, {bool markTouched = true}) {
    _applyingWifiFields = true;
    _ssidCtrl.text = preset.ssid;
    _passCtrl.text = preset.isOpen ? '' : preset.password;
    _applyingWifiFields = false;
    _wifiPresetId = preset.id;
    _wifiFieldsTouched = markTouched;
  }

  void _selectWifiPreset(String id) {
    setState(() {
      if (id == kWifiPresetCustomId) {
        _wifiPresetId = kWifiPresetCustomId;
        _wifiFieldsTouched = true;
        return;
      }
      final preset = kWifiPresets.firstWhere((p) => p.id == id);
      _applyPreset(preset);
    });
  }

  void _syncWifiFields(NusBleService ble, {bool force = false}) {
    if (!force && _wifiFieldsTouched) return;
    if (ble.wifiConfigVersion == _lastWifiVersion && !force) return;
    _lastWifiVersion = ble.wifiConfigVersion;

    final ssid = ble.deviceSsid;
    final pass = ble.devicePass;
    if (ssid == null && pass == null) return;

    _applyingWifiFields = true;
    if (ssid != null && ssid.isNotEmpty) {
      _ssidCtrl.text = ssid;
    }
    if (pass != null) {
      _passCtrl.text = pass;
    }
    _applyingWifiFields = false;

    final matched = matchWifiPreset(
      _ssidCtrl.text.trim(),
      passOpen: (_passCtrl.text.trim().isEmpty),
      pass: _passCtrl.text,
    );
    setState(() {
      _wifiPresetId = matched?.id ?? kWifiPresetCustomId;
      _wifiFieldsTouched = false;
    });
  }

  Future<void> _pickWifiSsid() async {
    final ssid = await WifiScanSheet.show(context);
    if (ssid == null || !mounted) return;
    setState(() {
      _applyingWifiFields = true;
      _ssidCtrl.text = ssid;
      _applyingWifiFields = false;
      _wifiFieldsTouched = true;
      final matched = matchWifiPreset(
        ssid,
        passOpen: _passCtrl.text.trim().isEmpty,
        pass: _passCtrl.text,
      );
      _wifiPresetId = matched?.id ?? kWifiPresetCustomId;
    });
  }

  Future<void> _reloadFromDevice() async {
    _wifiFieldsTouched = false;
    await context.read<NusBleService>().fetchStoredWifi();
    if (mounted) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('장비에서 Wi-Fi 설정을 읽는 중…'),
          duration: Duration(seconds: 1),
        ),
      );
    }
  }

  Future<void> _sendWifi() async {
    final ssid = _ssidCtrl.text.trim();
    if (ssid.isEmpty) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('SSID를 입력하세요')),
      );
      return;
    }
    await context.read<NusBleService>().setWifi(
          ssid: ssid,
          password: _passCtrl.text,
        );
  }

  Future<void> _confirmAndSend(String label, String command) async {
    final ok = await showDialog<bool>(
      context: context,
      builder: (ctx) => AlertDialog(
        title: Text(label),
        content: Text('"$command" 명령을 보낼까요?'),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(ctx, false),
            child: const Text('취소'),
          ),
          FilledButton(
            onPressed: () => Navigator.pop(ctx, true),
            child: const Text('전송'),
          ),
        ],
      ),
    );
    if (ok == true && mounted) {
      await context.read<NusBleService>().sendCommand(command);
      if (mounted) await _openLogPage();
    }
  }

  String _fmtDateTime(DateTime dt) {
    String two(int n) => n.toString().padLeft(2, '0');
    return '${dt.year}-${two(dt.month)}-${two(dt.day)} '
        '${two(dt.hour)}:${two(dt.minute)}:${two(dt.second)}';
  }

  String? _parseDeviceTime(List<String> lines) {
    final re = RegExp(
      r'TIME\s*:\s*(\d{4}-\d{2}-\d{2}\s+\d{2}:\d{2}:\d{2})',
      caseSensitive: false,
    );
    for (final line in lines.reversed) {
      final m = re.firstMatch(line);
      if (m != null) return m.group(1);
    }
    return null;
  }

  Future<void> _openTimeSetDialog() async {
    final ble = context.read<NusBleService>();
    if (!ble.isConnected) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('먼저 장비를 연결하세요')),
      );
      return;
    }
    await ble.sendCommand('time');
    await Future<void>.delayed(const Duration(milliseconds: 400));
    if (!mounted) return;

    final deviceTime = _parseDeviceTime(ble.logLines);
    final ctrl = TextEditingController(
      text: deviceTime ?? _fmtDateTime(DateTime.now()),
    );

    final action = await showDialog<String>(
      context: context,
      builder: (ctx) => AlertDialog(
        title: const Text('현재시간설정'),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Text(
              deviceTime == null
                  ? '장비 시각을 읽지 못했습니다. 아래 값을 확인하세요.'
                  : '장비 시각: $deviceTime',
            ),
            const SizedBox(height: 12),
            TextField(
              controller: ctrl,
              decoration: const InputDecoration(
                border: OutlineInputBorder(),
                labelText: '설정할 시간',
                hintText: 'YYYY-MM-DD HH:MM:SS',
              ),
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(ctx),
            child: const Text('취소'),
          ),
          TextButton(
            onPressed: () => Navigator.pop(ctx, 'typed'),
            child: const Text('전송'),
          ),
          FilledButton(
            onPressed: () => Navigator.pop(ctx, 'phone'),
            child: const Text('현재시간 전송'),
          ),
        ],
      ),
    );

    if (!mounted) return;
    if (action == 'typed') {
      final v = ctrl.text.trim();
      ctrl.dispose();
      if (v.isEmpty) return;
      await _sendAndShowLog('time $v');
    } else if (action == 'phone') {
      ctrl.dispose();
      await _sendAndShowLog('time ${_fmtDateTime(DateTime.now())}');
    } else {
      ctrl.dispose();
    }
  }

  Future<void> _checkLatestFirmware() async {
    final ble = context.read<NusBleService>();
    if (!ble.isConnected) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('먼저 장비를 연결하세요')),
      );
      return;
    }
    setState(() => _fwChecking = true);
    try {
      await ble.fetchFwInfo();
      if (!mounted) return;
      final msg = ble.fwCheckError != null
          ? '확인 실패: ${ble.fwCheckError}'
          : ble.updateAvailable
              ? '새 펌웨어 ${ble.serverLatest} (현재 ${ble.deviceFwVersion})'
              : ble.fwIsLatest
                  ? '최신 버전입니다 (${ble.deviceFwVersion})'
                  : '버전을 비교할 수 없습니다';
      ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text(msg)));
    } finally {
      if (mounted) setState(() => _fwChecking = false);
    }
  }

  Future<void> _openConnect() async {
    if (_connectOpening || _logOpening) return;
    _connectOpening = true;
    try {
      await Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const ScanScreen()),
      );
      if (!mounted) return;
      final ble = context.read<NusBleService>();
      if (ble.isConnected) {
        _wifiFieldsTouched = false;
        ble.fetchStoredWifi();
      }
    } finally {
      _connectOpening = false;
    }
  }

  Future<void> _sendAndShowLog(String command) async {
    await context.read<NusBleService>().sendCommand(command);
    if (mounted) await _openLogPage();
  }

  Future<void> _openLogPage() async {
    if (_logOpening || _connectOpening) return;
    _logOpening = true;
    try {
      await Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const LogScreen()),
      );
    } finally {
      _logOpening = false;
    }
  }

  @override
  Widget build(BuildContext context) {
    final ble = context.watch<NusBleService>();
    if (ble.wifiConfigVersion != _lastWifiVersion) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        if (!mounted) return;
        _syncWifiFields(context.read<NusBleService>());
      });
    }
    final name = ble.device?.platformName ?? 'UPS43D1P';
    final storedHint = ble.deviceSsid == null
        ? null
        : '장비 저장값: ${ble.deviceSsid}'
            '${(ble.devicePass == null || ble.devicePass!.isEmpty) ? ' / (open)' : ' / ••••••'}';

    return GestureDetector(
      onHorizontalDragEnd: (details) {
        final v = details.primaryVelocity ?? 0;
        if (v < -400) {
          _openLogPage();
        } else if (v > 400) {
          _openConnect();
        }
      },
      child: Scaffold(
        appBar: AppBar(
          title: Text(ble.isConnected ? name : 'UPS43D1P'),
          actions: [
            IconButton(
              tooltip: '로그',
              onPressed: _openLogPage,
              icon: const Icon(Icons.article_outlined),
            ),
            if (ble.isConnected)
              IconButton(
                tooltip: '연결 해제',
                onPressed: () async {
                  await ble.disconnect();
                },
                icon: const Icon(Icons.link_off),
              )
            else
              IconButton(
                tooltip: '장비 연결',
                onPressed: _openConnect,
                icon: const Icon(Icons.bluetooth),
              ),
          ],
        ),
        body: SafeArea(
          top: false,
          child: Column(
          children: [
            Padding(
              padding: const EdgeInsets.fromLTRB(16, 12, 16, 0),
              child: FilledButton.icon(
                onPressed: _openConnect,
                icon: Icon(
                  ble.isConnected ? Icons.bluetooth_connected : Icons.bluetooth,
                ),
                label: Text(ble.isConnected ? '장비 변경' : '장비 연결'),
              ),
            ),
            Expanded(
              child: SingleChildScrollView(
                padding: const EdgeInsets.fromLTRB(16, 12, 16, 24),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.stretch,
                  children: [
                    InkWell(
                      borderRadius: BorderRadius.circular(8),
                      onTap: () {
                        final opening = !_wifiExpanded;
                        setState(() => _wifiExpanded = opening);
                        if (opening) {
                          _wifiFieldsTouched = false;
                          ble.fetchStoredWifi();
                        }
                      },
                      child: Padding(
                        padding: const EdgeInsets.symmetric(vertical: 4),
                        child: Row(
                          children: [
                            Icon(
                              _wifiExpanded
                                  ? Icons.expand_less
                                  : Icons.expand_more,
                              size: 22,
                            ),
                            const SizedBox(width: 4),
                            Text(
                              'Wi-Fi / OTA',
                              style: Theme.of(context).textTheme.titleMedium,
                            ),
                            if (!_wifiExpanded && storedHint != null) ...[
                              const SizedBox(width: 8),
                              Expanded(
                                child: Text(
                                  storedHint,
                                  overflow: TextOverflow.ellipsis,
                                  style: Theme.of(context)
                                      .textTheme
                                      .bodySmall
                                      ?.copyWith(
                                        color: Theme.of(context)
                                            .colorScheme
                                            .onSurfaceVariant,
                                      ),
                                ),
                              ),
                            ] else
                              const Spacer(),
                            if (_wifiExpanded)
                              TextButton.icon(
                                onPressed:
                                    !ble.isConnected ? null : _reloadFromDevice,
                                icon: const Icon(Icons.download, size: 18),
                                label: const Text('장비값 읽기'),
                              ),
                          ],
                        ),
                      ),
                    ),
                    AnimatedCrossFade(
                      firstChild: const SizedBox.shrink(),
                      secondChild: _buildWifiSection(ble, storedHint),
                      crossFadeState: _wifiExpanded
                          ? CrossFadeState.showSecond
                          : CrossFadeState.showFirst,
                      duration: const Duration(milliseconds: 250),
                    ),
                    const SizedBox(height: 16),
                    if (ble.updateAvailable)
                      Card(
                        color: Theme.of(context).colorScheme.primaryContainer,
                        child: ListTile(
                          leading: const Icon(Icons.new_releases),
                          title: Text(
                            '새 펌웨어 ${ble.serverLatest}',
                          ),
                          subtitle: Text(
                            '현재 ${ble.deviceFwVersion ?? '-'}',
                          ),
                          trailing: TextButton(
                            onPressed: () => OtaUpgradeFlow.start(context, ble),
                            child: const Text('업그레이드'),
                          ),
                        ),
                      )
                    else if (ble.fwIsLatest)
                      Card(
                        color: Theme.of(context).colorScheme.surfaceContainerHighest,
                        child: ListTile(
                          leading: const Icon(Icons.verified),
                          title: const Text('최신 버전입니다'),
                          subtitle: Text(
                            '현재 ${ble.deviceFwVersion} · 서버 ${ble.serverLatest}',
                          ),
                        ),
                      ),
                    if (ble.updateAvailable || ble.fwIsLatest)
                      const SizedBox(height: 8),
                    FilledButton.tonalIcon(
                      onPressed: !ble.isConnected || _fwChecking
                          ? null
                          : _checkLatestFirmware,
                      icon: _fwChecking
                          ? const SizedBox(
                              width: 18,
                              height: 18,
                              child: CircularProgressIndicator(strokeWidth: 2),
                            )
                          : const Icon(Icons.refresh),
                      label: Text(_fwChecking ? '확인 중…' : '최신 버전 확인'),
                    ),
                    const SizedBox(height: 8),
                    FilledButton.icon(
                      onPressed: !ble.isConnected
                          ? null
                          : () => OtaUpgradeFlow.start(context, ble),
                      icon: const Icon(Icons.system_update_alt),
                      label: Text(
                        ble.updateAvailable
                            ? '펌웨어 업그레이드 (새 버전)'
                            : '펌웨어 업그레이드',
                      ),
                    ),
                    if (ble.isConnected)
                      Padding(
                        padding: const EdgeInsets.only(top: 6),
                        child: Text(
                          ble.fwCheckError != null
                              ? '펌웨어 확인: ${ble.fwCheckError}'
                              : '현재 ${ble.deviceFwVersion ?? '-'}'
                                  '${ble.serverLatest != null ? ' · 서버 ${ble.serverLatest}' : ''}'
                                  '${ble.updateAvailable ? ' · 새 버전' : (ble.serverLatest != null ? ' · 최신' : '')}',
                          style: Theme.of(context).textTheme.bodySmall?.copyWith(
                                color: ble.updateAvailable
                                    ? Theme.of(context).colorScheme.primary
                                    : Theme.of(context)
                                        .colorScheme
                                        .onSurfaceVariant,
                              ),
                        ),
                      ),
                    const SizedBox(height: 20),
                    InkWell(
                      borderRadius: BorderRadius.circular(8),
                      onTap: () => setState(
                          () => _quickCmdExpanded = !_quickCmdExpanded),
                      child: Padding(
                        padding: const EdgeInsets.symmetric(vertical: 4),
                        child: Row(
                          children: [
                            Icon(
                              _quickCmdExpanded
                                  ? Icons.expand_less
                                  : Icons.expand_more,
                              size: 22,
                            ),
                            const SizedBox(width: 4),
                            Text(
                              '빠른 명령',
                              style: Theme.of(context).textTheme.titleMedium,
                            ),
                          ],
                        ),
                      ),
                    ),
                    AnimatedCrossFade(
                      firstChild: const SizedBox.shrink(),
                      secondChild: Padding(
                        padding: const EdgeInsets.only(top: 8),
                        child: Wrap(
                          spacing: 8,
                          runSpacing: 8,
                          children: [
                            _CmdChip(
                              label: 'fw',
                              onTap: () => _sendAndShowLog('fw'),
                            ),
                            _CmdChip(
                              label: 'help',
                              onTap: () => _sendAndShowLog('help'),
                            ),
                            _CmdChip(
                              label: 'version',
                              onTap: () => _sendAndShowLog('version'),
                            ),
                            _CmdChip(
                              label: 'ip',
                              onTap: () => _sendAndShowLog('ip'),
                            ),
                            _CmdChip(
                              label: 'ssid?',
                              onTap: () async {
                                _wifiFieldsTouched = false;
                                ble.fetchStoredWifi();
                                await _openLogPage();
                              },
                            ),
                            _CmdChip(
                              label: 'ls',
                              onTap: () => _sendAndShowLog('ls'),
                            ),
                            _CmdChip(
                              label: 'df',
                              onTap: () => _sendAndShowLog('df'),
                            ),
                            _CmdChip(
                              label: 'update',
                              color: Colors.deepOrange,
                              onTap: () =>
                                  OtaUpgradeFlow.start(context, ble),
                            ),
                            _CmdChip(
                              label: '현재시간설정',
                              onTap: _openTimeSetDialog,
                            ),
                            _CmdChip(
                              label: 'reboot',
                              color: Colors.redAccent,
                              onTap: () =>
                                  _confirmAndSend('재부팅', 'reboot'),
                            ),
                          ],
                        ),
                      ),
                      crossFadeState: _quickCmdExpanded
                          ? CrossFadeState.showSecond
                          : CrossFadeState.showFirst,
                      duration: const Duration(milliseconds: 250),
                    ),
                    const SizedBox(height: 20),
                    Text(
                      '직접 명령',
                      style: Theme.of(context).textTheme.titleMedium,
                    ),
                    const SizedBox(height: 8),
                    Row(
                      children: [
                        Expanded(
                          child: TextField(
                            controller: _cmdCtrl,
                            decoration: const InputDecoration(
                              border: OutlineInputBorder(),
                              hintText: '예: cat eventLog.hex',
                            ),
                            onSubmitted: (v) async {
                              await ble.sendCommand(v);
                              _cmdCtrl.clear();
                            },
                          ),
                        ),
                        const SizedBox(width: 8),
                        FilledButton(
                          onPressed: !ble.isConnected
                              ? null
                              : () async {
                                  await ble.sendCommand(_cmdCtrl.text);
                                  _cmdCtrl.clear();
                                },
                          child: const Text('전송'),
                        ),
                      ],
                    ),
                    const SizedBox(height: 24),
                  ],
                ),
              ),
            ),
            if (!ble.isConnected)
              MaterialBanner(
                content: const Text('장비가 연결되지 않았습니다'),
                actions: [
                  TextButton(
                    onPressed: _openConnect,
                    child: const Text('연결'),
                  ),
                ],
              ),
          ],
        ),
        ),
      ),
    );
  }

  Widget _buildWifiSection(NusBleService ble, String? storedHint) {
    final isCustom = _wifiPresetId == kWifiPresetCustomId;
    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      children: [
        if (storedHint != null) ...[
          const SizedBox(height: 4),
          Text(
            storedHint,
            style: Theme.of(context).textTheme.bodySmall?.copyWith(
                  color: Theme.of(context).colorScheme.onSurfaceVariant,
                ),
          ),
        ],
        const SizedBox(height: 8),
        Text(
          '프리셋 선택 (탭하면 SSID/PASS 자동 입력)',
          style: Theme.of(context).textTheme.bodySmall,
        ),
        const SizedBox(height: 6),
        Wrap(
          spacing: 8,
          runSpacing: 8,
          children: [
            for (final p in kWifiPresets)
              ChoiceChip(
                label: Text(p.label),
                selected: _wifiPresetId == p.id,
                onSelected: (_) => _selectWifiPreset(p.id),
              ),
            ChoiceChip(
              label: const Text('직접 입력'),
              selected: isCustom,
              onSelected: (_) => _selectWifiPreset(kWifiPresetCustomId),
            ),
          ],
        ),
        const SizedBox(height: 10),
        Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Expanded(
              child: TextField(
                controller: _ssidCtrl,
                readOnly: !isCustom,
                decoration: InputDecoration(
                  labelText: 'SSID',
                  border: const OutlineInputBorder(),
                  hintText: isCustom ? '무선 AP 이름' : null,
                ),
                textInputAction: TextInputAction.next,
              ),
            ),
            const SizedBox(width: 8),
            Padding(
              padding: const EdgeInsets.only(top: 4),
              child: FilledButton.tonalIcon(
                onPressed: () async {
                  if (!isCustom) _selectWifiPreset(kWifiPresetCustomId);
                  await _pickWifiSsid();
                },
                icon: const Icon(Icons.search),
                label: const Text('WIFI-검색'),
              ),
            ),
          ],
        ),
        const SizedBox(height: 10),
        TextField(
          controller: _passCtrl,
          readOnly: !isCustom,
          obscureText: _obscurePass && isCustom,
          decoration: InputDecoration(
            labelText: '비밀번호',
            border: const OutlineInputBorder(),
            hintText: '비우면 open (pass none)',
            suffixIcon: isCustom
                ? IconButton(
                    onPressed: () =>
                        setState(() => _obscurePass = !_obscurePass),
                    icon: Icon(
                      _obscurePass
                          ? Icons.visibility
                          : Icons.visibility_off,
                    ),
                  )
                : null,
          ),
        ),
        if (!isCustom) ...[
          const SizedBox(height: 6),
          Text(
            'PASS: ${kWifiPresets.firstWhere((p) => p.id == _wifiPresetId).passHint}',
            style: Theme.of(context).textTheme.bodySmall?.copyWith(
                  color: Theme.of(context).colorScheme.onSurfaceVariant,
                ),
          ),
        ],
        const SizedBox(height: 10),
        FilledButton.icon(
          onPressed: !ble.isConnected ? null : _sendWifi,
          icon: const Icon(Icons.upload),
          label: const Text('SSID / PASS 장비로 전송'),
        ),
      ],
    );
  }
}

class _CmdChip extends StatelessWidget {
  const _CmdChip({
    required this.label,
    required this.onTap,
    this.color,
  });

  final String label;
  final VoidCallback onTap;
  final Color? color;

  @override
  Widget build(BuildContext context) {
    return ActionChip(
      label: Text(label),
      avatar: Icon(
        Icons.terminal,
        size: 16,
        color: color ?? Theme.of(context).colorScheme.primary,
      ),
      onPressed: onTap,
      side: color == null
          ? null
          : BorderSide(color: color!.withValues(alpha: 0.5)),
    );
  }
}
