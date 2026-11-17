#!/bin/bash
# tools/release.sh X.Y.Z [--ensaio] [--instalar]
set -euo pipefail
exec python3 "$(dirname "$0")/release.py" "$@"
