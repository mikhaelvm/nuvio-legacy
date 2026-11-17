#!/usr/bin/env python3
"""Release local webOS: dois pacotes IPK, sem publish/deploy implicito."""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import tempfile
import time

ROOT = Path(__file__).resolve().parent.parent


def command(*args):
    return subprocess.check_output(args, text=True).strip()


def digest(path):
    with open(path, 'rb') as f:
        return hashlib.file_digest(f, 'sha256').hexdigest() if hasattr(hashlib, 'file_digest') else hashlib.sha256(f.read()).hexdigest()


def verify_sums(manifest):
    for line in manifest.read_text().splitlines():
        expected, name = line.split(None, 1)
        if digest(manifest.parent / name) != expected:
            raise RuntimeError(f'checksum divergiu: {name}')


def tree_state():
    status = command('git', 'status', '--porcelain', '--untracked-files=all')
    h = hashlib.sha256(subprocess.check_output(['git', 'diff', 'HEAD', '--binary']))
    h.update(command('git', 'rev-parse', 'HEAD').encode())
    h.update(status.encode())
    for name in subprocess.check_output(['git', 'ls-files', '--others', '--exclude-standard', '-z']).decode().split('\0'):
        if name:
            p = Path(name)
            h.update(name.encode())
            h.update(os.readlink(p).encode() if p.is_symlink() else p.read_bytes())
    return status, h.hexdigest()


def check_tree():
    status, fingerprint = tree_state()
    expected = os.environ.get('NUVIO_RELEASE_TREE')
    if expected:
        if fingerprint != expected:
            raise RuntimeError('arvore mudou durante o build')
    elif status:
        raise RuntimeError('arvore suja; release exige commit limpo (use --ensaio apenas para medir alteracoes locais)')
    return status, fingerprint


def config():
    # env.sh valida ANTES de criar o arquivo; nunca imprimir valores.
    with tempfile.TemporaryDirectory(prefix='nuvio-release-env-') as tmp:
        path = Path(tmp) / 'env'
        env = dict(os.environ, NUVIO_REQUIRE_ALL='1')
        subprocess.run(['bash', 'tools/env.sh', '--env-file', str(path)], env=env, check=True)
        return dict(line.split('=', 1) for line in path.read_text().splitlines())


def expected_files(version):
    app_id = json.loads((ROOT / 'deploy/app/appinfo.json').read_text(encoding='utf-8'))['id']
    return [f'{app_id}_{version}_arm{suffix}.ipk' for suffix in ('', '-highcache')]


