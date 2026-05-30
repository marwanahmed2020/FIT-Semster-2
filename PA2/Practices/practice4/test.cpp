#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

class CIntArray {
public:
    CIntArray() = default;

    CIntArray(const CIntArray &other)
        : m_Size(other.m_Size),
          m_Capacity(other.m_Capacity),
          m_Data(m_Capacity ? new int[m_Capacity] : nullptr) {
        for (size_t i = 0; i < m_Size; ++i) {
            m_Data[i] = other.m_Data[i];
        }
    }

    CIntArray(CIntArray &&other) noexcept
        : m_Size(other.m_Size),
          m_Capacity(other.m_Capacity),
          m_Data(other.m_Data) {
        other.m_Size = 0;
        other.m_Capacity = 0;
        other.m_Data = nullptr;
    }

    CIntArray &operator=(CIntArray other) {
        swap(*this, other);
        return *this;
    }

    ~CIntArray() {
        delete[] m_Data;
    }

    void pushBack(int x) {
        ensureCapacityForOneMore();
        m_Data[m_Size++] = x;
    }

    void popBack() {
        if (empty()) {
            throw std::out_of_range("popBack on empty array");
        }
        --m_Size;
    }

    void insert(size_t pos, int x) {
        if (pos > m_Size) {
            throw std::out_of_range("insert position out of range");
        }
        ensureCapacityForOneMore();
        for (size_t i = m_Size; i > pos; --i) {
            m_Data[i] = m_Data[i - 1];
        }
        m_Data[pos] = x;
        ++m_Size;
    }

    void erase(size_t pos) {
        if (pos >= m_Size) {
            throw std::out_of_range("erase position out of range");
        }
        for (size_t i = pos; i + 1 < m_Size; ++i) {
            m_Data[i] = m_Data[i + 1];
        }
        --m_Size;
    }

    int find(int x) const {
        for (size_t i = 0; i < m_Size; ++i) {
            if (m_Data[i] == x) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    size_t removeValue(int x) {
        size_t newSize = 0;
        size_t removed = 0;
        for (size_t i = 0; i < m_Size; ++i) {
            if (m_Data[i] == x) {
                ++removed;
            } else {
                m_Data[newSize++] = m_Data[i];
            }
        }
        m_Size = newSize;
        return removed;
    }

    void sortAsc() {
        std::sort(m_Data, m_Data + m_Size);
    }

    int &operator[](size_t i) {
        if (i >= m_Size) {
            throw std::out_of_range("index out of range");
        }
        return m_Data[i];
    }

    int operator[](size_t i) const {
        if (i >= m_Size) {
            throw std::out_of_range("index out of range");
        }
        return m_Data[i];
    }

    bool operator==(const CIntArray &rhs) const {
        if (m_Size != rhs.m_Size) {
            return false;
        }
        for (size_t i = 0; i < m_Size; ++i) {
            if (m_Data[i] != rhs.m_Data[i]) {
                return false;
            }
        }
        return true;
    }

    size_t size() const {
        return m_Size;
    }

    bool empty() const {
        return m_Size == 0;
    }

    friend void swap(CIntArray &a, CIntArray &b) noexcept {
        using std::swap;
        swap(a.m_Size, b.m_Size);
        swap(a.m_Capacity, b.m_Capacity);
        swap(a.m_Data, b.m_Data);
    }

    friend std::ostream &operator<<(std::ostream &os, const CIntArray &a) {
        os << "[";
        for (size_t i = 0; i < a.m_Size; ++i) {
            if (i) {
                os << ", ";
            }
            os << a.m_Data[i];
        }
        os << "]";
        return os;
    }

private:
    size_t m_Size = 0;
    size_t m_Capacity = 0;
    int *m_Data = nullptr;

    void ensureCapacityForOneMore() {
        if (m_Size < m_Capacity) {
            return;
        }

        size_t newCap;
        if (m_Capacity == 0) {
            newCap = 2;
        } else {
            newCap = m_Capacity * 2;
        }

        int *newData = new int[newCap];
        for (size_t i = 0; i < m_Size; ++i) {
            newData[i] = m_Data[i];
        }

        delete[] m_Data;
        m_Data = newData;
        m_Capacity = newCap;
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
