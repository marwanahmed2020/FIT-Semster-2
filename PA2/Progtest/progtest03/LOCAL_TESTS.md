# CFile local checklist (micro-tests)

## Step A: core memory/copy
- [ ] default file size is 0
- [ ] copy ctor creates independent copy
- [ ] operator= creates independent copy
- [ ] self-assignment is safe
- [ ] no leaks in asan

## Step B: seek/read/write
- [ ] write at pos 0 increases size
- [ ] seek inside range works
- [ ] seek outside range fails
- [ ] read returns only available bytes
- [ ] overwrite in middle keeps size unless extending

## Step C: truncate
- [ ] truncate at middle shrinks size
- [ ] truncate at end leaves size unchanged
- [ ] truncate at 0 empties file

## Step D: versions
- [ ] addVersion stores content and cursor
- [ ] undoVersion restores both content and cursor
- [ ] multiple undo works in LIFO order
- [ ] undoVersion on empty history returns false

## Step E: sample parity
- [ ] provided sample main passes
- [ ] random quick smoke tests pass

## Step F: optimization (later)
- [ ] memory usage reduced with shared snapshots
- [ ] no shared-mutation bugs (COW correctness)
