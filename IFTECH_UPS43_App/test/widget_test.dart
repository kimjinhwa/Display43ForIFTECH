import 'package:flutter_test/flutter_test.dart';
import 'package:iftech_ups_app/main.dart';

void main() {
  testWidgets('App loads scan screen', (WidgetTester tester) async {
    await tester.pumpWidget(const IftechUpsApp());
    expect(find.text('IFTECH UPS'), findsOneWidget);
    expect(find.text('장비 검색'), findsOneWidget);
  });
}
