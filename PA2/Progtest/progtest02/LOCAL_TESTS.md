# Local test checklist by step

## Step A validation
- [ ] `CDate(2000,2,29)` valid
- [ ] `CDate(2001,2,29)` throws
- [ ] `CDate(2000,11,31)` throws

## Step B serial conversion
- [ ] Round-trip conversion works on random dates in 2000..2030
- [ ] Day transitions: end of month, end of year, leap day

## Step C arithmetic and compare
- [ ] `a + 1500`
- [ ] `b - 2000`
- [ ] `b - a`
- [ ] all relational operators

## Step D ++/--
- [ ] prefix ++
- [ ] prefix --
- [ ] postfix ++
- [ ] postfix --

## Step E stream ISO
- [ ] `<<` emits `YYYY-MM-DD`
- [ ] `>>` accepts valid date
- [ ] `>>` rejects invalid and keeps old object unchanged

## Step F bonus format
- [ ] output custom format `%d.%m.%Y`
- [ ] input custom format `%m/%d/%Y`
- [ ] invalid format spec handling
