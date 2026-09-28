import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../ble/nus_ble_service.dart';
import '../widgets/log_console.dart';

class LogScreen extends StatelessWidget {
  const LogScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final ble = context.watch<NusBleService>();
    return GestureDetector(
      onHorizontalDragEnd: (details) {
        final v = details.primaryVelocity ?? 0;
        if (v.abs() > 400 && Navigator.of(context).canPop()) {
          Navigator.of(context).pop();
        }
      },
      child: Scaffold(
        appBar: AppBar(
          title: const Text('로그'),
        ),
        body: Padding(
          padding: const EdgeInsets.fromLTRB(12, 0, 12, 12),
          child: SizedBox.expand(
            child: LogConsole(
              lines: ble.logLines,
              shareTitle: 'UPS43D1P Log',
              onClear: () => ble.clearLog(),
            ),
          ),
        ),
      ),
    );
  }
}
