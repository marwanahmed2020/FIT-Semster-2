# PA2 Code Review Playbook (Any Homework Can Be Picked)

## 1) What you must always be able to explain
- Problem constraints and assumptions.
- Chosen data model and why it fits.
- Core algorithm path (happy path + edge path).
- Complexity of key operations.
- Failure handling and input validation.

## 2) Standard structure for every homework
- `README.md` in homework folder:
  - Problem summary.
  - Design decisions.
  - Complexity table.
  - Known limits.
- `solve.cpp` (or required filename):
  - Clean API.
  - Private helpers for complex logic.
- `local-tests.cpp` (optional while developing):
  - Focused tests per feature.

## 3) Commit policy (learning + explainability)
- One concept per commit.
- Commit message template:
  - `feat(step-X): ...`
  - `why: ...`
  - `tested: ...`

Example:
- `feat(step-c): implement date arithmetic with serial day conversion`
- `why: keeps +/- and diff logic simple and consistent`
- `tested: sample asserts for +1500, -2000, and day diff`

## 4) Pre-submission checklist
- Compiles with strict warnings.
- No crash with malformed input.
- No stale debug prints.
- Deterministic behavior (no UB).
- Passes provided sample tests.

## 5) Code review rehearsal (5 minutes)
- 1 minute: restate problem in your own words.
- 2 minutes: draw class/data diagram on paper.
- 1 minute: walk through one non-trivial example.
- 1 minute: explain one bug you fixed and how tests caught it.

## 6) C++ quality points teachers look for
- Encapsulation over global helpers.
- Correct copy/destructor semantics when owning memory.
- Const-correct interfaces.
- Operator overload semantics consistent with STL style.
- Clear naming and minimal but meaningful comments.
