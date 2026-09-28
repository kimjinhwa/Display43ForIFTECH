/// ESP32 Nordic UART Service (NUS) UUIDs — matches myBlueTooth.h
class BleConstants {
  /// Advertised as IFT_… (e.g. IFT_43_ + MAC)
  static const String deviceNamePrefix = 'IFT_';

  static const String serviceUuid = '6E400001-B5A3-F393-E0A9-E50E24DCCA9B';
  static const String rxUuid = '6E400002-B5A3-F393-E0A9-E50E24DCCA9B';
  static const String txUuid = '6E400003-B5A3-F393-E0A9-E50E24DCCA9B';

  /// Same as firmware `-D FW_UPDATE_BASE` / `FW_UPDATE_META`.
  /// Used when the device is too old to have the `fw` CLI.
  static const String fwUpdateBase =
      'http://ift.iptime.org:81/Esp32UploadFirmware';
  static const String fwUpdateMeta = 'IFTECH_DISP43.json';
}
