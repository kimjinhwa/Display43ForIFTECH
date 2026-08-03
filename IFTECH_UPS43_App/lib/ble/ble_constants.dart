/// ESP32 Nordic UART Service (NUS) UUIDs — matches myBlueTooth.h
class BleConstants {
  /// Advertised as IFT_UPS… (e.g. IFT_UPS43_ + MAC)
  static const String deviceNamePrefix = 'IFT_UPS';

  static const String serviceUuid = '6E400001-B5A3-F393-E0A9-E50E24DCCA9B';
  static const String rxUuid = '6E400002-B5A3-F393-E0A9-E50E24DCCA9B';
  static const String txUuid = '6E400003-B5A3-F393-E0A9-E50E24DCCA9B';
}
