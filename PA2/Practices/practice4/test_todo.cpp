#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

class CIntArray {
public:
    CIntArray() = default;

    // TODO 1: deep copy constructor
    CIntArray(const CIntArray &other)
        : m_Size(0), m_Capacity(0), m_Data(nullptr) {
        (void) other;
        // Steps:
        // 1) copy m_Size and m_Capacity
        // 2) allocate new int[m_Capacity] if capacity > 0
        // 3) copy elements [0..m_Size)
    }

    // TODO 2: move constructor
    CIntArray(CIntArray &&other) noexcept
        : m_Size(0), m_Capacity(0), m_Data(nullptr) {
        (void) other;
        // Steps:
        // 1) steal pointer and size/capacity
        // 2) reset other to empty/nullptr
    }

    // TODO 3: copy assignment (copy-and-swap)
    CIntArray &operator=(CIntArray other) {
        (void) other;
        // Steps:
        // 1) swap(*this, other)
        // 2) return *this
        return *this;
    }

    ~CIntArray() {
        delete[] m_Data;
    }

    // TODO 4: pushBack
    void pushBack(int x) {
        (void) x;
        // Steps:
        // 1) ensure capacity for one more
        // 2) store x at m_Data[m_Size]
        // 3) increment m_Size
    }

    // TODO 5: popBack
    void popBack() {
        // Throw std::out_of_range if empty.
    }

    // TODO 6: insert
    void insert(size_t pos, int x) {
        (void) pos;
        (void) x;
        // If pos > m_Size throw.
        // Shift elements right from end to pos.
        // Insert x and increment size.
    }

    // TODO 7: erase
    void erase(size_t pos) {
        (void) pos;
        // If pos >= m_Size throw.
        // Shift elements left starting at pos.
        // Decrement size.
    }

    // TODO 8: find
    int find(int x) const {
        (void) x;
        // Return first index containing x, else -1.
        return -1;
    }

    // TODO 9: removeValue
    size_t removeValue(int x) {
        (void) x;
        // Remove all x in O(n) without new allocation.
        // Return number of removed elements.
        return 0;
    }

    // TODO 10: sortAsc
    void sortAsc() {
        // Sort used range [0, m_Size).
    }

    // TODO 11: operator[] (non-const)
    int &operator[](size_t i) {
        (void) i;
        // Throw std::out_of_range for bad index.
        // Return reference to element.
        throw std::out_of_range("not implemented");
    }

    // TODO 12: operator[] (const)
    int operator[](size_t i) const {
        (void) i;
        // Throw std::out_of_range for bad index.
        throw std::out_of_range("not implemented");
    }

    // TODO 13: operator==
    bool operator==(const CIntArray &rhs) const {
        (void) rhs;
        // Compare size first, then all elements.
        return false;
    }

    size_t size() const {
        return m_Size;
    }

    bool empty() const {
        return m_Size == 0;
    }

    // TODO 14: swap helper
    friend void swap(CIntArray &a, CIntArray &b) noexcept {
        // Use std::swap on size, capacity, pointer.
        (void) a;
        (void) b;
    }

    friend std::ostream &operator<<(std::ostream &os, const CIntArray &a) {
        os << "[";
        for (size_t i = 0; i < a.m_Size; ++i) {
            if (i) os << ", ";
            os << a.m_Data[i];
        }
        os << "]";
        return os;
    }

private:
    size_t m_Size = 0;
    size_t m_Capacity = 0;
    int *m_Data = nullptr;

    // TODO 15: internal growth helper
    void ensureCapacityForOneMore() {
        // If m_Size < m_Capacity: return.
        // Else allocate bigger array (start at 2, then double), copy old data,
        // delete old data, update pointer and capacity.
    }
};

int main() {
    CIntArray a;
    assert(a.empty());

    a.pushBack(5);
    a.pushBack(2);
    a.pushBack(5);
    assert(a.size() == 3);
    assert(a.find(2) == 1);
    assert(a.find(42) == -1);

    CIntArray b = a; // copy ctor
    b[1] = 99;
    assert(a[1] == 2);  // proves deep copy
    assert(b[1] == 99);

    CIntArray c = std::move(b); // move ctor
    assert(c.size() == 3);

    c.insert(1, 7);
    assert((c == CIntArray(c)));
    c.erase(0);
    assert(c.size() == 3);

    c.pushBack(5);
    size_t removed = c.removeValue(5);
    assert(removed == 2);
    assert(c.size() == 2);

    c.sortAsc();
    assert(c[0] == 7);
    assert(c[1] == 99);

    bool threw = false;
    try {
        c.erase(100);
    } catch (const std::out_of_range &) {
        threw = true;
    }
    assert(threw);

    std::cout << c << std::endl;
    return 0;
}
