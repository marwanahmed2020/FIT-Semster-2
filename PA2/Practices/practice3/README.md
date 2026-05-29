# Practice 3

## Owned Text Buffer with Copying, Moving, and Polymorphism

Your task is to design and implement a small text-buffer library that stores a list of strings in dynamically allocated memory. The goal is to practice:

- manual memory allocation and deallocation
- deep copy
- copy constructor
- copy assignment operator
- move constructor and move assignment operator
- `std::unique_ptr`
- `std::move`
- a simple use of inheritance and polymorphism

The exercise is intentionally small, but it mixes several important C++ topics from the copying lecture.

## Story

You are building a tiny editor backend for a note-taking app. Each note owns a dynamic array of text lines. Notes can be copied, moved, appended to, filtered, and printed. Later, a special note type adds a title and changes how it prints itself.

## Required classes

### 1. `CTextNote`
Represents a note that owns a dynamic array of text lines.

Required behavior:
- store lines in dynamically allocated memory
- support adding a line to the end
- support removing a line by index
- support reading a line by index
- support printing all lines
- support deep copying
- support moving

### 2. `CTitleNote`
Derived from `CTextNote`

Required behavior:
- add a title
- print the title before the note content
- reuse the base class functionality

### 3. `INotePrinter` or a similar abstract interface
Use a small abstract base class with at least one virtual method so you can practice dynamic binding.

You can decide the exact interface, but the tests should be able to:
- store a pointer or reference to the base class
- call the virtual print function
- see the derived behavior

## Suggested interface

You may use this interface as a starting point:

```cpp
class CTextNote
{
public:
    CTextNote();
    CTextNote(const CTextNote &other);
    CTextNote(CTextNote &&other) noexcept;
    CTextNote &operator=(CTextNote other);
    ~CTextNote();

    void addLine(const std::string &line);
    void removeLine(size_t idx);
    const std::string &line(size_t idx) const;
    size_t size() const;
    void print(std::ostream &out) const;

    friend void swap(CTextNote &a, CTextNote &b) noexcept;

private:
    size_t m_Size;
    size_t m_Capacity;
    std::unique_ptr<std::string[]> m_Data;
};
```

Then add a derived class:

```cpp
class CTitleNote : public CTextNote
{
public:
    CTitleNote(std::string title);
    void print(std::ostream &out) const override;

private:
    std::string m_Title;
};
```

You do not have to follow this exact interface, but your implementation should still practice the same topics.

## What to practice

- constructor for an object owning memory
- copy constructor performing deep copy
- copy assignment using copy-and-swap
- move constructor and move assignment
- `std::unique_ptr` for owned arrays
- `swap` with `using std::swap;`
- `virtual` and `override`
- calling derived behavior through a base pointer or reference

## Hints

### Copy constructor
When you copy a note, the new note must get its own array. Do not copy the raw pointer only.

Hint:
- allocate a new array
- copy each stored line one by one

### Copy assignment
A safe pattern is copy-and-swap:
- take the parameter by value
- swap the current object with the parameter
- return `*this`

Hint:
- define `friend void swap(CTextNote &a, CTextNote &b) noexcept`
- inside `swap`, use `using std::swap;`

### Move operations
After moving, the source object must remain valid and destroyable.

Hint:
- use `std::move` when transferring strings or arrays
- reset the moved-from object to an empty state

### Inheritance and polymorphism
If you want dynamic binding, the base print function must be `virtual`.

Hint:
- call the method through a base pointer or reference
- add `override` in the derived class

### Bounds checking
If the user asks for an invalid line or removes a line out of range, throw `std::out_of_range`.

## Example usage

```cpp
CTextNote a;
a.addLine("first");
a.addLine("second");

CTextNote b = a;        // copy constructor
b.addLine("third");

CTitleNote titled("Lecture notes");
titled.addLine("copy constructor");
titled.addLine("move constructor");

std::unique_ptr<CTextNote> p = std::make_unique<CTitleNote>("Advanced topic");
p->print(std::cout);    // dynamic binding
```

## Optional challenge

If you want an extra challenge, add one more derived class:

- `CBulletNote` that prints every line with `- ` at the beginning

or add one more feature:

- `filterLines(keyword)` that removes all lines not containing a keyword

## Deliverable

Create your implementation in this folder, for example:

- `practice3.cpp`
- or `text_note.cpp`

The file should compile on its own and include a small `main()` with tests.
