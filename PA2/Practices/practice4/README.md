# Practice 4

## Dynamic Int Array (Progtest Style)

Implement class `CIntArray` that stores integers in manually managed dynamic memory.

This practice is meant to be simpler than `practice3`, but still covers:

- dynamic allocation (`new[]`, `delete[]`)
- deep copy
- copy constructor
- copy assignment (copy-and-swap)
- move constructor
- operator overloading
- searching and simple array algorithms

## Required Interface

```cpp
class CIntArray
{
public:
    CIntArray();
    CIntArray(const CIntArray &other);          // deep copy
    CIntArray(CIntArray &&other) noexcept;      // move
    CIntArray &operator=(CIntArray other);      // copy-and-swap
    ~CIntArray();

    void pushBack(int x);
    void popBack();                             // throw if empty
    void insert(size_t pos, int x);             // throw if pos > size
    void erase(size_t pos);                     // throw if pos >= size

    int find(int x) const;                      // first index, else -1
    size_t removeValue(int x);                  // remove all, return count
    void sortAsc();

    int &operator[](size_t i);                  // throw if bad index
    int operator[](size_t i) const;             // throw if bad index
    bool operator==(const CIntArray &rhs) const;

    size_t size() const;
    bool empty() const;

    friend void swap(CIntArray &a, CIntArray &b) noexcept;
    friend std::ostream &operator<<(std::ostream &os, const CIntArray &a);
};
```

## What Each Function Means

- `CIntArray()`:
  Creates an empty array.
- `CIntArray(const CIntArray&)`:
  Creates a deep copy (new memory, copied elements).
- `CIntArray(CIntArray&&)`:
  Transfers ownership from source to destination (fast move).
- `operator=(CIntArray other)`:
  Copy-and-swap assignment; safe and concise.
- `pushBack(x)`:
  Appends `x`, growing capacity if needed.
- `popBack()`:
  Removes last element.
- `insert(pos, x)`:
  Inserts at `pos`, shifts right.
- `erase(pos)`:
  Removes element at `pos`, shifts left.
- `find(x)`:
  Returns first index containing `x`, or `-1`.
- `removeValue(x)`:
  Removes all `x` values in linear time without extra array.
- `sortAsc()`:
  Sorts used range ascending.
- `operator[]`:
  Access element by index (with bounds check).
- `operator==`:
  Compares arrays by size and content.
- `swap(a,b)`:
  Swaps internals (`size`, `capacity`, pointer).

## Error Handling Rules

Throw `std::out_of_range` in:

- `popBack()` when empty
- `insert(pos, x)` when `pos > size`
- `erase(pos)` when `pos >= size`
- `operator[]` when index is invalid

## Deliverable

Implement in `test.cpp` in this folder so all included asserts pass.

## Study Mode (recommended)

- Your exercise file: `test_todo.cpp` (contains TODOs only).
- Reference solution: `test.cpp`.

Use this workflow:

1. Work only in `test_todo.cpp`.
2. Compile often and fix one TODO at a time.
3. Compare with `test.cpp` only after you finish or get stuck.

Compile example:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -g test.cpp -o test
./test
```
