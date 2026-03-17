#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <memory>

class my_string {
public:
    static bool less(my_string const & a, my_string const & b) {
        const char * a_data = a.m_data ? a.m_data : "";
        const char * b_data = b.m_data ? b.m_data : "";
        return std::strcmp(a_data, b_data) < 0;
    }

    my_string() = default;

    my_string(my_string const & other) {
        if (!other.m_data) return;
        m_data = new char[other.m_len + 1];
        std::copy(other.m_data, other.m_data + other.m_len + 1, m_data);
        m_len = other.m_len;
    }

    my_string & operator=(my_string const & other) {
        if (this == &other) return *this;
        my_string temp = other;
        swap(temp);
        return *this;
    }

    void swap(my_string & other) {
        std::swap(m_data, other.m_data);
        std::swap(m_len, other.m_len);
    }

    ~my_string() {
        delete [] m_data;
    }

    bool read(std::istream & is) {
        std::string temp;
        is >> temp;
        if (!is) return false;

        delete [] m_data;
        m_data = new char[temp.size() + 1];
        std::copy(temp.data(), temp.data() + temp.size(), m_data);
        m_data[temp.size()] = '\0';
        m_len = temp.size();
        return true;
    }

    void print(std::ostream & os) const {
        if (m_data) os.write(m_data, m_len);
    }

private:
    char * m_data = nullptr;
    size_t m_len = 0;
};

class my_vector {
public:
    my_vector() = default;
    my_vector(my_vector const &) = default;
    my_vector & operator=(my_vector const &) = default;

    void push_back(my_string const & item);

    my_string * begin() { return m_block->m_data; }
    my_string * end() { return begin() + m_block->m_len; }
    my_string const * begin() const { return m_block->m_data; }
    my_string const * end() const { return begin() + m_block->m_len; }

    long owners() const { return m_block.use_count(); }

private:
    struct control_block {
        my_string * m_data = nullptr;
        size_t m_cap = 0;
        size_t m_len = 0;

        ~control_block() {
            delete [] m_data;
        }
    };

    void reallocate(size_t new_cap);

private:
    std::shared_ptr<control_block> m_block = std::make_shared<control_block>();
};

void my_vector::push_back(my_string const & item) {
    if (m_block->m_len >= m_block->m_cap)
        reallocate(m_block->m_cap * 3 / 2 + 10);

    m_block->m_data[m_block->m_len++] = item;
}

void my_vector::reallocate(size_t new_cap) {
    my_string * new_data = new my_string[new_cap];
    std::copy(begin(), end(), new_data);
    delete [] m_block->m_data;
    m_block->m_data = new_data;
    m_block->m_cap = new_cap;
}

void print_tokens(my_vector const & tokens, const char * title) {
    std::cout << title << "\n";
    std::cout << std::setw(25) << std::setfill('-') << "" << "\n";

    for (auto const & item : tokens) {
        item.print(std::cout);
        std::cout << "\n";
    }
}

int main() {
    my_string token;
    my_vector tokens;
    my_vector unsorted;

    while (token.read(std::cin))
        tokens.push_back(token);

    unsorted = tokens; // shared_ptr: both point to same control block
    tokens.push_back({});
    std::sort(tokens.begin(), tokens.end(), my_string::less);

    std::cout << "owners tokens=" << tokens.owners() << ", unsorted=" << unsorted.owners() << "\n";
    print_tokens(tokens, "sorted/shared_ptr");
    print_tokens(unsorted, "unsorted/shared_ptr");
    return 0;
}
