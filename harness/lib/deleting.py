"""Which claimed addresses are compiler-generated deleting destructors.

A scalar or vector deleting destructor (`??_G` / `??_E`) has no source line of its own; its
annotation is stacked on its class's out-of-class destructor definition (harness/SRC_LAYOUT.md).
Which annotation on that definition is the deleting destructor is read from the evidence for its
address, never from the source. tools/manifest.py, the harness (harness/lib/verify.py) and the
object lift (tools/objlift/objlift.py) all decide it here, from the same evidence, so they cannot
disagree. This module imports nothing from the harness, so tools/manifest.py can use it alone.
"""
import re

# kind: (mangled-name prefix, the compiler's name for it)
KINDS = {'scalar': ('??_G', 'scalar deleting destructor'),
         'vector': ('??_E', 'vector deleting destructor')}


def kind(recorded=None, mangled='', name=''):
    """'scalar' or 'vector' when the evidence names a deleting destructor, else None.

    The evidence, in order, the first that names one deciding: the matched-set entry's symbol
    (`recorded`, a dict from analysis/results/matched_set.json, or None), functions.csv's
    mangled name, the matched-set entry's name, functions.csv's name. A symbol names one by its
    `??_G` / `??_E` prefix; a name by `scalar_deleting_destructor` / `vector_deleting_destructor`
    (with spaces or underscores, quoted or not)."""
    recorded = recorded if isinstance(recorded, dict) else {}
    for text in (recorded.get('symbol'), mangled, recorded.get('name'), name):
        text = text or ''
        for k, (prefix, words) in KINDS.items():
            if text.startswith(prefix) or re.search(words.replace(' ', '[ _]'), text):
                return k
    return None
