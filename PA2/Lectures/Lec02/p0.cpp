// lets create class array 
#include <iostream>
#include <sstream>

using namespace std;

class Array
{
private:
    size_t size_, cap_;
    int *data_;

public:
    Array();
    ~Array();

    size_t size() const;
    int& at(size_t i);
    int at(size_t i) const;
    void push(int x);
    void sort();
    void print(std::ostream& out = std::cout) const;
    bool read(std::istream& inp = std::cin);
    

};

Array::Array() : size_(0), cap_(0), data_(nullptr){}

Array::~Array()
{
    delete[] data_;
}


void Array::push(int x)
{
    if(size_ >= cap_)
    {
        cap_ = cap_ *2 +2;
        
        int *new_data = new int[cap_]; // new array with new cap

        for(size_t i=0;i<size_;i++)
        {
            // copying old array to the new one
            new_data[i]=data_[i];
        }

        delete[] data_;
        data_ = new_data;
    }
    data_[size_++]=x;
}

int& Array::at(size_t i)
{
    return data_[i]; 
}

int Array::at(size_t i) const
{
    return data_[i]; 
}

size_t Array::size() const
{
    return size_;
}

// i will create non const sort function , it will work on non-const object
void Array::sort() 
{
    for (size_t j = 0; j < size(); j++)
        for (size_t i = 1; i < size(); i++)
            if (at(i - 1) > at(i))
                std::swap(at(i - 1), at(i));
}

void Array::print(ostream& out) const 
{
    for (size_t i = 0; i < size(); i++)
        out << at(i) << std::endl;
}
//void Array::print(std::ostream& out) const {
//                                     ↑
//                              const method
//                         'this' is const here
//                      → compiler picks int at() const ✅


bool Array::read(istream& inp)
{
    int x;
    while (inp >> x)
        push(x);
    return inp.eof();
}


int main()
{
    // ─────────────────────────────────────────
    // TEST 1: constructor
    // creating an object → constructor runs automatically
    // ─────────────────────────────────────────
    std::cout << "===== TEST 1: Constructor =====" << std::endl;
    Array a;
    std::cout << "size = " << a.size() << std::endl;  // expected: 0
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 2: push() — adding elements
    // ─────────────────────────────────────────
    std::cout << "===== TEST 2: push() =====" << std::endl;
    a.push(5);
    a.push(3);
    a.push(8);
    a.push(1);
    a.push(9);
    a.push(2);
    std::cout << "size after 6 pushes = " << a.size() << std::endl;  // expected: 6
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 3: print() — printing unsorted array
    // ─────────────────────────────────────────
    std::cout << "===== TEST 3: print() before sort =====" << std::endl;
    a.print();  // expected: 5 3 8 1 9 2 (each on a new line)
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 4: sort() — sorting the array
    // ─────────────────────────────────────────
    std::cout << "===== TEST 4: sort() =====" << std::endl;
    a.sort();
    a.print();  // expected: 1 2 3 5 8 9 (each on a new line)
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 5: at() non-const — reading and modifying
    // ─────────────────────────────────────────
    std::cout << "===== TEST 5: at() non-const =====" << std::endl;
    std::cout << "element at index 0 = " << a.at(0) << std::endl;  // expected: 1
    a.at(0) = 99;  // modify the first element directly via reference
    std::cout << "after a.at(0) = 99, element at index 0 = " << a.at(0) << std::endl;  // expected: 99
    a.at(0) = 1;   // restore it back
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 6: at() const — only reading via const reference
    // ─────────────────────────────────────────
    std::cout << "===== TEST 6: at() const =====" << std::endl;
    const Array& ca = a;  // const reference to a
    std::cout << "const at(0) = " << ca.at(0) << std::endl;  // expected: 1
    std::cout << "const at(2) = " << ca.at(2) << std::endl;  // expected: 3
    // ca.at(0) = 99;  // ← this would be a compile ERROR, const object!
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 7: print() to std::cerr instead of std::cout
    // demonstrating ostream flexibility
    // ─────────────────────────────────────────
    std::cout << "===== TEST 7: print() to cerr =====" << std::endl;
    std::cerr << "(this goes to error output)" << std::endl;
    a.print(std::cerr);  // prints to error stream instead of screen
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 8: read() from a stringstream
    // instead of keyboard, read from a string of numbers
    // ─────────────────────────────────────────
    std::cout << "===== TEST 8: read() from stringstream =====" << std::endl;
    std::istringstream input("10 20 30 40 50");
    Array b;
    bool ok = b.read(input);
    std::cout << "read() returned: " << (ok ? "true (valid input)" : "false (invalid input)") << std::endl;
    std::cout << "array b after read: " << std::endl;
    b.print();  // expected: 10 20 30 40 50
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 9: read() with invalid input
    // ─────────────────────────────────────────
    std::cout << "===== TEST 9: read() invalid input =====" << std::endl;
    std::istringstream bad_input("10 20 abc 40");
    Array c;
    bool ok2 = c.read(bad_input);
    std::cout << "read() returned: " << (ok2 ? "true (valid)" : "false (invalid input detected!)") << std::endl;
    std::cout << "array c after bad read: " << std::endl;
    c.print();  // expected: 10 20 (stopped at abc)
    std::cout << std::endl;

    // ─────────────────────────────────────────
    // TEST 10: destructor
    // destructor runs automatically when object goes out of scope
    // ─────────────────────────────────────────
    std::cout << "===== TEST 10: destructor =====" << std::endl;
    {
        Array temp;
        temp.push(1);
        temp.push(2);
        temp.push(3);
        std::cout << "temp array inside block: " << std::endl;
        temp.print();
    }  // ← destructor runs HERE, frees heap memory automatically
    std::cout << "temp is now destroyed (went out of scope)" << std::endl;
    std::cout << std::endl;

    return 0;
}