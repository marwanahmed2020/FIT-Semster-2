
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;


class CTimeStamp
{
private:
  int year_, month_, day_, hour_, minute_, second_;
  
public:
  CTimeStamp();
  CTimeStamp(int year, int month, int day, int hour, int minute, int second);
   bool operator<(const CTimeStamp& o) const {
  if (year_!=o.year_) return year_<o.year_;
  if (month_!=o.month_) return month_<o.month_;
  if (day_!=o.day_) return day_<o.day_;
  if (hour_!=o.hour_) return hour_<o.hour_;
  if (minute_!=o.minute_) return minute_<o.minute_;
  return second_<o.second_;
}
bool operator<=(const CTimeStamp& o) const { return ! (o < *this); }
  
};

CTimeStamp::CTimeStamp(int year, int month, int day, int hour, int minute, int second)
{
  year_=year; month_=month; day_=day; hour_=hour; minute_=minute; second_=second;
}

CTimeStamp::CTimeStamp(){}


class CContact
{
private:
  int phone1_, phone2_;

public:
  CContact(const CTimeStamp &time, int phone1, int phone2);
  bool isSelf() const;
  bool involves(int phone) const;
  int getOtherPhone(int phone) const;
  const CTimeStamp& time() const { return time_; }
  
  
// Lexicographic comparison for timestamps
bool CTimeStamp::operator<(const CTimeStamp &o) const
{
  if (year_ != o.year_) return year_ < o.year_;
  if (month_ != o.month_) return month_ < o.month_;
  if (day_ != o.day_) return day_ < o.day_;
  if (hour_ != o.hour_) return hour_ < o.hour_;
  if (minute_ != o.minute_) return minute_ < o.minute_;
  return second_ < o.second_;
}

bool CTimeStamp::operator<=(const CTimeStamp &o) const
{
  return ! (o < *this);
}

CContact::CContact(const CTimeStamp &time, int phone1, int phone2) :
  time_(time), phone1_(phone1), phone2_(phone2)
{}

bool CContact::isSelf() const
  return phone1_==phone2_; // we dont use phone here as parameter cause i dont need to i just need to find identicals if they are same then i will skip
}

bool CContact::involves(int phone) const
{
  return (phone1_== phone) || (phone2_==phone);
}

int CContact::getOtherPhone(int phone) const
{
  if(phone1_==phone) 
    return phone2_;
  if(phone2_==phone) 
    return phone1_;
}
 


class CEFaceMask
{ 
private:
  std::vector<CContact> contacts_;
  
public: 
  CEFaceMask();
  
  CEFaceMask &addContact(const CContact &contact);
  std::vector<int> CEFaceMask::listContacts(int phone) const;
  std::vector<int> listContacts(int phone, const CTimeStamp &from, const CTimeStamp &to) const;
 
};

//CEFaceMask::CEFaceMask() {}

CEFaceMask & CEFaceMask::addContact(const CContact &contact) 
{
  contacts_.push_back(contact);
  return *this;
}

std::vector<int> CEFaceMask::listContacts(int phone) const
{
  // so here i should take a phone number as a parameter and then search in the database of the contacts , to get the contacts where this number contacted with
  // i just need to list the contacts that have a contact with this number
  vector<int> results;
  
  for(const CContact &c : contacts_)
  {
    // first eliminate the duplicates using isSelf()
    if(c.isSelf())
      continue;
    
    // then try involves 
    if(c.involves(phone)){
      int other = c.getOtherPhone(phone);
      if(std::find(results.begin(), results.end(), other) == results.end()) // so find returns the value that we find and if not the value we are searching for are there then find will return the last element
      
      {
        results.push_back(other);
      }
    }
  }
  return results;
}

std::vector<int> CEFaceMask::listContacts(int phone, const CTimeStamp &from, const CTimeStamp &to) const
{
    std::vector<int> result;

    for (const CContact &c : contacts_)
    {
        if (c.isSelf() || !c.involves(phone))
            continue;

        if (!c.time().inside(from, to))
            continue;

        int other = c.getOtherPhone(phone);
        if (std::find(result.begin(), result.end(), other) == result.end())
            result.push_back(other);
    }

    return result;
}

int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
