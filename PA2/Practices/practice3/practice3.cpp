#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class INotePrinter
{
public:
    virtual ~INotePrinter() = default;
    virtual void print(std::ostream &out) const = 0;
};

class CTextNote : public INotePrinter
{
public:
    CTextNote() = default;
    CTextNote(const CTextNote &other);
    CTextNote(CTextNote &&other) noexcept;
    CTextNote &operator=(CTextNote other);
    ~CTextNote() override = default;

    void addLine(const std::string &line);
    void removeLine(std::size_t idx);
    const std::string &line(std::size_t idx) const;
    std::size_t size() const;
    void print(std::ostream &out) const override;

    friend void swap(CTextNote &a, CTextNote &b) noexcept;

private:
    std::size_t m_Size = 0;
    std::size_t m_Capacity = 0;
    std::unique_ptr<std::string[]> m_Data;
};

class CTitleNote : public CTextNote
{
public:
    explicit CTitleNote(std::string title);
    void print(std::ostream &out) const override;

private:
    std::string m_Title;
};

// TODO: implement the special member functions and line-management methods.
// Hints:
// - copy constructor must allocate a new array
// - copy assignment can use copy-and-swap
// - move constructor should steal the array and leave the source valid
// - use override for polymorphic print

// the copy constructor
CTextNote::CTextNote(const CTextNote &other)
    : m_Size(other.m_Size), m_Capacity(other.m_Capacity)
{
    if(m_Capacity)
    {
        this->m_Data = std::make_unique<std::string[]>(m_Capacity);
        for(std::size_t i=0;i<m_Size;i++)
        {
            m_Data[i] = other.m_Data[i];
        }
    }
}

CTextNote & CTextNote::operator=(CTextNote other)
{
    this->m_Size = other.m_Size;
    this->m_Capacity = other.m_Capacity;
    if(m_Capacity)
    {
        this->m_Data = std::make_unique<std::string[]>(m_Capacity);
        for(std::size_t i=0;i<m_Size;i++)
        {
            m_Data[i] = other.m_Data[i];
        }
    }
}


void CTextNote::addLine(const std::string &line)
{
    if(m_Size <= m_Capacity)
    {
        std::cout<< "the Text note is full" << std::endl;
    }
    else if(line.size()+m_Capacity > m_Size)
    {
        std::cout<<"the line is too long for the size of the text note"<<std::endl;
    }
    else
    {
        m_Capacity += line.size();
        
    }
}





int main()
{
    // TODO: write your own tests here.
    // Suggested tests:
    // 1. add a few lines to a note
    // 2. copy it and modify the copy
    // 3. move it into another note
    // 4. print through a base pointer/reference
    // 5. try invalid index access and catch std::out_of_range
    std::cout << "Practice 3 skeleton" << std::endl;
    return 0;
}
