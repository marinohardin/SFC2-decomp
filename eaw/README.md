# eaw/: Empires at War

Not started. Nothing in this repository reads the Empires at War executable yet, and no tool
treats this folder as a source root.

Orion Pirates' project is a fork of Empires at War's. Most of the engine (`op/Tiger/`, about four
fifths identical, and `op/dmp3d/`) and about half of the game proper are the same code in both
executables. When this module starts, its first piece of work is the tooling for two modules:

- a second source root, with its own `UNITS.tsv`, manifest and matched set;
- the engine code both games share moved to a tree of its own, each function there annotated once
  per executable, at that executable's address;
- the progress report run per module (`analysis/results/progress/eaw/` holds its badges, which
  read "not started" until then).

Until then a change here is a README change only.
