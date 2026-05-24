
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
};

CTimeStamp::CTimeStamp(int year, int month, int day, int hour, int minute, int second)
{
  year_=year; month_=month; day_=day; hour_=hour; minute_=minute; second_=second;
}

CTimeStamp::CTimeStamp(){}


class CContact
{
private:
  CTimeStamp time_;
  int phone1_, phone2_;

public:
  CContact(const CTimeStamp &time, int phone1, int phone2);
  bool isSelf() const;
  bool involves(int phone) const;
  int getOtherPhone(int phone) const;
  const CTimeStamp& time() const { return time_; }
}; 

CContact::CContact(const CTimeStamp &time, int phone1, int phone2) :
  time_(time), phone1_(phone1), phone2_(phone2)
{}

bool CContact::isSelf() const
{
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
  
};

CEFaceMask::CEFaceMask() {}

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
    if(c.involves(phone))
      results.push_back(c.getOtherPhone(phone));
      
  }
  return results;
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
