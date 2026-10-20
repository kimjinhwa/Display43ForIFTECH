"""Version.h patch를 1 올리고 IFTECH_DISP43 만 빌드한 뒤 배포 폴더를 갱신한다.

일반 pio run / USB upload 에서는 버전을 올리지 않고, inetpub 과 uploadFirmware 도 건드리지 않는다.
앱 OTA 는 IFTECH_DISP43.json 만 받으므로 noscr 환경은 여기서 빌드하지 않는다.

  python versionup.py
"""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
ENV_NAME = "IFTECH_DISP43"


def bump_version_h():
    version_h_path = os.path.join(ROOT, "Version.h")
    with open(version_h_path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    changed = False
    old_ver = None
    new_ver = None
    new_lines = []
    for line in lines:
        m = re.match(
            r'^(\s*#define\s+VERSION\s+")(\d+)\.(\d+)\.(\d+)(".*)$',
            line.rstrip("\r\n"),
        )
        if m and not changed:
            major, minor, patch = int(m.group(2)), int(m.group(3)), int(m.group(4))
            old_ver = f"{major}.{minor}.{patch}"
            new_ver = f"{major}.{minor}.{patch + 1}"
            new_lines.append(f"{m.group(1)}{new_ver}{m.group(5)}\n")
            changed = True
        else:
            new_lines.append(line if line.endswith("\n") else line + "\n")

    if not changed:
        raise SystemExit("versionup: 활성 #define VERSION 을 찾지 못했습니다.")

    with open(version_h_path, "w", encoding="utf-8", newline="\n") as f:
        f.writelines(new_lines)
    return old_ver, new_ver


def main():
    old_ver, new_ver = bump_version_h()
    print(f"=== versionup: {old_ver} -> {new_ver} ({ENV_NAME}) ===")

    build = subprocess.run(f"pio run -e {ENV_NAME}", cwd=ROOT, shell=True)
    if build.returncode != 0:
        raise SystemExit(build.returncode)

    publish = subprocess.run([sys.executable, os.path.join(ROOT, "post_build.py")], cwd=ROOT)
    if publish.returncode != 0:
        raise SystemExit(publish.returncode)

    print("=== versionup: inetpub 과 uploadFirmware 갱신 완료 ===")


if __name__ == "__main__":
    main()
