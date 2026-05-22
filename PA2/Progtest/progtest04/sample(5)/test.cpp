#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#include <compare>
#include <iterator>
#endif /* __PROGTEST */

#ifndef __PROGTEST__
class CTimeStamp
{
public:
  CTimeStamp(int year,int month,int day,int hour,int minute,int sec);
  std::strong_ordering operator<=>(const CTimeStamp &x) const;
  bool operator==(const CTimeStamp &x) const;
    friend std::ostream &operator<<(std::ostream &os,const CTimeStamp &x);
private:
    int m_Year;
    int m_Month;
    int m_Day;
    int m_Hour;
    int m_Minute;
    int m_Sec;
};
//=================================================================================================
class CMailBody
{
public:
    CMailBody(int size,const char data[]);
    // copy cons/op=/destructor is correctly implemented in the testing environment
    friend std::ostream &operator<<(std::ostream &os,const CMailBody &x)
    {
      return os << "mail body: " << x . m_Size << " B";
    }
private:
    int m_Size;
    char* m_Data;
};
//=================================================================================================
class CAttach
{
public:
    CAttach(int x)
      : m_X(x)
    {
    }
    void addRef()
    { 
      m_RefCnt++; 
    }
    void release()
    { 
      if (!--m_RefCnt)
        delete this; 
    }
private:
    int m_X;
    int m_RefCnt = 1;
    CAttach(const CAttach &x);
    CAttach &operator=(const CAttach &x);
    ~CAttach() = default;
    friend std::ostream &operator<<(std::ostream &os,const CAttach &x)
    {
      return os << "attachment: " << x . m_X << " B";
    }
};
//=================================================================================================
#endif /* __PROGTEST__, DO NOT remove */


class CMail
{
public:
    CMail(const CTimeStamp &timeStamp,const std::string &from,const CMailBody &body,CAttach *attach);
    CMail(const CMail &x);
    CMail &operator=(const CMail &x);
    ~CMail();
    const std::string &from() const;
    const CMailBody &body() const;
    const CTimeStamp &timeStamp() const;
    CAttach *attachment() const;
    friend std::ostream &operator<<(std::ostream &os,const CMail &x);
private:
    CTimeStamp m_TimeStamp;
    std::string m_From;
    CMailBody m_Body;
    CAttach *m_Attach;
};
//=================================================================================================
class CMailBox
{
public:
    CMailBox();
    bool delivery(const CMail &mail);
    bool newFolder(const std::string &folderName);
    bool moveMail(const std::string &fromFolder,const std::string &toFolder);
    std::list<CMail> listMail(const std::string &folderName,const CTimeStamp &from,const CTimeStamp &to) const;
    std::set<std::string> listAddr(const CTimeStamp &from,const CTimeStamp &to) const;
private:
    std::map<std::string, std::multimap<CTimeStamp, CMail>> m_Folders;
};
//=================================================================================================
#ifndef __PROGTEST__
CTimeStamp::CTimeStamp(int year,int month,int day,int hour,int minute,int sec)
  : m_Year(year),
    m_Month(month),
    m_Day(day),
    m_Hour(hour),
    m_Minute(minute),
    m_Sec(sec)
{
}

std::strong_ordering CTimeStamp::operator<=>(const CTimeStamp &x) const
{
  if (m_Year != x.m_Year) return m_Year < x.m_Year ? std::strong_ordering::less : std::strong_ordering::greater;
  if (m_Month != x.m_Month) return m_Month < x.m_Month ? std::strong_ordering::less : std::strong_ordering::greater;
  if (m_Day != x.m_Day) return m_Day < x.m_Day ? std::strong_ordering::less : std::strong_ordering::greater;
  if (m_Hour != x.m_Hour) return m_Hour < x.m_Hour ? std::strong_ordering::less : std::strong_ordering::greater;
  if (m_Minute != x.m_Minute) return m_Minute < x.m_Minute ? std::strong_ordering::less : std::strong_ordering::greater;
  if (m_Sec != x.m_Sec) return m_Sec < x.m_Sec ? std::strong_ordering::less : std::strong_ordering::greater;
  return std::strong_ordering::equivalent;
}

