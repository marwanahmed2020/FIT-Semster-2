#include <iostream>
#include <cmath>
#include <stdexcept>

// ============================================================================
// RECTANGLE CLASS - Practice with operators and methods
// ============================================================================

class Rectangle
{
private:
    double width, height;
    
public:
    // TODO 1: Constructor Rectangle(double w = 1.0, double h = 1.0)
    // Validate: throw std::invalid_argument if w <= 0 or h <= 0
    Rectangle(double w = 1.0, double h = 1.0);
    
    // TODO 2: Getter double getWidth() const - return width
    double getWidth() const;
    
    // TODO 3: Getter double getHeight() const - return height
    double getHeight() const;
    
    // TODO 4: Method double getArea() const - return width * height
    double getArea() const;
    
    // TODO 5: Method double getPerimeter() const - return 2 * (width + height)
    double getPerimeter() const;
    
    // TODO 6: Method void display() const - print [width x height]
    void display() const;
    
    // TODO 7: Operator Rectangle operator*(double factor) const
    // Validate: throw std::invalid_argument if factor <= 0
    // Return new Rectangle with (width * factor, height * factor)
    Rectangle operator*(double factor) const;
    
    // TODO 8: Operator bool operator==(const Rectangle& other) const
    // Compare width and height with epsilon tolerance (1e-9)
    bool operator==(const Rectangle& other) const;
    
    // TODO 9: Operator bool operator<(const Rectangle& other) const
    // Return true if this.getArea() < other.getArea()
    bool operator<(const Rectangle& other) const;
};

// ============================================================================
// IMPLEMENTATIONS
// ============================================================================

// TODO 1: Constructor


// TODO 2: getWidth()


// TODO 3: getHeight()


// TODO 4: getArea()


// TODO 5: getPerimeter()


// TODO 6: display()


// TODO 7: operator*


// TODO 8: operator==


// TODO 9: operator<


// ============================================================================
// MAIN
// ============================================================================

int main()
{
    using namespace std;
    
    // TODO 10: Create Rectangle rect1(5, 3)
    
    // TODO 11: Create Rectangle rect2(4, 4)
    
    // TODO 12: Display rect1 and rect2
    
    // TODO 13: Print area and perimeter of rect1 and rect2
    
    // TODO 14: Create rect1_scaled = rect1 * 2.0 and display
    
    // TODO 15: Print comparison: rect1 == rect2
    
    // TODO 16: Print comparison: rect1 < rect2
    
    // TODO 17: Create Rectangle square(5, 5) and display
    
    // TODO 18: Print comparisons: square == rect1, square < rect1
    
    // TODO 19: Error handling - try to create Rectangle invalid(0, 5)
    // Use try/catch to catch std::invalid_argument
    
    return 0;
}
