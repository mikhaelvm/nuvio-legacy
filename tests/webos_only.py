#!/usr/bin/env python3
"""Guard the webOS source/build boundary without requiring a TV SDK."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent
retired = re.compile(r'\b(?:NV_ANDROID|__ANDROID__|NV_TPK\w*|NV_VIDAA|NV_TIZEN3_LITE|__EMSCRIPTEN__|NV_TOUCH_UI|NV_SEM_WEBOS|NV_LINUX_DESKTOP)\b')
errors = []
for path in (ROOT/'src').rglob('*'):
    if 'vendor' in path.parts or path.suffix not in ('.c', '.h', '.inc'):
        continue
    source = path.read_text(encoding='utf8')
    for number, line in enumerate(source.splitlines(), 1):
        if re.match(r'\s*#\s*(?:if|ifdef|ifndef|elif)\b', line) and retired.search(line):
            errors.append(f'{path.relative_to(ROOT)}:{number}: removed build target')
        if re.match(r'\s*#\s*(?:if|elif)\b', line) and 'defined(0' in line:
            errors.append(f'{path.relative_to(ROOT)}:{number}: malformed directive')
    for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"', source, re.M):
        if include in ('backupcategories.h', 'nuvio_engine/nuvio_engine.h'):
            continue  # Optional local header / external P2P dependency.
        if not (path.parent/include).is_file() and not (ROOT/'src'/include).is_file():
            errors.append(f'{path.relative_to(ROOT)}: missing include {include}')

for relative in ('android/app', 'tizen-tpk', 'tizen-tpk-spike', 'tools/tpk', 'tools/android', 'tools/vidaa-instalar', 'deploy/app/tizen', 'servidor/tv'):
    path = ROOT/relative
    if path.exists() and any(p.is_file() for p in path.rglob('*')):
        errors.append(f'{relative}: removed port files remain')
env = (ROOT/'tools/env.sh').read_text(encoding='utf8')
assert 'tizen-config.xml' not in '\n'.join(line for line in env.splitlines() if not line.lstrip().startswith('#'))
assert '-DNV_WEBOS' in (ROOT/'tools/arm.sh').read_text(encoding='utf8')
if errors:
    raise SystemExit('\n'.join(errors))
print('PASS: webOS source boundary, local includes, build configuration and removed ports')
