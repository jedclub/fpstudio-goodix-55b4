#!/usr/bin/env python3
"""Build the pinned Arch driver as the logged-in user, then install via polkit.

Never run makepkg as root. Build in a new cache directory, preserving the repo.
Package-manager operations request administrator authorization separately.
"""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

REPO = Path(__file__).resolve().parents[1]
DEPENDENCIES = ["git", "meson", "pkgconf", "gobject-introspection", "gtk-doc", "doctest",
                "glib2-devel", "libgusb", "openssl", "pixman", "nss", "libgudev", "opencv"]


def pinned_recipe(source):
    source = source.replace("git+https://github.com/TheWeirdDev/libfprint.git\"",
                            "git+https://github.com/TheWeirdDev/libfprint.git#commit=c1937b9\"")
    lines = [line for line in source.splitlines() if line.strip() not in
             ("git switch '55b4-experimental'", "git pull -f origin 55b4-experimental")]
    return "\n".join(lines) + "\n"


def main():
    if os.geteuid() == 0:
        raise SystemExit("Run the wizard as your ordinary user; makepkg must never run as root")
    if not shutil.which("pacman") or not shutil.which("makepkg"):
        raise SystemExit("This driver installer currently supports Arch/CachyOS only")
    missing = subprocess.run(["/usr/bin/pacman", "-T", *DEPENDENCIES], text=True, capture_output=True)
    if missing.returncode not in (0, 127):
        raise SystemExit(missing.stderr or "Dependency check failed")
    packages = missing.stdout.split()
    if packages:
        if any(package not in DEPENDENCIES for package in packages):
            raise SystemExit("Unexpected dependency output")
        subprocess.run(["/usr/bin/pkexec", "/usr/bin/pacman", "-S", "--needed", "--noconfirm", *packages], check=True)
    cache = Path(os.environ.get("XDG_CACHE_HOME", str(Path.home() / ".cache"))) / "fpstudio-build"
    cache.mkdir(mode=0o700, parents=True, exist_ok=True)
    build = Path(tempfile.mkdtemp(prefix="driver-", dir=cache))
    recipe = pinned_recipe((REPO / "src/driver/PKGBUILD").read_text())
    (build / "PKGBUILD").write_text(recipe)
    shutil.copytree(REPO / "src/driver/patches", build / "patches")
    print(f"Building pinned driver in {build}", flush=True)
    subprocess.run(["/usr/bin/makepkg", "--force", "--noconfirm"], cwd=build, check=True)
    packages = [p for p in build.glob("libfprint-goodixtls-55x4-fixed-*.pkg.tar.*") if "-debug-" not in p.name and not p.name.endswith(".sig")]
    if len(packages) != 1:
        raise SystemExit("Expected exactly one built driver package; refusing to guess")
    subprocess.run(["/usr/bin/pkexec", "/usr/bin/pacman", "-U", "--noconfirm", str(packages[0])], check=True)
    subprocess.run(["/usr/bin/pkexec", "/usr/bin/pacman", "-S", "--needed", "--noconfirm", "fprintd"], check=True)
    print("Driver and fprintd installed. Continue sensor communication and enrollment checks.")


if __name__ == "__main__":
    main()