bool CTimeStamp::operator==(const CTimeStamp &x) const
{
  return m_Year == x . m_Year
      && m_Month == x . m_Month
      && m_Day == x . m_Day
      && m_Hour == x . m_Hour
      && m_Minute == x . m_Minute
      && m_Sec == x . m_Sec;
}

std::ostream &operator<<(std::ostream &os,const CTimeStamp &x)
{
  std::ios oldState(nullptr);
  oldState . copyfmt ( os );
  os << std::setfill ( '0' )
     << std::setw ( 4 ) << x . m_Year << '-'
     << std::setw ( 2 ) << x . m_Month << '-'
     << std::setw ( 2 ) << x . m_Day << ' '
     << std::setw ( 2 ) << x . m_Hour << ':'
     << std::setw ( 2 ) << x . m_Minute << ':'
     << std::setw ( 2 ) << x . m_Sec;
  os . copyfmt ( oldState );
  return os;
}

CMailBody::CMailBody(int size,const char data[])
  : m_Size(size),
    m_Data(new char [size + 1])
{
  if ( size > 0 )
    std::memcpy ( m_Data, data, static_cast<size_t> (size) );
  m_Data[size] = '\0';
}
#endif /* __PROGTEST__ */

CMail::CMail(const CTimeStamp &timeStamp,const std::string &from,const CMailBody &body,CAttach *attach)
  : m_TimeStamp(timeStamp),
    m_From(from),
    m_Body(body),
    m_Attach(attach)
{
  if ( m_Attach )
    m_Attach -> addRef ();
}

CMail::CMail(const CMail &x)
  : m_TimeStamp(x.m_TimeStamp),
    m_From(x.m_From),
    m_Body(x.m_Body),
    m_Attach(x.m_Attach)
{
  if ( m_Attach )
    m_Attach -> addRef ();
}

CMail &CMail::operator=(const CMail &x)
{
  if ( this == &x )
    return *this;

  if ( m_Attach )
    m_Attach -> release ();

  m_TimeStamp = x . m_TimeStamp;
  m_From = x . m_From;
  m_Body = x . m_Body;
  m_Attach = x . m_Attach;

  if ( m_Attach )
    m_Attach -> addRef ();

  return *this;
}

CMail::~CMail()
{
  if ( m_Attach )
    m_Attach -> release ();
}

const std::string &CMail::from() const
{
  return m_From;
}

const CMailBody &CMail::body() const
{
  return m_Body;
}

const CTimeStamp &CMail::timeStamp() const
{
  return m_TimeStamp;
}

CAttach *CMail::attachment() const
{
  if ( m_Attach )
    m_Attach -> addRef ();
  return m_Attach;
}

std::ostream &operator<<(std::ostream &os,const CMail &x)
{
  os << x . m_TimeStamp << " " << x . m_From << " " << x . m_Body;
  if ( x . m_Attach )
    os << " + " << *x . m_Attach;
  return os;
}

CMailBox::CMailBox()
{
  m_Folders . emplace ( "inbox", std::multimap<CTimeStamp, CMail> () );
}

bool CMailBox::delivery(const CMail &mail)
{
  m_Folders["inbox"] . emplace ( mail . timeStamp (), mail );
  return true;
}

bool CMailBox::newFolder(const std::string &folderName)
{
  return m_Folders . emplace ( folderName, std::multimap<CTimeStamp, CMail> () ) . second;
}

bool CMailBox::moveMail(const std::string &fromFolder,const std::string &toFolder)
{
  auto fromIt = m_Folders . find ( fromFolder );
  auto toIt = m_Folders . find ( toFolder );

  if ( fromIt == m_Folders . end () || toIt == m_Folders . end () )
    return false;

  if ( fromIt == toIt )
    return true;

  toIt -> second . merge ( fromIt -> second );
  return true;
}

std::list<CMail> CMailBox::listMail(const std::string &folderName,const CTimeStamp &from,const CTimeStamp &to) const
{
  std::list<CMail> out;
  auto folderIt = m_Folders . find ( folderName );
  if ( folderIt == m_Folders . end () )
    return out;

  const auto & folder = folderIt -> second;
  auto beginIt = folder . lower_bound ( from );
  auto endIt = folder . upper_bound ( to );

  for ( auto it = beginIt; it != endIt; ++it )
    out . push_back ( it -> second );

  return out;
}

