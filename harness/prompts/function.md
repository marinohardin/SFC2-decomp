You are rebuilding one function of Star Trek Starfleet Command II: Orion Pirates 2.5.5.2 as
C++ source that Visual C++ 6.0 SP4 compiles to the same machine code as the original. This is
attempt {{ATTEMPT}} on this function ({{MODEL}}). {{ROLE}}

## The function

- Original address: {{ADDRESS}}
- Name: {{NAME}}
- Mangled name: {{MANGLED}}
- Prototype (from analysis, may be rough): {{PROTOTYPE}}
- Size: {{SIZE}} bytes
- Original source file, if known: {{TRANSLATION_UNIT}}
- Score when this session started: {{BASELINE}}%
{{PAIR}}
## What to do

1. Find or create the translation unit for it under `{{SRC_ROOT}}/` (see
   `harness/SRC_LAYOUT.md`): the file named after its original source file when that is known,
   next to the functions of the same class otherwise.
2. Put the annotation on the line directly before the function definition:

       // FUNCTION: STARFLEETOP {{ADDRESS}}

3. Build and score it. This is the only judge of your work:

       harness/build-and-verify.sh {{ADDRESS}}

   It compiles the unit with the original compiler and flags, compares the function with the
   original bytes, and prints `OK`/`ERROR` lines, the differing instructions, and the path of a
   side-by-side listing (original on the left, your build on the right). Run it after every
   change. To see the original instructions before you have written anything, add the annotation
   to an empty function with the right signature and run it once.
4. Iterate until the score is 100% or you cannot improve it further, then stop. When you stop,
   the harness builds and scores the function again; it lets you finish only if the score went
   up. Stopping again after that gives up the attempt, which is fine: another attempt may follow.
5. If you stop short of 100%, end your last message with a section that starts with the line
   `HANDOFF:` and holds at most eight short lines: what you tried, which differences remain, and
   what you suspect causes them. The harness keeps it outside the repository and gives it to the
   next attempt on this function.
6. If you take the function to 100% from a lower starting score, end your last message with
   one line `RULE: <what the diff showed> -> <the source change that fixed it> (for
   {{ADDRESS}})`, on one line, in source terms and without assembly, such as `RULE: two
   branches came out swapped -> wrote the if and else the other way round (for {{ADDRESS}})`.
   The harness collects these lines.

## Deleting destructors

A scalar or vector deleting destructor (mangled `??_G` or `??_E`, about 30 bytes: call the
class's destructor, test the flag, call `operator delete`) is never written by hand. The
compiler generates it from the class's real destructor. In this code base it is claimed by
stacking its annotation on the real destructor's out-of-class definition, with the real
destructor's own annotation below it, like `tThreadEvent` (`Common/Thread/ThreadEvent.cpp`)
and `tWinTimer` (`Common/Timer/WinTimer.cpp`):

    // FUNCTION: STARFLEETOP 0x0070e4e0
    // FUNCTION: STARFLEETOP 0x0070e500
    tThreadEvent::~tThreadEvent()
    {
        ...the real destructor's body...
    }

So the work is the real destructor: write it from the original, with an honest body (a claim
never sits on an empty body or a placeholder), and the compiler emits the deleting destructor
from it. Do not write a `scalar_deleting_destructor` member, a body that calls `delete this`,
or any body for the deleting destructor itself; an older file that still has such a member form
is rewritten to the stacked form when the pair is done. A destructor written inline in the class
gets an out-of-class definition for this (in the unit, or `inline` after the class in its
header). If the pair above says a partner is already exact, keep its annotation and keep the
pair compiling. If the compiler does not emit the deleting destructor from your definition, say
so in your HANDOFF; do not fake it.

The context pack after the rules (Ghidra's pseudo-C, the callees, the class declarations and an
exact function nearby) and any notes from earlier attempts were gathered for you; start from
them rather than searching for the same things again.

## Rules

- Write the code the original programmers would have written: the class layouts, names and
  idioms of the surrounding source. Use names from the exe or the SDK headers
  (`{{SDK_HEADERS}}`); otherwise keep `fn_<address>` style names. Do not invent names.
- No inline assembly, no `#pragma` that changes code generation, no register variables or other
  tricks that only exist to force a match. If a construct is needed only to match and you
  believe it is not what the original said, mark it `// MATCH-ONLY: <why>`. Mark guesses about
  types or intent `// HYPOTHESIS: <what>`.
- Do not delete calls, change casts, or reorder code to raise the score at the cost of meaning.
  A wrong callee or global is reported as a relocation error even when the bytes look equal.
- Edit only source files under `{{SRC_ROOT}}/`. The harness, toolchain, original binary,
  analysis exports, matched set, queue and settings are off limits; attempts to change them are
  refused and end nothing but your progress.
- Do not run git commands that change anything; the loop commits a success for you.
- Keep other annotated functions in the same file as they are (the addresses of your pair, if
  you were given one, are yours to claim). Lowering any recorded score is a
  regression and fails the build.
