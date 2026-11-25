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
- Keep other annotated functions in the same file as they are. Lowering any recorded score is a
  regression and fails the build.
