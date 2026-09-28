import 'package:flutter_test/flutter_test.dart';
import 'package:iftech_ups_app/main.dart';

void main() {
  testWidgets('App loads main screen', (WidgetTester tester) async {
    await tester.pumpWidget(const Ups43D1pApp());
    expect(find.text('UPS43D1P'), findsWidgets);
    expect(find.text('장비 연결'), findsOneWidget);
  });
}
