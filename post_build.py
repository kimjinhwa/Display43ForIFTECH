import json
import os
import re
import shutil

# IIS 가상경로 http://ift.iptime.org:81/Esp32UploadFirmware/ 의 실제 물리 경로
UPLOAD_FIRMWARE_DIR = r"C:\inetpub\ftproot\Esp32UploadFirmware"
try:
    Import("env")
    _STANDALONE = False
except NameError:
    _STANDALONE = True

    class FakeEnv(dict):
        pass

    _ROOT = os.path.dirname(os.path.abspath(__file__))
    env = FakeEnv({
        "PROJECT_DIR": _ROOT,
        "PROJECT_BUILD_DIR": os.path.join(_ROOT, ".pio", "build"),
        "PIOENV": "IFTECH_DISP43",
    })


def get_version_from_h():
    version_h_path = os.path.join(env.get("PROJECT_DIR"), "Version.h")
    try:
        with open(version_h_path, "r", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if line.startswith("#define VERSION") or line.startswith("#define version"):
                    match = re.search(r'"([^"]+)"', line)
                    if match:
                        return match.group(1)
    except Exception as e:
        print(f"Error reading Version.h: {e}")
    return None


def parse_version(ver):
    """ '1.0.7' -> (1, 0, 7). 파싱 실패 시 None """
    if not ver:
        return None
    m = re.match(r"^(\d+)\.(\d+)\.(\d+)", str(ver).strip())
    if not m:
        return None
    return (int(m.group(1)), int(m.group(2)), int(m.group(3)))


def is_newer_version(current, server):
    """ server > current 이면 True """
    c = parse_version(current)
    s = parse_version(server)
    if c is None or s is None:
        return False
    return s > c


def sync_version_json(version_json_path, version, firmware_filename):
    """ env별 <env>.json 의 latest 가 Version.h 보다 낮으면 최신으로 맞춤 """
    data = {"latest": version, "filename": firmware_filename}

    if os.path.exists(version_json_path):
        try:
            with open(version_json_path, "r", encoding="utf-8") as f:
                existing = json.load(f)
            json_ver = existing.get("latest", "0.0.0")

            if not is_newer_version(json_ver, version):
                print(f"{os.path.basename(version_json_path)} 유지: {json_ver} (>= Version.h {version})")
                return

            print(f"{os.path.basename(version_json_path)} 갱신: {json_ver} -> {version}")
        except Exception as e:
            print(f"{os.path.basename(version_json_path)} 읽기 실패, 새로 작성: {e}")
    else:
        print(f"{os.path.basename(version_json_path)} 없음 → 생성: {version}")

    with open(version_json_path, "w", encoding="utf-8", newline="\n") as f:
        json.dump(data, f, indent=2)
        f.write("\n")
    print(
        f"{os.path.basename(version_json_path)} written: {version_json_path} "
        f"latest={data['latest']} filename={data['filename']}"
    )


def after_build(source, target, env):
    print("=== post_build.py: Build completed ===")
    firmware_path = env.get("PROJECT_BUILD_DIR")
    pioenv = env.get("PIOENV")
    source_directory = os.path.join(firmware_path, pioenv)
    print("source_directory: " + source_directory)

    version = get_version_from_h()
    if version is None:
        print("Version.h에서 VERSION을 찾을 수 없습니다. DGR 사용")
        version = "DGR"
    print("version: " + version)

    firmware_bin = os.path.join(source_directory, "firmware.bin")
    if not os.path.exists(firmware_bin):
        if source and len(source) > 0:
            firmware_bin = str(source[0])
        if not os.path.exists(firmware_bin):
            print("firmware.bin 없음: " + firmware_bin)
            return

    target_directory = UPLOAD_FIRMWARE_DIR
    if not os.path.exists(target_directory):
        os.makedirs(target_directory)
        print("업로드 폴더 생성: " + target_directory)

    legacy_directory = os.path.join(env.get("PROJECT_DIR"), "uploadFirmware")
    if not os.path.exists(legacy_directory):
        os.makedirs(legacy_directory)
        print("uploadFirmware 폴더 생성: " + legacy_directory)

    new_name = f"firmware_{version}_{pioenv}.bin"
    env_json_name = f"{pioenv}.json"

    # OTA용: IIS에는 앱 firmware + env JSON만 (공용 폴더라 bootloader/partitions 올리지 않음)
    ota_bins = [
        ("firmware.bin", new_name, firmware_bin),
    ]
    # 로컬 미러: 전체 플래시용 bootloader/partitions 포함
    local_bins = ota_bins + [
        (
            "bootloader.bin",
            f"bootloader_{version}_{pioenv}.bin",
            os.path.join(source_directory, "bootloader.bin"),
        ),
        (
            "partitions.bin",
            f"partitions_{version}_{pioenv}.bin",
            os.path.join(source_directory, "partitions.bin"),
        ),
    ]

    def copy_bins(dest_dir, bins):
        for plain_name, versioned_name, src_path in bins:
            if not os.path.exists(src_path):
                print(f"건너뜀 (없음): {src_path}")
                continue
            try:
                dest_plain = os.path.join(dest_dir, plain_name)
                dest_versioned = os.path.join(dest_dir, versioned_name)
                shutil.copy2(src_path, dest_plain)
                print(f"Copied: {dest_plain}")
                shutil.copy2(src_path, dest_versioned)
                print(f"Copied: {dest_versioned}")
            except Exception as copy_error:
                print(f"Error during copy ({dest_dir}/{plain_name}): {str(copy_error)}")

        if version != "DGR":
            version_json_path = os.path.join(dest_dir, env_json_name)
            sync_version_json(version_json_path, version, new_name)

    copy_bins(target_directory, ota_bins)
    copy_bins(legacy_directory, local_bins)

    print("=== post_build.py: 복사 완료 ===")


if not _STANDALONE:
    env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", after_build)
else:
    after_build(None, None, env)
