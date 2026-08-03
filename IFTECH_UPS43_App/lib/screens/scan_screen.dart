import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:provider/provider.dart';

import '../ble/nus_ble_service.dart';
import 'control_screen.dart';

class ScanScreen extends StatefulWidget {
  const ScanScreen({super.key});

  @override
  State<ScanScreen> createState() => _ScanScreenState();
}

class _ScanScreenState extends State<ScanScreen> {
  bool _requesting = false;

  Future<bool> _requestPermissions() async {
    setState(() => _requesting = true);
    try {
      final statuses = await [
        Permission.bluetoothScan,
        Permission.bluetoothConnect,
        Permission.locationWhenInUse,
      ].request();

      final scanOk = statuses[Permission.bluetoothScan]?.isGranted ?? true;
      final connectOk = statuses[Permission.bluetoothConnect]?.isGranted ?? true;
      // Location is required on older Android for BLE scan
      final loc = statuses[Permission.locationWhenInUse];
      final locOk = loc == null ||
          loc.isGranted ||
          loc.isLimited ||
          loc.isPermanentlyDenied == false;

      if (!scanOk || !connectOk) {
        if (mounted) {
          ScaffoldMessenger.of(context).showSnackBar(
            const SnackBar(content: Text('블루투스 권한이 필요합니다.')),
          );
        }
        return false;
      }
      if (loc != null && loc.isPermanentlyDenied) {
        if (mounted) {
          ScaffoldMessenger.of(context).showSnackBar(
            const SnackBar(
              content: Text('위치 권한이 거부되었습니다. 설정에서 허용해 주세요.'),
            ),
          );
        }
        await openAppSettings();
        return false;
      }
      return locOk || scanOk;
    } finally {
      if (mounted) setState(() => _requesting = false);
    }
  }

  Future<void> _onScan() async {
    final ok = await _requestPermissions();
    if (!ok) return;
    if (!mounted) return;
    await context.read<NusBleService>().startScan();
  }

  Future<void> _onConnect(ScanResult result) async {
    final ble = context.read<NusBleService>();
    await ble.connect(result.device);
    if (!mounted) return;
    if (ble.isConnected) {
      await Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const ControlScreen()),
      );
    } else if (ble.error != null) {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text(ble.error!)),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    final ble = context.watch<NusBleService>();
    final scanning = ble.state == BleConnectionState.scanning;
    final connecting = ble.state == BleConnectionState.connecting;

    return Scaffold(
      appBar: AppBar(
        title: const Text('UPS43D1P'),
        actions: [
          if (scanning || connecting || _requesting)
            const Padding(
              padding: EdgeInsets.only(right: 16),
              child: Center(
                child: SizedBox(
                  width: 22,
                  height: 22,
                  child: CircularProgressIndicator(strokeWidth: 2),
                ),
              ),
            ),
        ],
      ),
      body: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Padding(
            padding: const EdgeInsets.fromLTRB(16, 16, 16, 8),
            child: Text(
              'IFT_UPS 장비를 검색합니다.\nBLE Nordic UART 방식입니다.',
              style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                    color: Theme.of(context).colorScheme.onSurfaceVariant,
                  ),
            ),
          ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 16),
            child: FilledButton.icon(
              onPressed: scanning || connecting ? null : _onScan,
              icon: Icon(scanning ? Icons.bluetooth_searching : Icons.search),
              label: Text(scanning ? '검색 중…' : '장비 검색'),
            ),
          ),
          if (ble.error != null)
            Padding(
              padding: const EdgeInsets.fromLTRB(16, 12, 16, 0),
              child: Text(
                ble.error!,
                style: TextStyle(color: Theme.of(context).colorScheme.error),
              ),
            ),
          const SizedBox(height: 8),
          Expanded(
            child: ble.scanResults.isEmpty
                ? Center(
                    child: Text(
                      scanning ? '주변 장비를 찾는 중…' : '검색된 장비가 없습니다',
                      style: TextStyle(
                        color: Theme.of(context).colorScheme.onSurfaceVariant,
                      ),
                    ),
                  )
                : ListView.separated(
                    padding: const EdgeInsets.all(12),
                    itemCount: ble.scanResults.length,
                    separatorBuilder: (_, _) => const SizedBox(height: 8),
                    itemBuilder: (context, index) {
                      final r = ble.scanResults[index];
                      final name = r.device.platformName.isNotEmpty
                          ? r.device.platformName
                          : '(이름 없음)';
                      return Card(
                        child: ListTile(
                          leading: CircleAvatar(
                            backgroundColor: Theme.of(context)
                                .colorScheme
                                .primaryContainer,
                            child: const Icon(Icons.bluetooth),
                          ),
                          title: Text(name),
                          subtitle: Text(
                            '${r.device.remoteId.str}\nRSSI ${r.rssi} dBm',
                          ),
                          isThreeLine: true,
                          trailing: connecting
                              ? const SizedBox(
                                  width: 24,
                                  height: 24,
                                  child: CircularProgressIndicator(strokeWidth: 2),
                                )
                              : const Icon(Icons.chevron_right),
                          onTap: connecting ? null : () => _onConnect(r),
                        ),
                      );
                    },
                  ),
          ),
        ],
      ),
    );
  }
}
