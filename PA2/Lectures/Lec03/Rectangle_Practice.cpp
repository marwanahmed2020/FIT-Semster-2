// ============================================================================
// RECTANGLE CLASS - Practice with operators and methods
//
// This file is a practice prompt only. Do not write any implementation here.
// Your task is to write the Rectangle class and tests yourself in a separate
// file or your editor, using the TODO list below as the exact problem statement.
//
// TODO 1: Design a Rectangle class with private width and height members.
// TODO 2: Add a constructor that accepts width and height.
//   - If width <= 0 or height <= 0, throw std::invalid_argument.
// TODO 3: Add getters getWidth() and getHeight().
// TODO 4: Add getArea() that returns width * height.
// TODO 5: Add getPerimeter() that returns 2 * (width + height).
// TODO 6: Add display() to print the rectangle as [width x height].
// TODO 7: Add operator* to scale the rectangle by a positive factor.
//   - If factor <= 0, throw std::invalid_argument.
// TODO 8: Add operator== to compare two rectangles.
// TODO 9: Add operator< to compare rectangles by area.
// TODO 10: In main(), create test rectangles and verify all methods/operators.
//   - Print dimensions, area, and perimeter.
//   - Compare rectangles with == and <.
//   - Scale a rectangle with operator*.
//   - Test invalid construction and scaling with try/catch.
//
// Notes:
// - Use const member functions where appropriate.
// - Keep data members private.
// - Use std::invalid_argument for validation errors.
// ============================================================================

#include <iostream>
#include <stdexcept>





class Rectangle
{
private:
    double width, height;

public:
    // constructor
    Rectangle(double w=1, double h=1);

    // getters
    double getWidth() const;
    double getHeight() const;

    // get area
    double getArea() const;

    // get perimeter
    double getPerimeter() const;

    // display
    void display(std::ostream &rectangle) const;

    // operators overloading 
    bool operator ==(const Rectangle& otherRectangle) const;

    bool operator <(const Rectangle & otherRectangle) const;

    void operator *(double factor);
};


// implementation of the class methods 


Rectangle::Rectangle(double w, double h)
{
    if(w<=0)
    {
        throw std::invalid_argument("width must be more than 0\n");
    }
    else
    {
        width = w;
    }

    if(h<=0)
    {
        throw std::invalid_argument("height must be more than 0\n");
    }
    else
    {
        height = h;
    }
}

double Rectangle::getWidth() const
{
    return width;
}

double Rectangle::getHeight() const
{
    return height;
}

double Rectangle::getArea() const
{
    return width * height;
}

double Rectangle::getPerimeter() const
{
    return 2 * (width + height);
}

void Rectangle::display(std::ostream &rectangle) const
{
    rectangle << "[" << width << " x " << height << "]" << std::endl;
}


bool Rectangle::operator ==(const Rectangle& otherRectangle) const
{
    // so basically here i will pass the other rectangle that we want to compare
    // with this rectangle i mean this object that invoked the operator function
    // and i will compare both of their width and height 
    
    return (width==otherRectangle.width) && (height==otherRectangle.height);
}

bool Rectangle::operator <(const Rectangle& otherRectangle) const
{
    // comparing by area

    return getArea() < otherRectangle.getArea();
}

void Rectangle::operator*(double factor)
{
    if(factor <= 0)
    {
        std::__throw_invalid_argument("factor is less than or equal 0\n");
    }
    else
    {
        width*=factor;
        height*=factor;
    }
}


 int main()
 {
    Rectangle rect1(3,4), rect2(5,6), rect3(3,4);

    // tesing all getters
    std::cout << "rect1 width : " << rect1.getWidth() << std::endl;
    std::cout << "rect1 height : " << rect1.getHeight() << std::endl;
    std::cout << "rect1 area : " << rect1.getArea() << std::endl;
    std::cout << "rect1 perimeter : " << rect1.getPerimeter() << std::endl;

    // testing display
    std::cout << "rect1 dimensions : ";
    rect1.display(std::cout); // printing tot the console screen

    // testing operator ==
    if(rect1 == rect2)
    {
        std::cout << "Equal !!" << std::endl;
    }
    else
    {
        std::cout << "Not Equal !!" << std::endl;
    }

    if(rect1 == rect3)
    {
        std::cout << "Equal !!" << std::endl;
    }
    else
    {
        std::cout << "Not Equal !!" << std::endl;
    }

    // testing operator < compating by areas 
    if(rect1 < rect2)
    {
        std::cout << "Area of rect2 is bigger than area of rect1" << std::endl;
    }
    else
    {
         std::cout << "Area of rect1 is bigger than area of rect2" << std::endl;
    }


    // tesing operator *
    rect1 * 2;
    std::cout << "New rect1 dimensions : ";
    rect1.display(std::cout);

 }