#include <iostream>
#include <cmath>
#include <stdexcept>

// ============================================================================
// RECTANGLE CLASS - Practice with operators and methods
// 
// TODO 1: Understand the class design - Rectangle has width and height
// TODO 2: Implement the constructor with validation
// TODO 3: Implement getters for width and height
// TODO 4: Implement area and perimeter calculation
// TODO 5: Implement operator+ to scale rectangle
// TODO 6: Implement operator== to compare rectangles
// TODO 7: Implement operator< to compare by area
// TODO 8: Implement display method
// TODO 9: Use all operators in main()
// ============================================================================

class Rectangle
{
private:
    double width, height;
    
public:
    // TODO 2: Constructor - initialize width and height
    // Validate that both are positive (> 0)
    // Throw std::invalid_argument if not
    Rectangle(double w = 1.0, double h = 1.0);
    
    // TODO 3: Getter methods (const and return by value)
    double getWidth() const;
    double getHeight() const;
    
    // TODO 4: Calculate area (width * height)
    double getArea() const;
    
    // TODO 4: Calculate perimeter (2 * (width + height))
    double getPerimeter() const;
    
    // TODO 8: Display rectangle as: [width x height]
    void display() const;
    
    // TODO 5: operator* - scale rectangle by a factor
    // Example: rect * 2.0 creates a rectangle with double dimensions
    // Signature: Rectangle operator*(double factor) const;
    Rectangle operator*(double factor) const;
    
    // TODO 6: operator== - two rectangles are equal if they have same width and height
    // Signature: bool operator==(const Rectangle& other) const;
    bool operator==(const Rectangle& other) const;
    
    // TODO 7: operator< - compare rectangles by area
    // Returns true if this rectangle's area < other rectangle's area
    // Signature: bool operator<(const Rectangle& other) const;
    bool operator<(const Rectangle& other) const;
};

// ============================================================================
// IMPLEMENTATIONS
// ============================================================================

// TODO 1: Constructor
Rectangle::Rectangle(double w, double h) : width(w), height(h)
{
    // YOUR CODE: Validate that w and h are positive, throw if not
    
}

// TODO 3: Getters
double Rectangle::getWidth() const
{
    // YOUR CODE: Return width
    
}

double Rectangle::getHeight() const
{
    // YOUR CODE: Return height
    
}

// TODO 4: Area and Perimeter
double Rectangle::getArea() const
{
    // YOUR CODE: Return width * height
    
}

double Rectangle::getPerimeter() const
{
    // YOUR CODE: Return 2 * (width + height)
    
}

// TODO 8: Display
void Rectangle::display() const
{
    // YOUR CODE: Print [width x height] format
    
}

// TODO 5: Operator* - scale by factor
Rectangle Rectangle::operator*(double factor) const
{
    // YOUR CODE: Return new Rectangle with scaled dimensions, validate factor > 0
    
}

// TODO 6: Operator== - compare dimensions
bool Rectangle::operator==(const Rectangle& other) const
{
    // YOUR CODE: Compare with epsilon tolerance (1e-9)
    
}

// TODO 7: Operator< - compare by area
bool Rectangle::operator<(const Rectangle& other) const
{
    // YOUR CODE: Compare by area
    
}

// ============================================================================
// MAIN FUNCTION - Practice using the Rectangle class
// ============================================================================

int main()
{
    using namespace std;
    
    cout << "=== Rectangle Class Practice ===" << endl << endl;
    
    // TODO 9.1: Create rect1 (5 x 3) and display it
    
    
    // TODO 9.2: Create rect2 (4 x 4) and display it
    
    
    // TODO 9.3: Print area and perimeter of both rectangles
    
    
    // TODO 9.4: Scale rect1 by 2.0 using operator* and display
    
    
    // TODO 9.5: Compare rect1 and rect2 using operator==
    
    
    // TODO 9.6: Compare areas using operator<
    
    
    // TODO 9.7: Create a square (5 x 5) and compare
    
    
    // TODO 9.8: Test error handling with try/catch
    
    
    return 0;
}

// ============================================================================
// BONUS TASKS (if you want more practice):
// ============================================================================
// 
// BONUS 1: Add operator+ to combine two rectangles (add widths and heights)
// BONUS 2: Add operator- for the unary minus (negate/reflect?)
// BONUS 3: Add operator!= (not equal)
// BONUS 4: Add operator<= (less than or equal by area)
// BONUS 5: Add a resize(double w, double h) method that modifies the rectangle
// BONUS 6: Add operator+= to scale the rectangle in-place (like += in CRat)
// BONUS 7: Add a getDiagonal() method to calculate diagonal length using Pythagorean theorem
// 
// ============================================================================
