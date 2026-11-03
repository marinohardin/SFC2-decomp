#!/usr/bin/env python3
"""Instantiate an OP API mission-script template, as Taldren's CreateScript wizard would.

    new_from_template.py <Single|Multi|Blank> <ProjName> <outdir>

Copies _ScriptTemplates/<kind>/*.template to <outdir> (Blank = the top-level templates listed by
the top-level ProjectFile.dsp.template), renaming Script.* to <ProjName>.* and
ProjectFile.dsp to <ProjName>.dsp, and replaces ##ProjName##. Reads reference/, never writes it.
"""
import os, sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TEMPLATES = os.path.join(REPO, 'reference/installed game/Starfleet Command Orion Pirates',
                         'OP API - R2.1/Scripts/_ScriptTemplates')


def main():
    kind, name, out = sys.argv[1:4]
    src = TEMPLATES if kind == 'Blank' else os.path.join(TEMPLATES, kind)
    files = [f for f in os.listdir(src) if f.endswith('.template')]
    if kind == 'Blank':  # the top-level .dsp names only these
        files = [f for f in files if f.split('.')[0] in ('CommonDefs', 'MissionMaps', 'MissionText',
                                                         'Script', 'ProjectFile')]
    os.makedirs(out, exist_ok=True)
    for f in sorted(files):
        base = f[:-len('.template')]
        stem, ext = os.path.splitext(base)
        if stem in ('Script', 'ProjectFile'):
            base = name + ext
        text = open(os.path.join(src, f), 'rb').read().replace(b'##ProjName##', name.encode())
        open(os.path.join(out, base), 'wb').write(text)
    print(f'{len(files)} files -> {out}')


if __name__ == '__main__':
    main()
