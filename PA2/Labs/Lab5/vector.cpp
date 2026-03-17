#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>


class my_string {
    public:
        static bool less(my_string const & a, my_string const & b) {
            const char * a_data = a.m_data? a.m_data : "";
            const char * b_data = b.m_data? b.m_data : "";

            return std::strcmp(a_data, b_data) < 0;
        }

        my_string() = default;
        // Rule of 3
        my_string(my_string const & other) {
            if (!other.m_data) return;
            m_data = new char [other.m_len+1];
            std::copy(other.m_data, other.m_data+other.m_len+1, m_data);
            m_len = other.m_len;
        }

        my_string& operator= (my_string const & other) {
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
            if (m_data) delete [] m_data;
            m_data = new char [temp.size()+1];
            std::copy(temp.data(), temp.data()+temp.size()+1, m_data);
            m_len = temp.size();
            return m_len > 0;
        }

        void print(std::ostream & os) const {
            os.write(m_data, m_len);
        }


    private:
        char * m_data = nullptr;
        size_t m_len = 0;
};

class my_vector {
    public:
        my_vector() = default;
        // Rule of 3
        my_vector(my_vector const &);
        my_vector& operator= (my_vector const &);
        ~my_vector();

        void push_back(my_string const & item);

        my_string * begin() { return m_data; };
        my_string * end() { return begin() + m_len; }
        my_string const * begin() const { return m_data; };
        my_string const * end() const { return begin() + m_len; }
        void swap(my_vector & other);

    private:
        void reallocate(size_t new_cap);

    private:
        my_string * m_data = nullptr;
        size_t m_cap = 0;
        size_t m_len = 0;
};

my_vector::my_vector(my_vector const & other) {
    reallocate(other.m_len);
    for (auto const & item : other) {
        push_back(item);
    }
}

my_vector& my_vector::operator= (my_vector const & other) {
    if (this == &other) return *this;
    // Copy ans Swap Idiom
    my_vector temp = other;
    swap(temp);

    return *this;
}

my_vector::~my_vector() {
    delete [] m_data;
}

void my_vector::swap(my_vector & other) {
    std::swap(m_data, other.m_data);
    std::swap(m_cap, other.m_cap);
    std::swap(m_len, other.m_len);
}

void my_vector::push_back(my_string const & item) {
    if (m_len >= m_cap) {
        reallocate(m_cap * 1.5 + 10);
    }
    m_data[m_len ++] = item;
}

void my_vector::reallocate(size_t new_cap) {
    my_string * new_data = new my_string[new_cap];
    std::copy(begin(), end(), new_data);
    delete [] m_data;
    m_data = new_data;
    m_cap = new_cap;
}

void print_tokens(my_vector const & tokens) {
    std::cout << std::setw(25) << std::setfill('-') << "\n";

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

    
    unsorted = tokens; // Op =
    tokens = tokens;
    tokens.push_back({});
    std::sort(tokens.begin(), tokens.end(), my_string::less);

    print_tokens(tokens);
    print_tokens(unsorted);
    return 0;
}
