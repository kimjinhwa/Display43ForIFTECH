import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'dart:ui' as ui;

/// RGB565 화면 한 장. 앱에서 PNG로 디코딩한 결과.
class ScreenCaptureResult {
  const ScreenCaptureResult({
    required this.png,
    required this.width,
    required this.height,
  });

  final Uint8List png;
  final int width;
  final int height;
}

class ScreenCaptureException implements Exception {
  ScreenCaptureException(this.message);
  final String message;

  @override
  String toString() => message;
}

class _ScrHeader {
  const _ScrHeader({
    required this.width,
    required this.height,
    required this.swap,
    required this.lines,
    required this.bands,
  });

  final int width;
  final int height;
  final int swap;
  final int lines;
  final int bands;
}

_ScrHeader? _parseHeader(String text) {
  final m = RegExp(
    r'SCR\s+W=(\d+)\s+H=(\d+)\s+CF=RGB565\s+SWAP=(\d+)\s+LINES=(\d+)\s+BANDS=(\d+)',
  ).firstMatch(text);
  if (m == null) return null;
  return _ScrHeader(
    width: int.parse(m.group(1)!),
    height: int.parse(m.group(2)!),
    swap: int.parse(m.group(3)!),
    lines: int.parse(m.group(4)!),
    bands: int.parse(m.group(5)!),
  );
}

Uint8List _assembleRaw(String text, _ScrHeader header) {
  final re = RegExp(
    r'SCRB\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s*\r?\n([A-Za-z0-9+/=\r\n]*?)\r?\nSCRE\s+\1\b',
  );
  final raw = Uint8List(header.width * header.height * 2);
  final got = List<bool>.filled(header.bands, false);

  for (final m in re.allMatches(text)) {
    final index = int.parse(m.group(1)!);
    final y = int.parse(m.group(2)!);
    final bandH = int.parse(m.group(3)!);
    final nbytes = int.parse(m.group(4)!);
    if (index < 0 || index >= header.bands) {
      throw ScreenCaptureException('밴드 번호가 범위를 벗어났습니다.');
    }
    final body = (m.group(5) ?? '').replaceAll(RegExp(r'\s+'), '');
    Uint8List bytes;
    try {
      bytes = base64.decode(body);
    } on FormatException {
      throw ScreenCaptureException('밴드 $index 데이터가 깨졌습니다.');
    }
    if (bytes.length != nbytes || nbytes != header.width * bandH * 2) {
      throw ScreenCaptureException('밴드 $index 길이가 맞지 않습니다.');
    }
    final offset = y * header.width * 2;
    if (y < 0 || bandH <= 0 || offset + bytes.length > raw.length) {
      throw ScreenCaptureException('밴드 $index 위치가 맞지 않습니다.');
    }
    raw.setRange(offset, offset + bytes.length, bytes);
    got[index] = true;
  }

  for (var i = 0; i < got.length; i++) {
    if (!got[i]) {
      throw ScreenCaptureException('밴드 $i 을 받지 못했습니다.');
    }
  }
  return raw;
}

Future<Uint8List> _rgb565ToPng(Uint8List raw, int width, int height, int swap) async {
  if (raw.length != width * height * 2) {
    throw ScreenCaptureException('화면 데이터 크기가 맞지 않습니다.');
  }
  final rgba = Uint8List(width * height * 4);
  var o = 0;
  for (var i = 0; i < raw.length; i += 2) {
    final a = raw[i];
    final b = raw[i + 1];
    final c = swap == 0 ? (a | (b << 8)) : ((a << 8) | b);
    final r5 = (c >> 11) & 31;
    final g6 = (c >> 5) & 63;
    final b5 = c & 31;
    rgba[o++] = (r5 << 3) | (r5 >> 2);
    rgba[o++] = (g6 << 2) | (g6 >> 4);
    rgba[o++] = (b5 << 3) | (b5 >> 2);
    rgba[o++] = 255;
  }

  final completer = Completer<ui.Image>();
  ui.decodeImageFromPixels(
    rgba,
    width,
    height,
    ui.PixelFormat.rgba8888,
    completer.complete,
  );
  final image = await completer.future;
  try {
    final data = await image.toByteData(format: ui.ImageByteFormat.png);
    if (data == null) {
      throw ScreenCaptureException('PNG로 만들지 못했습니다.');
    }
    return data.buffer.asUint8List();
  } finally {
    image.dispose();
  }
}

/// 펌웨어 `scr` 응답 전체를 PNG로 만든다.
Future<ScreenCaptureResult> decodeScreenCapture(String text) async {
  if (text.contains('SCR ERR')) {
    final err = RegExp(r'SCR ERR\s+(\S+)').firstMatch(text);
    final code = err?.group(1) ?? '';
    if (code == 'nomem') {
      throw ScreenCaptureException('캡처에 쓸 메모리를 잡지 못했습니다.');
    }
    if (code == 'nodisp') {
      throw ScreenCaptureException('화면이 아직 준비되지 않았습니다.');
    }
    throw ScreenCaptureException('화면 캡처 오류: $code');
  }

  final header = _parseHeader(text);
  if (header == null) {
    if (text.contains('ERROR:')) {
      throw ScreenCaptureException(
        '이 펌웨어는 화면 캡처(scr)를 지원하지 않습니다.',
      );
    }
    throw ScreenCaptureException('화면 정보를 받지 못했습니다.');
  }
  if (!text.contains('SCR DONE')) {
    throw ScreenCaptureException('화면 데이터가 끝까지 오지 않았습니다.');
  }

  final raw = _assembleRaw(text, header);
  final png = await _rgb565ToPng(raw, header.width, header.height, header.swap);
  return ScreenCaptureResult(png: png, width: header.width, height: header.height);
}

/// 진행 표시용. 헤더의 BANDS 와 지금까지 도착한 SCRE 개수.
class ScreenCaptureProgress {
  const ScreenCaptureProgress({required this.bandsDone, required this.bandsTotal});

  final int bandsDone;
  final int bandsTotal;

  String get label {
    if (bandsTotal <= 0) return '화면을 그리는 중…';
    return '받는 중 $bandsDone / $bandsTotal';
  }
}
