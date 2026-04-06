# CFile Learning Roadmap (Engineer Mode)

## 0) Freeze policy
- CDate work is frozen.
- Current focus is only Homework 3 (`CFile`).
- We implement in tiny steps and test each step before moving on.

## 1) Requirements distilled
- Simulated binary file in memory (no disk).
- Public API:
  - constructor, copy ctor, destructor, op=
  - `seek`, `read`, `write`, `truncate`, `fileSize`
  - `addVersion`, `undoVersion`
- `seek(pos)` valid range: `0..fileSize` inclusive.
- `read` reads up to requested bytes; advances position by bytes actually read.
- `write` overwrites/appends from current position; advances position.
- `truncate` cuts file to current position.
- Version snapshot stores both content and position.
- Copy must be independent (modifying one must not affect other).
- STL/string are disabled in evaluator.

## 2) Architecture phases

### Phase M (Mandatory-first, simple deep copy)
Goal: pass mandatory tests with clear code.

Data model (simple):
- Dynamic byte buffer: `uint8_t * m_Data`
- Size/capacity: `size_t m_Size`, `size_t m_Cap`
- File cursor: `size_t m_Pos`
- Versions stack (deep snapshots):
  - each snapshot stores own byte buffer + size + pos

This is easiest to reason about and debug.

### Phase O (Optional efficiency)
Goal: reduce memory for copies/versions using shared blocks + ref counting.

### Phase B (Bonus efficiency)
Goal: add copy-on-write with fine-grained sharing.

For now we start with Phase M only.

## 3) UML for Phase M

```text
+-------------------------------------------------+
| CFile                                           |
+-------------------------------------------------+
| - m_Data      : uint8_t*                        |
| - m_Size      : size_t                          |
| - m_Cap       : size_t                          |
| - m_Pos       : size_t                          |
| - m_Versions  : Snapshot* (dynamic array/stack) |
| - m_VCnt      : size_t                          |
| - m_VCap      : size_t                          |
+-------------------------------------------------+
| + CFile()                                       |
| + CFile(const CFile&)                           |
| + ~CFile()                                      |
| + operator=(const CFile&)                       |
| + seek(offset) : bool                           |
| + read(dst, bytes) : size_t                     |
| + write(src, bytes) : size_t                    |
| + truncate() : void                             |
| + fileSize() const : size_t                     |
| + addVersion() : void                           |
| + undoVersion() : bool                          |
+-------------------------------------------------+
| - ensureCap(newSize) : void                     |
| - clearVersions() : void                        |
| - copyFrom(other) : void                        |
+-------------------------------------------------+

Snapshot:
+--------------------------------+
| - data : uint8_t*              |
| - size : size_t                |
| - pos  : size_t                |
+--------------------------------+
```

## 4) Step-by-step implementation path

### Step A: Core file state + memory management
Implement:
- ctor, dtor, copy ctor, op= (deep copy)
- `fileSize`
- private `ensureCap`

Test:
- create/copy/destroy with no crash
- self-assignment safety

Commit:
- `step-a-core-memory-and-copy`

### Step B: seek/read/write (no versions yet)
Implement:
- `seek`, `read`, `write`

Test:
- overwrite in middle
- append beyond end by write from end
- read less than requested at EOF

Commit:
- `step-b-seek-read-write`

### Step C: truncate
Implement:
- `truncate`

Test:
- cut at middle
- cut to zero
- read after truncate

Commit:
- `step-c-truncate`

### Step D: versions (deep snapshots)
Implement:
- snapshot storage structure
- `addVersion`, `undoVersion`

Test:
- multiple add/undo
- position restore correctness
- undo when empty returns false

Commit:
- `step-d-versioning-deep`

### Step E: run provided sample
- Run full sample main from `sampleToFill.cpp`
- fix only correctness bugs (no optimization yet)

Commit:
- `step-e-pass-mandatory`

### Step F (later): optimize with ref-count/COW
- only after mandatory path is stable.

## 5) Debug loop for each step
- Build:
  - `g++ -std=c++20 -Wall -Wextra -pedantic -g testing.cpp -o testing`
- Run:
  - `./testing`
- Sanitizers:
  - `g++ -std=c++20 -g -fsanitize=address,undefined -fno-omit-frame-pointer testing.cpp -o testing-asan`
  - `./testing-asan`

## 6) Code review prep reminders
- Keep methods short, name by intent.
- Comment only non-trivial blocks.
- Be ready to explain copy behavior and ownership.
- Be ready to draw data structure and one undo scenario.
