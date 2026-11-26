"""toolchain/calibrate.sh pins one objdiff-cli release, with a checksum for each platform it
supports. Read from the script's text; nothing is downloaded."""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]


def test_each_platform_names_its_asset_and_a_sha256():
    text = (ROOT / 'toolchain' / 'calibrate.sh').read_text()
    assert 'OBJDIFF_VERSION=v3.8.3' in text
    cases = re.findall(r'^\s*(\S+-\S+)\) asset=(\S+)\n\s*sum=([0-9a-f]+) ;;', text, re.M)
    assert cases == [
        ('Darwin-arm64', 'objdiff-cli-macos-arm64',
         '27dbd79196394c246dde77ef0a6ac855ff6c568069171dd7db0093f39c03ee8d'),
        ('Linux-x86_64', 'objdiff-cli-linux-x86_64',
         'e823ae932a56c68dcd86b68d12f2268c953e0c027402162c8a6dfa6a00d65008'),
    ]
