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
    if(m_Size == m_Capacity)
    {
        std::size_t newCap;
        if(m_Capacity == 0)
            newCap = 2;
        else
            newCap = m_Capacity * 2; // incresead the size of the note
            // by 2 lines

        auto newData = std::make_unique<std::string[]>(newCap);
        for(std::size_t i =0; i<m_Size; i++)
        {
            newData[i] = m_Data[i];
        }

        m_Data = std::move(newData);
        m_Capacity = newCap;
    }

    m_Data[m_Size++] = line;



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
