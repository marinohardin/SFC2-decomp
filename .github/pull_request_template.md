<!-- Title: a Conventional Commits subject. It becomes the commit message when this is squashed. -->

**Batch or work package:** <!-- its ID, or "none" -->

## What changed

## Why

## How it was tested

## Evidence

<!-- For a change to op/ or the harness: the `RESULT` line of `sh harness/build-and-verify.sh`
     and the exact count before and after. For a progress chart: the image. -->

## From a fork

<!-- Outside contributors only (CONTRIBUTING.md); delete this section otherwise.
     1. The claim issue, on a line of its own: Claim: #<number>
     2. The RESULT line of `sh harness/build-and-verify.sh <unit>` for your units.
     3. The output of `python3 tools/evidence.py --log <saved harness output>`. -->

Claim: #

<!--
Delete what does not apply:

- Adds a dependency: say why it is needed.
- Adds a cost (a service, a scheduled job, a per-request external call): state it.
- Changes the build interface (docs/INTERFACE.md): INTERFACE_VERSION is raised.
- Changes behavior in DESIGN.md or a rule in CONVENTIONS.md: the document is updated in this pull request.
-->
