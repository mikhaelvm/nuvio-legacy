#!/usr/bin/env python3
"""The fork's package, runtime, updates and release must use one identity."""
import importlib.util
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent
app = json.loads((ROOT/'deploy/app/appinfo.json').read_text(encoding='utf8'))
header = (ROOT/'src/app_id.h').read_text(encoding='utf8')
runtime_id = re.search(r'^#define NV_APP_ID_PRODUCTION "([^"]+)"', header, re.M)[1]
assert app['title'] == 'Nuvio'
assert app['id'] == runtime_id == 'com.mikhael.nuvio'
assert '#define NV_LS_MEDIA_CLIENT "com.webos.media.client." NV_APP_ID' in header
assert app['id'] != 'space.nuvio.native.legacy'

spec = importlib.util.spec_from_file_location('release', ROOT/'tools/release.py')
release = importlib.util.module_from_spec(spec)
spec.loader.exec_module(release)
assert release.expected_files(app['version']) == [
    f"{app['id']}_{app['version']}_arm.ipk",
    f"{app['id']}_{app['version']}_arm-highcache.ipk",
]
updates = (ROOT/'src/atualizacao.c').read_text(encoding='utf8')
assert 'void atualizacao_verificar(void) {}' in updates
assert 'api.github.com' not in updates
data = (ROOT/'src/dados.c').read_text(encoding='utf8')
assert '"/media/developer/temp/" NV_APP_ID' in data
assert '"%s/.%s", home, NV_APP_ID' in data
logs = (ROOT/'src/registro.c').read_text(encoding='utf8')
assert '"/tmp/" NV_APP_ID ".log"' in logs
previous = (ROOT/'src/avisos.c').read_text(encoding='utf8')
assert '#define AV_LOG_ANTERIOR "/tmp/" NV_APP_ID "-anterior.log"' in previous
for package in json.loads((ROOT/'repo.json').read_text(encoding='utf8'))['packages']:
    assert package['id'] == app['id']
    assert package['manifest']['id'] == app['id']
print('PASS: independent package ID, Nuvio title, namespaced data/media client and fork updates')
