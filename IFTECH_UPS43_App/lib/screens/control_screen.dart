import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:share_plus/share_plus.dart';

import '../ble/nus_ble_service.dart';
import '../ble/wifi_presets.dart';
import '../widgets/log_console.dart';
import '../widgets/ota_upgrade_flow.dart';
import '../widgets/wifi_scan_sheet.dart';

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

  @override
  void initState() {
    super.initState();
    _applyPreset(kWifiPresets.first, markTouched: false);
    _ssidCtrl.addListener(_onWifiFieldEdited);
    _passCtrl.addListener(_onWifiFieldEdited);
    WidgetsBinding.instance.addPostFrameCallback((_) {
      final ble = context.read<NusBleService>();
      _syncWifiFields(ble, force: true);
      ble.fetchStoredWifi();
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
    }
  }

  void _openFullScreenLog() {
    final ble = context.read<NusBleService>();
    Navigator.of(context).push(
      MaterialPageRoute(
        builder: (_) => FullScreenLogPage(
          title: '로그',
          lines: ble.logLines,
          shareTitle: 'UPS43D1P Log',
          onClear: () => ble.clearLog(),
        ),
      ),
    );
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

    return PopScope(
      canPop: true,
      onPopInvokedWithResult: (didPop, result) async {
        if (didPop) {
          await context.read<NusBleService>().disconnect();
        }
      },
      child: Scaffold(
        appBar: AppBar(
          title: Text(name),
          actions: [
            IconButton(
              tooltip: '로그 공유',
              onPressed: ble.logLines.isEmpty
                  ? null
                  : () async {
                      final text = ble.logLines.join('\n');
                      final box = context.findRenderObject() as RenderBox?;
                      await Share.share(
                        text,
                        subject: 'UPS43D1P Log - $name',
                        sharePositionOrigin: box != null
                            ? box.localToGlobal(Offset.zero) & box.size
                            : null,
                      );
                    },
              icon: const Icon(Icons.share),
            ),
            IconButton(
              tooltip: '로그 지우기',
              onPressed: () => ble.clearLog(),
              icon: const Icon(Icons.delete_outline),
            ),
            IconButton(
              tooltip: '연결 해제',
              onPressed: () async {
                await ble.disconnect();
                if (context.mounted) Navigator.of(context).pop();
              },
              icon: const Icon(Icons.link_off),
            ),
          ],
        ),
        body: Column(
          children: [
            Expanded(
              flex: 5,
              child: SingleChildScrollView(
                padding: const EdgeInsets.fromLTRB(16, 12, 16, 8),
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
                    FilledButton.icon(
                      onPressed: !ble.isConnected
                          ? null
                          : () => OtaUpgradeFlow.start(context, ble),
                      icon: const Icon(Icons.system_update_alt),
                      label: const Text('펌웨어 업그레이드'),
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
                              label: 'help',
                              onTap: () => ble.sendCommand('help'),
                            ),
                            _CmdChip(
                              label: 'version',
                              onTap: () => ble.sendCommand('version'),
                            ),
                            _CmdChip(
                              label: 'ip',
                              onTap: () => ble.sendCommand('ip'),
                            ),
                            _CmdChip(
                              label: 'ssid?',
                              onTap: () {
                                _wifiFieldsTouched = false;
                                ble.fetchStoredWifi();
                              },
                            ),
                            _CmdChip(
                              label: 'ls',
                              onTap: () => ble.sendCommand('ls'),
                            ),
                            _CmdChip(
                              label: 'df',
                              onTap: () => ble.sendCommand('df'),
                            ),
                            _CmdChip(
                              label: 'update',
                              color: Colors.deepOrange,
                              onTap: () =>
                                  OtaUpgradeFlow.start(context, ble),
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
                  ],
                ),
              ),
            ),
            Expanded(
              flex: 4,
              child: Padding(
                padding: const EdgeInsets.fromLTRB(12, 0, 12, 12),
                child: LogConsole(
                  lines: ble.logLines,
                  shareTitle: 'UPS43D1P Log - $name',
                  onClear: () => ble.clearLog(),
                  onFullScreen: _openFullScreenLog,
                ),
              ),
            ),
            if (!ble.isConnected)
              MaterialBanner(
                content: const Text('연결이 끊어졌습니다'),
                actions: [
                  TextButton(
                    onPressed: () => Navigator.of(context).pop(),
                    child: const Text('뒤로'),
                  ),
                ],
              ),
          ],
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
        TextField(
          controller: _ssidCtrl,
          readOnly: !isCustom,
          decoration: InputDecoration(
            labelText: 'SSID',
            border: const OutlineInputBorder(),
            hintText: isCustom ? '무선 AP 이름' : null,
            suffixIcon: isCustom
                ? IconButton(
                    tooltip: '주변 Wi-Fi 검색',
                    onPressed: _pickWifiSsid,
                    icon: const Icon(Icons.wifi_find),
                  )
                : null,
          ),
          textInputAction: TextInputAction.next,
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
