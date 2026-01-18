#!/usr/bin/env python3

import os
import sys
import json
import subprocess
import shlex
import stat
from pathlib import Path


# Keep this function in sync with root_on_install.py
def get_exe_path() -> Path:
    meson_introspect = os.getenv("MESONINTROSPECT")

    installed_files_raw = subprocess.run(
        shlex.split(meson_introspect) + ["--installed"],
        stdout=subprocess.PIPE,
        check=True,
        text=True,
    )

    installed_files = json.loads(installed_files_raw.stdout)

    if len(installed_files) == 0:
        sys.exit(
            "Meson introspect sees no installed files! If you have modified "
            "meson.build file, please fix it."
        )
    if len(installed_files) > 1:
        sys.exit(
            "Meson introspect sees more than one installed file! Please update "
            "setuid_root_on_install.py to handle this."
        )

    base_path = Path(list(installed_files.values())[0])

    if "DESTDIR" in os.environ:
        return Path(os.getenv("DESTDIR")) / base_path.relative_to("/")
    return base_path


if __name__ == "__main__":
    exe_path = get_exe_path()

    dry_run = "MESON_INSTALL_DRY_RUN" in os.environ

    if "MESON_INSTALL_QUIET" not in os.environ:
        base = f"Setting the set-user-ID (SETUID) bit for installed executable '{exe_path}' by chmoding to -rwsr-xr-x (4755)"
        if dry_run:
            base += " [dry run]..."
        else:
            base += "..."
        print(base)

    if dry_run:
        sys.exit()

    os.chmod(
        exe_path,
        stat.S_ISUID
        | stat.S_IRUSR
        | stat.S_IWUSR
        | stat.S_IXUSR
        | stat.S_IRGRP
        | stat.S_IXGRP
        | stat.S_IROTH
        | stat.S_IXOTH,
    )