std::set<std::string> CMailBox::listAddr(const CTimeStamp &from,const CTimeStamp &to) const
{
  std::set<std::string> out;

  for ( const auto & folderPair : m_Folders )
  {
    const auto & folder = folderPair . second;
    auto beginIt = folder . lower_bound ( from );
    auto endIt = folder . upper_bound ( to );

    for ( auto it = beginIt; it != endIt; ++it )
      out . insert ( it -> second . from () );
  }

  return out;
}

#ifndef __PROGTEST__
static std::string showMail ( const std::list<CMail> & l )
{
  std::ostringstream oss;
  for ( const auto & x : l )
    oss << x << std::endl;
  return oss . str ();
}
static std::string showUsers ( const std::set<std::string> & s )
{
  std::ostringstream oss;
  for ( const auto & x : s )
    oss << x << std::endl;
  return oss . str ();
}
int main ()
{
  CAttach * att;
  std::ostringstream oss;

  att = new CAttach ( 100 );
  CMail testMail ( CTimeStamp ( 2026, 1, 2, 12, 5, 0 ), "test@domain.cz", CMailBody ( 10, "test, test" ), att );
  att -> release ();
  assert ( testMail . timeStamp () == CTimeStamp ( 2026, 1, 2, 12, 5, 0 ) );
  assert ( testMail . from () == "test@domain.cz" );
  att = testMail . attachment ();
  oss << *att;
  att -> release ();
  assert ( oss . str () == "attachment: 100 B" );
  assert ( showMail ( { testMail } ) == "2026-01-02 12:05:00 test@domain.cz mail body: 10 B + attachment: 100 B\n" );

  CMailBox m0;
  assert ( m0 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
  assert ( m0 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), nullptr ) ) );
  att = new CAttach ( 200 );
  assert ( m0 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), att ) ) );
  assert ( m0 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
  att -> release ();
  att = new CAttach ( 97 );
  assert ( m0 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), att ) ) );
  att -> release ();
  assert ( showMail ( m0 . listMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2024-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
                        "2024-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                        "2024-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
                        "2024-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n" );
  assert ( showMail ( m0 . listMail ( "inbox",
                      CTimeStamp ( 2024, 3, 31, 15, 26, 23 ),
                      CTimeStamp ( 2024, 3, 31, 16, 12, 48 ) ) ) == "2024-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                        "2024-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n" );
  assert ( showUsers ( m0 . listAddr ( CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                       CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "boss1@fit.cvut.cz\n"
                        "user1@fit.cvut.cz\n"
                        "user2@fit.cvut.cz\n" );
  assert ( showUsers ( m0 . listAddr ( CTimeStamp ( 2024, 3, 31, 15, 26, 23 ),
                       CTimeStamp ( 2024, 3, 31, 16, 12, 48 ) ) ) == "boss1@fit.cvut.cz\n"
                        "user2@fit.cvut.cz\n" );

  CMailBox m1;
  assert ( m1 . newFolder ( "work" ) );
  assert ( m1 . newFolder ( "spam" ) );
  assert ( !m1 . newFolder ( "spam" ) );
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
  att = new CAttach ( 500 );
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), att ) ) );
  att -> release ();
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), nullptr ) ) );
  att = new CAttach ( 468 );
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
  att -> release ();
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), nullptr ) ) );
  assert ( showMail ( m1 . listMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2024-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2024-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2024-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( showMail ( m1 . listMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( m1 . moveMail ( "inbox", "work" ) );
  assert ( showMail ( m1 . listMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( showMail ( m1 . listMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2024-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2024-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2024-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 19, 24, 13 ), "user2@fit.cvut.cz", CMailBody ( 14, "mail content 4" ), nullptr ) ) );
  att = new CAttach ( 234 );
  assert ( m1 . delivery ( CMail ( CTimeStamp ( 2024, 3, 31, 13, 26, 23 ), "user3@fit.cvut.cz", CMailBody ( 9, "complains" ), att ) ) );
  att -> release ();
  assert ( showMail ( m1 . listMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2024-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                        "2024-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );
  assert ( showMail ( m1 . listMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2024-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2024-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2024-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( m1 . moveMail ( "inbox", "work" ) );
  assert ( showMail ( m1 . listMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( showMail ( m1 . listMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2024-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                        "2024-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2024-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2024-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2024-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
                        "2024-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
