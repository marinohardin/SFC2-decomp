"""scripts/rebuild_scope.py: which changes may skip the matched-set rebuild."""
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'scripts'))

import rebuild_scope  # noqa: E402


def test_documentation_alone_skips_the_rebuild():
    assert rebuild_scope.classify([
        'README.md',
        'op/README.md',
        'harness/prompts/build.md',
        'docs/DESIGN.md',
        'docs/images/tree.svg',
        '.claude/settings.json',
        '.github/pull_request_template.md',
        '.github/CODEOWNERS',
        '.github/hotspots.txt',
        'LICENSE',
        'LICENSE.txt',
    ]) == 'skip'


def test_any_source_tool_or_result_change_keeps_the_rebuild():
    for path in (
        'op/Common/DynamicAllocator.cpp',
        'op/manifest.tsv',
        'harness/units.json',
        'harness/lib/verify.py',
        'toolchain/Dockerfile',
        'tools/manifest.py',
        'analysis/results/matched_set.json',
        'ghidra/exports/functions.csv',
        'scripts/check.sh',
        'tests/test_manifest.py',
        '.github/workflows/pull-request.yml',
        '.github/actions/setup/action.yml',
        '.githooks/pre-push',
        'pytest.ini',
        'op/LICENSE',
    ):
        assert rebuild_scope.classify(['README.md', path]) == 'rebuild', path


def test_no_change_keeps_the_rebuild():
    assert rebuild_scope.classify([]) == 'rebuild'
    assert rebuild_scope.classify(['']) == 'rebuild'
