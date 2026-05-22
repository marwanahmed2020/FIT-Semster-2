#include <iostream>
#include <stdexcept>
#include <cmath>




// making the class rational for rational numbers



class CRat
{
private:
    int numerator,denominator;
    void simplify();

public:
    CRat(int num=0, int den=1);

    void print(std::ostream& out) const;
    void print() const;

    // manipulators (instance methods: left operand is the instance)
    CRat add(const CRat& y) const;
    CRat neg() const;
    CRat sub(const CRat& y) const;
    bool equal(const CRat& y) const;

    // getters >>> they are basically for accessing the vars
    int get_numerator() const { return numerator; }
    int get_denominator() const { return denominator; }


    // operators
    CRat operator +(const CRat& y) const; // it will return an object 
    CRat operator -() const;
    CRat operator -(const CRat& y) const;
    bool operator ==(const CRat& y) const;
    CRat& operator +=(const CRat& y);


};
// construcor
CRat::CRat(int num, int den): numerator(num), denominator(den)
{
    if(den==0)
    {
        throw std::invalid_argument("denominator is zero");
    }
    else
    {
        simplify();
    }
}

// print function
void CRat::print(std::ostream& out) const
{
    out << '(' << numerator << '/' << denominator << ')'<<std::endl;
}

void CRat::print() const
{
    std::cout <<"("<< numerator << "/" << denominator << ")"<<std::endl;
}




void CRat::simplify()
{
    // Find GCD of num_ and den_
    int a = std::abs(numerator);
    int b = std::abs(denominator);
    while(b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    int gcd = a;
    
    // Divide both by GCD
    numerator /= gcd;
    denominator /= gcd;
    
    // Ensure denominator is positive
    if(denominator < 0)
    {
        numerator = -numerator;
        denominator = -denominator;
    }
}

CRat CRat::add(const CRat& y) const
{
    return CRat(
        numerator * y.denominator + y.numerator * denominator,
        denominator * y.denominator
    );
}

CRat CRat::neg() const
{
    return CRat(-numerator, denominator);
}

CRat CRat::sub(const CRat& y) const
{
    return add(y.neg());
}

bool CRat::equal(const CRat& y) const
{
    return numerator == y.numerator && denominator == y.denominator;
}

CRat CRat::operator+(const CRat& y) const
{
    int newNumrator=numerator * y.denominator + y.numerator * denominator;
    int newDenominator=denominator * y.denominator;
    
    return CRat(newNumrator,newDenominator);
}

CRat CRat::operator-() const
{
    return neg();
}

CRat CRat::operator-(const CRat& y) const
{
    return *this + -y;
}

bool CRat::operator==(const CRat& y) const
{
    return equal(y);
}

CRat& CRat::operator+=(const CRat& y)
{
    numerator = numerator * y.denominator + y.numerator * denominator;
    denominator = denominator * y.denominator;
    simplify();
    return *this;
}




// working with operator overloading 









int main()
{
    using namespace std;
    CRat a(3,5), b(1), c;
    a.print(); cout << ' '; b.print();
    cout << endl;
    c = a + b;
    c.print(); cout << endl;
    c = a - b;
    c.print(); cout << endl;
    cout << ((b - a) == a) << endl; // (b - a) == a
    return 0;
}