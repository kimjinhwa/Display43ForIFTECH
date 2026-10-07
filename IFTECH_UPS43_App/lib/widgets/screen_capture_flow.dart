import 'dart:async';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:share_plus/share_plus.dart';

import '../ble/nus_ble_service.dart';
import '../ble/screen_capture.dart';

/// `scr` 로 현재 화면을 받아 보여주고, 저장을 고르면 공유 시트로 PNG를 넘긴다.
Future<void> runScreenCapture(BuildContext context, NusBleService ble) async {
  final progress = ValueNotifier<String>('화면을 그리는 중…');
  final nav = Navigator.of(context, rootNavigator: true);
  final route = DialogRoute<void>(
    context: context,
    barrierDismissible: false,
    builder: (ctx) => PopScope(
      canPop: false,
      child: AlertDialog(
        content: ValueListenableBuilder<String>(
          valueListenable: progress,
          builder: (_, text, _) => Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              const CircularProgressIndicator(),
              const SizedBox(height: 16),
              Text(text, textAlign: TextAlign.center),
            ],
          ),
        ),
      ),
    ),
  );
  final dialogFuture = nav.push(route);

  await WidgetsBinding.instance.endOfFrame;

  ScreenCaptureResult? shot;
  Object? error;
  try {
    shot = await ble.captureScreen(
      onProgress: (status) => progress.value = status,
    );
  } catch (e) {
    error = e;
  }

  if (route.isActive) {
    nav.removeRoute(route);
  }
  await dialogFuture;
  progress.dispose();

  if (!context.mounted) return;
  final image = shot;
  if (error != null || image == null) {
    final message = error is TimeoutException
        ? '응답 시간이 초과했습니다. 다시 시도해 주세요.'
        : error is ScreenCaptureException
            ? error.message
            : '화면 캡처 실패: $error';
    ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text(message)));
    return;
  }

  final save = await showDialog<bool>(
    context: context,
    builder: (ctx) => AlertDialog(
      title: Text('화면 캡처 ${image.width}×${image.height}'),
      content: SizedBox(
        width: double.maxFinite,
        child: InteractiveViewer(
          minScale: 0.8,
          maxScale: 4,
          child: Image.memory(image.png, fit: BoxFit.contain),
        ),
      ),
      actions: [
        TextButton(
          onPressed: () => Navigator.pop(ctx, false),
          child: const Text('닫기'),
        ),
        FilledButton(
          onPressed: () => Navigator.pop(ctx, true),
          child: const Text('저장'),
        ),
      ],
    ),
  );

  if (save != true || !context.mounted) return;
  try {
    await _sharePng(context, image);
  } catch (e) {
    if (!context.mounted) return;
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(content: Text('저장 화면을 열지 못했습니다: $e')),
    );
  }
}

Future<void> _sharePng(BuildContext context, ScreenCaptureResult shot) async {
  final box = context.findRenderObject() as RenderBox?;
  final origin = (box != null && box.hasSize && box.size.width > 0 && box.size.height > 0)
      ? box.localToGlobal(Offset.zero) & box.size
      : const Rect.fromLTWH(0, 0, 1, 1);

  final stamp = DateTime.now();
  String two(int n) => n.toString().padLeft(2, '0');
  final name =
      'ups_${stamp.year}${two(stamp.month)}${two(stamp.day)}_${two(stamp.hour)}${two(stamp.minute)}${two(stamp.second)}.png';
  final file = File('${Directory.systemTemp.path}${Platform.pathSeparator}$name');
  await file.writeAsBytes(shot.png, flush: true);

  await Share.shareXFiles(
    [XFile(file.path, mimeType: 'image/png', name: name)],
    subject: 'UPS 화면 $name',
    sharePositionOrigin: origin,
  );
}
