"""scripts/port_footprint.py lists SFCOP_FIX sites in nested conditionals, beside F1's fixture.

The fix sites below are written the way src/fixes/fixes.h and CONVENTIONS §4.1 ask: `#if
SFCOP_FIX(name)` around the smallest region with the original in `#else`, nested inside other
conditionals and inside each other. Switch declarations in the src/fixes/switches.def form
(`SFCOP_FIX_BUILD_...`, `SFCOP_FIX_name()`, `SFCOP_FIX_SWITCH(...)`) are not sites.
"""
import os

from test_port_footprint import make_tree, run, write


def add_fix_sites(base):
    write(os.path.join(base, 'game', 'Source', 'Weapons', 'tPhaser.cpp'),
          '// FUNCTION: STARFLEETOP 0x0045a120\n'                       # 1
          'int tPhaser::Fire(int arc) {\n'                              # 2
          '#ifdef _DEBUG\n'                                             # 3
          '    Trace(arc);\n'                                           # 4
          '#else\n'                                                     # 5
          '#if SFCOP_FIX(fix_phaser_arc_bounds)\n'                      # 6
          '    if (arc < 0 || arc > 5) return 0;\n'                     # 7
          '#endif\n'                                                    # 8
          '#endif\n'                                                    # 9
          '#if SFCOP_FIX(fix_phaser_arc_bounds)\n'                      # 10
          '#if !SFCOP_FIX(fix_phaser_overload)\n'                       # 11
          '    mCharge = 0;\n'                                          # 12
          '#else\n'                                                     # 13
          '    mCharge = mCharge / 2; // PORT: undefined behavior the original relied on\n'  # 14
          '#endif\n'                                                    # 15
          '#elif SFCOP_FIX(fix_phaser_overload)\n'                      # 16
          '    mCharge = 1;\n'                                          # 17
          '#else\n'                                                     # 18
          '    mCharge = 0;\n'                                          # 19
          '#endif\n'                                                    # 20
          '    return arc;\n'                                           # 21
          '}\n')                                                        # 22
    # A switch declaration in the switches.def form, in a header the counter reads.
    write(os.path.join(base, 'game', 'port', 'fixes_decl.h'),
          '#ifndef SFCOP_FIX_BUILD_fix_phaser_arc_bounds\n'
          '#define SFCOP_FIX_BUILD_fix_phaser_arc_bounds 1\n'
          '#endif\n'
          '#define SFCOP_FIX_fix_phaser_arc_bounds() '
          'SFCOP_FIX_POSITION(SFCOP_FIX_BUILD_fix_phaser_arc_bounds)\n'
          'SFCOP_FIX_SWITCH(fix_phaser_arc_bounds, Build, 1, Project, "arc bounds")\n')


def test_nested_fix_sites_are_listed_with_file_and_line_and_declarations_are_not(tmp_path):
    make_tree(str(tmp_path))
    add_fix_sites(str(tmp_path))
    p = run('game', str(tmp_path))
    assert p.returncode == 0, p.stderr
    assert p.stdout.splitlines() == [
        'PORT game/Common/Allocator.cpp:3: long size = n; '
        '// PORT: 64-bit, the original stores a pointer in a long',
        'PORT game/Source/Ship.h:2: int mShields; '
        '/* PORT: undefined behaviour the original relied on */',
        'PORT game/Source/Weapons/tPhaser.cpp:14: mCharge = mCharge / 2; '
        '// PORT: undefined behavior the original relied on',
        'SFCOP_FIX game/Common/Allocator.cpp:4: #if SFCOP_FIX(fix_shield_overflow)',
        'SFCOP_FIX game/Source/Ship.h:4: #if SFCOP_FIX(fix_a) || SFCOP_FIX(fix_b)',
        'SFCOP_FIX game/Source/Ship.h:4: #if SFCOP_FIX(fix_a) || SFCOP_FIX(fix_b)',
        'SFCOP_FIX game/Source/Weapons/tPhaser.cpp:6: #if SFCOP_FIX(fix_phaser_arc_bounds)',
        'SFCOP_FIX game/Source/Weapons/tPhaser.cpp:10: #if SFCOP_FIX(fix_phaser_arc_bounds)',
        'SFCOP_FIX game/Source/Weapons/tPhaser.cpp:11: #if !SFCOP_FIX(fix_phaser_overload)',
        'SFCOP_FIX game/Source/Weapons/tPhaser.cpp:16: #elif SFCOP_FIX(fix_phaser_overload)',
        'PORT lines: 3',
        'SFCOP_FIX sites: 7',
    ]


def test_switch_declarations_alone_are_not_fix_sites(tmp_path):
    add_fix_sites(str(tmp_path))
    os.remove(os.path.join(str(tmp_path), 'game', 'Source', 'Weapons', 'tPhaser.cpp'))
    p = run('game', str(tmp_path))
    assert p.returncode == 0, p.stderr
    assert p.stdout.splitlines() == ['PORT lines: 0', 'SFCOP_FIX sites: 0']


def test_no_fix_sites_flag_fails_a_tree_with_a_fix_site_after_listing_it(tmp_path):
    make_tree(str(tmp_path))
    add_fix_sites(str(tmp_path))
    p = run('game', str(tmp_path), '--no-fix-sites')
    assert p.returncode == 1
    assert p.stdout.splitlines()[-2:] == ['PORT lines: 3', 'SFCOP_FIX sites: 7']
    assert p.stderr.splitlines() == [
        'port_footprint: 7 SFCOP_FIX site(s) under game; a fix to game code is an override unit '
        'under src/fixes/overrides/ (DESIGN §6.9)',
    ]


def test_no_fix_sites_flag_passes_a_tree_with_port_lines_only(tmp_path):
    write(os.path.join(str(tmp_path), 'game', 'Tiger', 'tpart.cpp'),
          '// FUNCTION: STARFLEETOP 0x00628a60\n'
          'void tPart::mValidateParts() {\n'
          '    long p = (long)mNext; // PORT: 64-bit, the original stores a pointer in a long\n'
          '}\n')
    p = run('game', str(tmp_path), '--no-fix-sites')
    assert p.returncode == 0, p.stderr
    assert p.stdout.splitlines() == [
        'PORT game/Tiger/tpart.cpp:3: long p = (long)mNext; '
        '// PORT: 64-bit, the original stores a pointer in a long',
        'PORT lines: 1',
        'SFCOP_FIX sites: 0',
    ]
    assert p.stderr == ''