def verify_packages(out, version, values):
    report = {}
    def check_binary(data, label):
        missing = [k for k, v in values.items() if v.encode() not in data]
        if missing:
            raise RuntimeError(f'{label}: configuracao ausente: {", ".join(missing)}')
        return {'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(), 'config': f'{len(values)}/{len(values)}'}
    for name in expected_files(version):
        path = out / name
        if not path.is_file() or not path.stat().st_size:
            raise RuntimeError(f'faltou {name}')
        binaries = {}
        payload = subprocess.check_output(['ar', 'p', str(path), 'data.tar.gz'])
        with tarfile.open(fileobj=io.BytesIO(payload), mode='r:gz') as tf:
            app = next(m for m in tf.getmembers() if m.name.endswith('/appinfo.json'))
            if json.load(tf.extractfile(app))['version'] != version:
                raise RuntimeError(f'{name}: versao do appinfo diverge')
            exe = next(m for m in tf.getmembers() if m.name.endswith('/nuvio-proto'))
            binaries['nuvio-proto'] = check_binary(tf.extractfile(exe).read(), name)
        report[name] = {'bytes': path.stat().st_size, 'sha256': digest(path), 'binaries': binaries}
    return report


def main():
    os.chdir(ROOT)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('version', nargs='?')
    parser.add_argument('--ensaio', action='store_true', help='medir WIP sem commit; registra arvore suja e exige conteudo inalterado')
    parser.add_argument('--instalar', action='store_true', help='depois da conferencia, instalar variante LG normal via arm.sh')
    parser.add_argument('--check-tree', action='store_true', help=argparse.SUPPRESS)
    args = parser.parse_args()
    if args.check_tree:
        check_tree()
        return
    if not args.version or not re.fullmatch(r'[0-9]+\.[0-9]+\.[0-9]+', args.version):
        parser.error('versao obrigatoria: X.Y.Z')
    # Nao herdar a dispensa de uma execucao anterior.
    os.environ.pop('NUVIO_RELEASE_TREE', None)
    status, fingerprint = tree_state()
    if not args.ensaio:
        check_tree()
    if args.ensaio:
        print('[preflight] ENSAIO local: arvore suja permitida, conteudo sera conferido no final.', flush=True)
    if args.ensaio and args.instalar:
        parser.error('--ensaio nao instala')
    values = config()
    if values['NV_VERSAO'] != args.version:
        raise RuntimeError(f'versao solicitada {args.version} != fontes {values["NV_VERSAO"]}')
    out = ROOT / f'build/release-{args.version}'
    out.mkdir(parents=True, exist_ok=True)
    lock = ROOT / 'build/.release-lock'
    try:
        lock.mkdir()
    except FileExistsError:
        raise RuntimeError('outro build usa este worktree (build/.release-lock); nao rode dois ao mesmo tempo')
    started = time.monotonic()
    logs = out / 'logs'
    logs.mkdir(exist_ok=True)
    timings = {}
    env = dict(os.environ, NUVIO_REQUIRE_ALL='1', NUVIO_RELEASE_TREE=fingerprint,
               NUVIO_RELEASE_OUT=str(out))
    def step(name, cmd):
        begin = time.monotonic()
        with (logs / f'{name}.log').open('w') as log:
            proc = subprocess.Popen(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
            for line in proc.stdout:
                line = f'[{name} +{time.monotonic()-begin:.1f}s] {line}'
                log.write(line)
                log.flush()
                print(line, end='', flush=True)
            rc = proc.wait()
        timings[name] = {'seconds': round(time.monotonic()-begin, 2), 'rc': rc}
        print(f'[{name}] fim: {timings[name]}', flush=True)
        if rc:
            raise RuntimeError(f'{name} falhou (rc={rc}); veja {logs / (name + ".log")}')
    def lg():
        step('lg', ['bash', 'tools/arm.sh', '--ipk', '--build'])
        step('lg-highcache', ['bash', 'tools/arm.sh', '--high-cache', '--ipk', '--build'])
    rc = 1
    try:
        # Um manifesto anterior nao pode atestar uma rodada que falhou.
        for name in ['SHA256SUMS', 'repo.json', 'webosbrew.manifest.json', 'verification.json']:
            (out / name).unlink(missing_ok=True)
        lg()
        if tree_state()[1] != fingerprint:
            raise RuntimeError('arvore mudou durante o build')
        if config() != values:
            raise RuntimeError('configuracao mudou durante o build')
        report = verify_packages(out, args.version, values)
        subprocess.run(['bash', 'tools/hb-repo.sh', str(out / expected_files(args.version)[0]), str(out)], check=True)
        files = expected_files(args.version) + ['repo.json', 'webosbrew.manifest.json']
        (out / 'SHA256SUMS').write_text(''.join(f'{digest(out / name)}  {name}\n' for name in sorted(files)))
        verify_sums(out / 'SHA256SUMS')
        if tree_state()[1] != fingerprint:
            (out / 'SHA256SUMS').unlink()
            raise RuntimeError('arvore mudou durante a conferencia')
        (out / 'verification.json').write_text(json.dumps({'version': args.version, 'commit': command('git', 'rev-parse', 'HEAD'),
            'tree': fingerprint, 'dirty': bool(status), 'ensaio': args.ensaio, 'packages': report}, indent=2)+'\n')
        rc = 0
        print(f'Conferidos 2 pacotes IPK + 2 JSON: {out}. Sem publicar.', flush=True)
    finally:
        timings['total'] = {'seconds': round(time.monotonic()-started, 2), 'rc': rc}
        (logs / 'timings.json').write_text(json.dumps(timings, indent=2)+'\n')
        lock.rmdir()
    if args.instalar:
        step('instalar-lg', ['bash', 'tools/arm.sh'])


if __name__ == '__main__':
    try:
        main()
    except (RuntimeError, subprocess.CalledProcessError) as error:
        sys.exit(f'release: {error}')
