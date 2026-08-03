import 'package:flutter/material.dart';

import '../ble/nus_ble_service.dart';

/// 펌웨어 업그레이드: 장치에 저장된 Wi-Fi로 update CLI 실행
class OtaUpgradeFlow {
  OtaUpgradeFlow._();

  static Future<void> start(BuildContext context, NusBleService ble) async {
    if (!ble.isConnected) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('장치가 연결되지 않았습니다')),
      );
      return;
    }

    final ssid = ble.deviceSsid ?? '(미확인)';
    final pass = (ble.devicePass == null || ble.devicePass!.isEmpty)
        ? '(open)'
        : '••••••';

    final ok = await showDialog<bool>(
      context: context,
      builder: (ctx) => AlertDialog(
        title: const Text('펌웨어 업그레이드'),
        content: Text(
          '장치에 저장된 Wi-Fi로 ift.iptime.org 서버에서 펌웨어를 확인합니다.\n'
          'OTA 후 장치가 재부팅됩니다.\n\n'
          'SSID: $ssid\n'
          'PASS: $pass',
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(ctx, false),
            child: const Text('취소'),
          ),
          FilledButton(
            onPressed: () => Navigator.pop(ctx, true),
            child: const Text('업그레이드'),
          ),
        ],
      ),
    );
    if (ok != true || !context.mounted) return;

    ScaffoldMessenger.of(context).showSnackBar(
      const SnackBar(content: Text('update 명령 전송…')),
    );

    await ble.sendCommand('update');
  }
}
