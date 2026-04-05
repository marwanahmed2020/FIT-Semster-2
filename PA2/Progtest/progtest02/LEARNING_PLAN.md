# CDate Learning Roadmap (Engineer Mode)

## 1) Problem requirements (from assignment)
- Build class CDate using Gregorian rules.
- Constructor `(y,m,d)` validates date, throws `InvalidDateException` if invalid.
- Arithmetic:
  - `date + int days`
  - `date - int days`
  - `date1 - date2` returns day difference.
- Increment/decrement:
  - prefix/postfix `++` and `--`.
- Comparisons:
  - `== != < <= > >=`
- Stream output `<<` default format `%Y-%m-%d`.
- Stream input `>>` default format `%Y-%m-%d`, failbit on error, object unchanged.
- Bonus: stream manipulator `date_format(...)` with per-stream format state.

## 2) UML (first design)

```text
+------------------------------------------------------+
| CDate                                                |
+------------------------------------------------------+
| - m_Year  : int                                      |
| - m_Month : int                                      |
| - m_Day   : int                                      |
+------------------------------------------------------+
| + CDate(y:int,m:int,d:int)                          |
| + operator+(days:int) const : CDate                 |
| + operator-(days:int) const : CDate                 |
| + operator-(rhs:CDate) const : int                  |
| + operator++() : CDate&                             |
| + operator++(int) : CDate                           |
| + operator--() : CDate&                             |
| + operator--(int) : CDate                           |
| + operator<=>(rhs:CDate) const : std::strong_ordering|
| + operator==(rhs:CDate) const : bool                |
+------------------------------------------------------+
| - static isLeap(y:int) : bool                        |
| - static daysInMonth(y:int,m:int) : int             |
| - static valid(y:int,m:int,d:int) : bool            |
| - static toSerial(y:int,m:int,d:int) : int          |
| - static fromSerial(n:int) : CDate                  |
+------------------------------------------------------+

+----------------------------------+
| InvalidDateException             |
+----------------------------------+
| (inherits std::invalid_argument) |
+----------------------------------+
```

## 3) Implementation strategy (small verified steps)

### Step A: Validation core only
Goal:
- Implement `isLeap`, `daysInMonth`, `valid`, constructor.

Tests:
- valid/invalid dates.
- leap-year checks (2000 true, 2001 false, 2004 true).

Commit:
- `step-a-validation-core`

### Step B: Day serial conversion
Goal:
- Implement `toSerial` and `fromSerial` only.

Tests:
- round-trip: date -> serial -> date must match.
- neighboring days across month/year boundaries.

Commit:
- `step-b-serial-conversion`

### Step C: Date arithmetic and comparison
Goal:
- Implement `+`, `-`, date difference, comparisons.

Tests:
- sample arithmetic from test file:
  - `2000-01-02 + 1500 == 2004-02-10`
  - `2010-02-03 - 2000 == 2004-08-13`
  - difference `185`

Commit:
- `step-c-arithmetic-comparison`

### Step D: ++/-- operators
Goal:
- prefix/postfix semantics exactly.

Tests:
- all four cases in sample asserts.

Commit:
- `step-d-increment-decrement`

### Step E: Stream output/input ISO
Goal:
- `<<` writes `%Y-%m-%d`
- `>>` reads exactly ISO and preserves old value on failure.

Tests:
- success/failure cases from sample mandatory part.

Commit:
- `step-e-stream-iso`

### Step F (bonus): date_format manipulator
Goal:
- implement stream-local format state for both input and output.

Tests:
- bonus section in sample test.

Commit:
- `step-f-date-format-bonus`

## 4) Suggested file structure
- `t.cpp`: your solve file (single-file submission style).
- `sample(3)/test.cpp`: reference tests.
- `LOCAL_TESTS.md`: optional checklist per step.

## 5) Debug workflow each step
- Build with warnings:
  - `g++ -std=c++20 -Wall -Wextra -pedantic -g t.cpp -o t`
- Run:
  - `./t`
- Memory/runtime checks (optional):
  - `g++ -std=c++20 -g -fsanitize=address,undefined -fno-omit-frame-pointer t.cpp -o t-asan`
  - `./t-asan`

## 6) Engineering habits
- One step = one focused commit.
- Keep a tiny test for the current step before adding next feature.
- Never move to next step until current step passes.
- If broken, `git diff` + isolate one failing assert.

## 7) Code-review readiness (important for all homeworks)

Your instructor may pick any submitted homework in the oral review. So solve every homework with the same discipline below.

### A) Explainability rule
- For each function, be able to explain:
  - input/output contract,
  - invariants before and after,
  - complexity,
  - one edge case.

### B) Design checklist (before coding)
- Name classes/functions by intent, not by type (`addDays`, `toSerial`, `isValidDate`).
- Keep data private and logic inside class methods.
- Do not expose unnecessary getters/setters.
- Add comments only for non-obvious logic blocks.

### C) Review checklist (before commit)
- Build flags clean: `-Wall -Wextra -pedantic`.
- No sanitizer issues with ASan/UBSan.
- Invalid input paths tested (constructor/input operator).
- Self-assignment path tested if operator= exists.
- Copy behavior tested if class owns resources (Rule of 3/5/0 awareness).

### D) Oral defense prep per commit
After each commit, write 3 short bullets in commit message or notes:
- What changed.
- Why this design.
- What test proves it.

### E) "Any homework can be picked" routine
For each solved homework, keep a short note with:
- key data structure choice,
- one rejected alternative and why,
- hardest bug and fix,
- complexity summary of critical operations.
