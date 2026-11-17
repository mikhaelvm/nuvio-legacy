#!/usr/bin/env python3
"""Offline regression tests for webOS release integrity and failure handling."""
import importlib.util
import io
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location('release', ROOT/'tools/release.py')
r = importlib.util.module_from_spec(spec)
spec.loader.exec_module(r)


class ReleaseTests(unittest.TestCase):
    def test_checksums_detect_changes(self):
        with tempfile.TemporaryDirectory() as d:
            out = Path(d)
            (out/'app.ipk').write_bytes(b'package')
            sums = out/'SHA256SUMS'
            sums.write_text(f'{r.digest(out/"app.ipk")}  app.ipk\n')
            r.verify_sums(sums)
            (out/'app.ipk').write_bytes(b'changed')
            with self.assertRaises(RuntimeError):
                r.verify_sums(sums)

    def test_tree_must_be_clean_and_stable(self):
        with patch.object(r, 'tree_state', return_value=(' M src/main.c', 'abc')), patch.dict(os.environ, {}, clear=True):
            with self.assertRaises(RuntimeError):
                r.check_tree()
            os.environ['NUVIO_RELEASE_TREE'] = 'abc'
            r.check_tree()
            os.environ['NUVIO_RELEASE_TREE'] = 'changed'
            with self.assertRaises(RuntimeError):
                r.check_tree()

    @staticmethod
    def payload(version, binary):
        buffer = io.BytesIO()
        with tarfile.open(fileobj=buffer, mode='w:gz') as archive:
            for name, data in [('usr/app/appinfo.json', json.dumps({'version': version}).encode()),
                               ('usr/app/nuvio-proto', binary)]:
                member = tarfile.TarInfo(name)
                member.size = len(data)
                archive.addfile(member, io.BytesIO(data))
        return buffer.getvalue()

    def test_both_packages_require_version_and_config(self):
        with tempfile.TemporaryDirectory() as d:
            out = Path(d)
            for name in r.expected_files('2.0.8'):
                self.assertTrue(name.endswith('.ipk'))
                (out/name).write_bytes(b'package')
            self.assertEqual(len(r.expected_files('2.0.8')), 2)
            for version, binary, valid in [('2.0.8', b'config-marker', True),
                                            ('2.0.7', b'config-marker', False),
                                            ('2.0.8', b'missing', False)]:
                with patch.object(r.subprocess, 'check_output', return_value=self.payload(version, binary)):
                    if valid:
                        self.assertEqual(len(r.verify_packages(out, '2.0.8', {'NV_SERVICE': 'config-marker'})), 2)
                    else:
                        with self.assertRaises(RuntimeError):
                            r.verify_packages(out, '2.0.8', {'NV_SERVICE': 'config-marker'})
            (out/r.expected_files('2.0.8')[1]).unlink()
            with patch.object(r.subprocess, 'check_output', return_value=self.payload('2.0.8', b'config-marker')):
                with self.assertRaises(RuntimeError):
                    r.verify_packages(out, '2.0.8', {'NV_SERVICE': 'config-marker'})

    def test_build_failure_invalidates_manifest_and_releases_lock(self):
        previous = Path.cwd()
        try:
            with tempfile.TemporaryDirectory() as d:
                root = Path(d)
                (root/'build').mkdir()
                out = root/'build/release-2.0.8'
                out.mkdir()
                (out/'SHA256SUMS').write_text('stale')
                with patch.object(r, 'ROOT', root), patch.object(r, 'tree_state', return_value=('', 'abc')), \
                     patch.object(r, 'config', return_value={'NV_VERSAO': '2.0.8'}), \
                     patch.object(r.subprocess, 'Popen', side_effect=OSError('SDK unavailable')), \
                     patch.object(sys, 'argv', ['release.py', '2.0.8']), patch.dict(os.environ):
                    with self.assertRaises(OSError):
                        r.main()
                self.assertFalse((out/'SHA256SUMS').exists())
                self.assertFalse((root/'build/.release-lock').exists())
                self.assertEqual(json.loads((out/'logs/timings.json').read_text())['total']['rc'], 1)
                os.chdir(previous)
        finally:
            os.chdir(previous)


if __name__ == '__main__':
    unittest.main()
