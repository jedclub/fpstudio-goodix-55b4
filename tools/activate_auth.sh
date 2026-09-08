#!/usr/bin/env bash
set -euo pipefail
repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
# Desktop activation uses the session's native Polkit agent. Never open a
# Konsole window or collect the administrator password in this launcher.
exec /usr/bin/pkexec --disable-internal-agent /usr/bin/python "$repo_dir/tools/dual_auth_install.py" --apply
