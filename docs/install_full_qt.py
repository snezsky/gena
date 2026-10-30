"""Install all available Qt build modules, excluding debug-symbol packages."""

import os
import subprocess
import sys


def aqt(*args, capture=False):
    return subprocess.run(
        [sys.executable, "-m", "aqt", *args],
        check=True,
        text=True,
        stdout=subprocess.PIPE if capture else None,
    )


version = os.environ["QT_VERSION"]
target = sys.argv[1]
if target == "ifw":
    aqt("install-tool", "linux", "desktop", "tools_ifw", "qt.tools.ifw.47", "--outputdir", "/opt/Qt")
else:
    host, architecture = ("linux", "linux_gcc_64") if target == "desktop" else ("all_os", "android_arm64_v8a")
    available = aqt("list-qt", host, target, "--modules", version, architecture, capture=True)
    modules = [name for name in available.stdout.split() if "debug" not in name.lower()]
    print(f"Installing {target} modules: {' '.join(modules)}", flush=True)
    aqt("install-qt", host, target, version, architecture,
        "--outputdir", "/opt/Qt", "--modules", *modules)
