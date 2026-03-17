#include <iostream>
using namespace std;

class Counter {
private:
    int value;

public:
    Counter(int value) {
        this->value = value; // disambiguation
    }

    Counter& add(int x) {
        this->value += x;
        return *this; // method chaining
    }

    Counter& copyFrom(const Counter& other) {
        if (this == &other) return *this; // self-check
        this->value = other.value;
        return *this;
    }

    void printAddress() const {
        cout << "this = " << this << ", value = " << this->value << endl;
    }
};

int main() {
    Counter a(10), b(3);
    a.add(5).add(2);   // chaining
    a.printAddress();
    b.copyFrom(a);
    b.printAddress();
}