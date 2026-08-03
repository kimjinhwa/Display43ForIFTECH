/// Wi-Fi presets for OTA — tap to fill SSID/PASS (no typing).
class WifiPreset {
  const WifiPreset({
    required this.id,
    required this.label,
    required this.ssid,
    required this.password,
    this.isOpen = false,
  });

  final String id;
  final String label;
  final String ssid;

  /// empty + [isOpen] → pass none
  final String password;
  final bool isOpen;

  String get passHint => isOpen ? '공개 (오픈 AP)' : password;
}

const kWifiPresetCustomId = 'custom';

const kWifiPresets = <WifiPreset>[
  WifiPreset(
    id: 'iftech',
    label: 'iftech',
    ssid: 'iftech',
    password: 'iftech0273',
  ),
  WifiPreset(
    id: 'iptime',
    label: 'iptime_mbhong',
    ssid: 'iptime_mbhong',
    password: '',
    isOpen: true,
  ),
  WifiPreset(
    id: 'hotspot',
    label: 'AndroidHotspot',
    ssid: 'AndroidHotspot1953',
    password: '87654321',
  ),
];

WifiPreset? matchWifiPreset(String? ssid, {bool? passOpen, String? pass}) {
  if (ssid == null || ssid.isEmpty) return null;
  for (final p in kWifiPresets) {
    if (p.ssid != ssid) continue;
    if (p.isOpen) {
      if (passOpen == true || (pass != null && pass.isEmpty)) return p;
    }
    if (pass == null || pass == p.password) return p;
  }
  return null;
}
