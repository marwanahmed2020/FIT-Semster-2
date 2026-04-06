#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
#endif /* __PROGTEST__ */

class CVATRegister
{
public:
    CVATRegister();
    ~CVATRegister();
    bool newCompany(const std::string & name, const std::string & addr, const std::string & taxID);
    bool cancelCompany(const std::string & name, const std::string & addr);
    bool cancelCompany(const std::string & taxID);
    bool invoice(const std::string & taxID, unsigned int amount);
    bool invoice(const std::string & name, const std::string & addr, unsigned int amount);
    bool auditCompany(const std::string & name, const std::string & addr, unsigned int & sumIncome) const;
    bool auditCompany(const std::string & taxID, unsigned int & sumIncome) const;
    bool firstCompany(std::string & name, std::string & addr) const;
    bool nextCompany(std::string & name, std::string & addr) const;
    unsigned int medianInvoice() const;

private:
    struct Company
    {
        std::string name, addr, id;
        std::string nameLower, addrLower;
        unsigned int total_sum = 0;
    };

    std::vector<Company>       Companies;
    std::vector<size_t>        Compnaies_id;
    std::vector<size_t>        Compaines_NameAddr;

    // two heaps for O(log n) median
    // loHalf is max-heap holding the smaller half of invoices
    // hiHalf is min-heap holding the larger half of invoices
    // invariant: loHalf.size() == hiHalf.size()  (even count)
    //         or loHalf.size() == hiHalf.size() + 1  (odd count)
    // median is always loHalf.front() when odd, or hiHalf.front() when even
    std::vector<unsigned int>  loHalf; // max-heap
    std::vector<unsigned int>  hiHalf; // min-heap

    static std::string toLower(const std::string & s); // i will use for comparing case insesitive
    bool findById(const std::string & id, size_t & indexOut) const;
    bool findByNameAddr(const std::string & name, const std::string & addr, size_t & indexOut) const;
    bool removeCompanyAt(size_t index);
    void addInvoice(unsigned int amount); // helper to insert into heaps
};

// empty constructor
CVATRegister::CVATRegister()
{
}

// destructor
CVATRegister::~CVATRegister()
{
}

// toLower function for cpmaring name addr case insesitive like A = a
// so before comparing i will convert all the string to lower case
std::string CVATRegister::toLower(const std::string & s)
{
    std::string x = s;
    for ( char & c : x )
        c = tolower(c);
    return x;
}


// findById function for search by tax id
// here i do binary search and return company index
bool CVATRegister::findById(const std::string & id, size_t & indexOut) const
{
    // i do classic binary search on sorted id index vector
    // left right mean current search window
    size_t left = 0;
    size_t right = Compnaies_id.size();

    while ( left < right )
    {
        // middle element in current window
        size_t mid = left + (right - left) / 2;
        size_t index = Compnaies_id[mid];

        // if current id is smaller, target must be on right side
        if ( Companies[index].id < id )
            left = mid + 1;
        else
            // otherwise keep left part (also covers equal case)
            right = mid;
    }

    // if left reached end, id is not in database
    if ( left == Compnaies_id.size() )
        return false;

    // candidate is first place where value is not smaller
    size_t index = Compnaies_id[left];
    // if not exactly equal then id not found
    if ( Companies[index].id != id )
        return false;

    // found -> return real company index
    indexOut = index;
    return true;
}

// findByNameAddr function for search by name and addr
// i compare on lowercase values for case insesitive lookup
bool CVATRegister::findByNameAddr(const std::string & name, const std::string & addr, size_t & indexOut) const
{
    // i convert input to lower because compare should be case insesitive
    std::string n = toLower(name);
    std::string a = toLower(addr);

    // binary search on vector sorted by (nameLower, addrLower)
    size_t left = 0;
    size_t right = Compaines_NameAddr.size();

    while ( left < right )
    {
        // middle element in sorted name+addr index list
        size_t mid = left + (right - left) / 2;
        size_t index = Compaines_NameAddr[mid];

        // check if current pair is lexicographically smaller than target pair
        // first compare name, if same then compare addr
        bool isSmaller = false;
        if ( Companies[index].nameLower < n )
            isSmaller = true;
        else if ( Companies[index].nameLower == n && Companies[index].addrLower < a )
            isSmaller = true;

        // smaller means target is on the right
        if ( isSmaller )
            left = mid + 1;
        else
            // otherwise keep left side (includes equal)
            right = mid;
    }

    // not found if we moved to end
    if ( left == Compaines_NameAddr.size() )
        return false;

    // verify exact match, because lower_bound gives first not-smaller
    size_t index = Compaines_NameAddr[left];
    if ( Companies[index].nameLower != n )
        return false;
    if ( Companies[index].addrLower != a )
        return false;

    // found -> return real company index in Companies vector
    indexOut = index;
    return true;
}

// removeCompanyAt function for delete from all vectors
// after delete i fix shifted indexes in sorted arrays
bool CVATRegister::removeCompanyAt(size_t index)
{
    for ( size_t i = 0; i < Compnaies_id.size(); ++i )
    {
        if ( Compnaies_id[i] == index )
        {
            Compnaies_id.erase(Compnaies_id.begin() + i);
            break;
        }
    }

    for ( size_t i = 0; i < Compaines_NameAddr.size(); ++i )
    {
        if ( Compaines_NameAddr[i] == index )
        {
            Compaines_NameAddr.erase(Compaines_NameAddr.begin() + i);
            break;
        }
    }

    Companies.erase(Companies.begin() + index);

    for ( size_t & x : Compnaies_id )
        if ( x > index )
            --x;
    for ( size_t & x : Compaines_NameAddr )
        if ( x > index )
            --x;

    return true;
}

// newCompany function for adding new company record
// first i check uniqueness then insert in sorted helper vectors
bool CVATRegister::newCompany(const std::string & name, const std::string & addr, const std::string & taxID)
{
    size_t index = 0;
    if ( findById(taxID, index) )
        return false;
    if ( findByNameAddr(name, addr, index) )
        return false;

    Company c;
    c.name = name;
    c.addr = addr;
    c.id = taxID;
    c.nameLower = toLower(name);
    c.addrLower = toLower(addr);
    c.total_sum = 0;

    Companies.push_back(c);
    index = Companies.size() - 1;

    size_t posId = 0;
    while ( posId < Compnaies_id.size() && Companies[Compnaies_id[posId]].id < taxID )
        ++posId;
    Compnaies_id.insert(Compnaies_id.begin() + posId, index);

    size_t posName = 0;
    while ( posName < Compaines_NameAddr.size() )
    {
        size_t cur = Compaines_NameAddr[posName];
        if ( Companies[cur].nameLower < c.nameLower )
        {
            ++posName;
            continue;
        }
        if ( Companies[cur].nameLower > c.nameLower )
            break;
        if ( Companies[cur].addrLower < c.addrLower )
        {
            ++posName;
            continue;
        }
        break;
    }
    Compaines_NameAddr.insert(Compaines_NameAddr.begin() + posName, index);

    return true;
}

// cancelCompany function by name and addr
// if found i remove company from storage and indexes
bool CVATRegister::cancelCompany(const std::string & name, const std::string & addr)
{
    size_t index = 0;
    if ( !findByNameAddr(name, addr, index) )
        return false;
    return removeCompanyAt(index);
}

// cancelCompany function by id string
// same logic as other overload but search by tax id
bool CVATRegister::cancelCompany(const std::string & taxID)
{
    size_t index = 0;
    if ( !findById(taxID, index) )
        return false;
    return removeCompanyAt(index);
}

// invoice function by tax id
// i update company sum and store amount for median
bool CVATRegister::invoice(const std::string & taxID, unsigned int amount)
{
    size_t index = 0;
    if ( !findById(taxID, index) )
        return false;
    Companies[index].total_sum += amount;
    addInvoice(amount); // this i will use for median later
    return true;
}

// invoice function by name and addr
// same update but using case insesitive find helper
bool CVATRegister::invoice(const std::string & name, const std::string & addr, unsigned int amount)
{
    size_t index = 0;
    if ( !findByNameAddr(name, addr, index) )
        return false;
    Companies[index].total_sum += amount;
    addInvoice(amount); // this ill help me now here
    return true;
}

// auditCompany function by name and addr
// this returns current accumulated income for company
bool CVATRegister::auditCompany(const std::string & name, const std::string & addr, unsigned int & sumIncome) const
{
    size_t index = 0;
    if ( !findByNameAddr(name, addr, index) )
        return false;
    sumIncome = Companies[index].total_sum;
    return true;
}

// auditCompany function by tax id
// if not found return false and do not touch output sum
bool CVATRegister::auditCompany(const std::string & taxID, unsigned int & sumIncome) const
{
    size_t index = 0;
    if ( !findById(taxID, index) )
        return false;
    sumIncome = Companies[index].total_sum;
    return true;
}

// firstCompany function returns first in sorted order
// sorted order is by name then by addr
bool CVATRegister::firstCompany(std::string & name, std::string & addr) const
{
    if ( Compaines_NameAddr.empty() )
        return false;
    size_t index = Compaines_NameAddr[0];
    name = Companies[index].name;
    addr = Companies[index].addr;
    return true;
}

// nextCompany function gives next after current company
// i find current index then move one step in sorted vector
bool CVATRegister::nextCompany(std::string & name, std::string & addr) const
{
    size_t index = 0;
    if ( !findByNameAddr(name, addr, index) )
        return false;

    for ( size_t i = 0; i < Compaines_NameAddr.size(); ++i )
    {
        if ( Compaines_NameAddr[i] == index )
        {
            if ( i + 1 >= Compaines_NameAddr.size() )
                return false;
            size_t nextIdx = Compaines_NameAddr[i + 1];
            name = Companies[nextIdx].name;
            addr = Companies[nextIdx].addr;
            return true;
        }
    }

    return false;
}

// addInvoice helper inserts amount into the two heaps and rebalances
// loHalf is max-heap (lower half), hiHalf is min-heap (upper half)
// after insert loHalf always has equal or one more element than hiHalf
void CVATRegister::addInvoice(unsigned int amount)
{
    // push into correct half first
    if ( loHalf.empty() || amount <= loHalf.front() )
    {
        // amount belongs in lower half
        loHalf.push_back(amount);
        std::push_heap(loHalf.begin(), loHalf.end()); // max-heap default
    }
    else
    {
        // amount belongs in upper half
        hiHalf.push_back(amount);
        std::push_heap(hiHalf.begin(), hiHalf.end(), std::greater<unsigned int>()); // min-heap
    }

    // rebalance so sizes stay within one of each other
    if ( loHalf.size() > hiHalf.size() + 1 )
    {
        // move max of loHalf to hiHalf
        std::pop_heap(loHalf.begin(), loHalf.end());
        unsigned int val = loHalf.back();
        loHalf.pop_back();
        hiHalf.push_back(val);
        std::push_heap(hiHalf.begin(), hiHalf.end(), std::greater<unsigned int>());
    }
    else if ( hiHalf.size() > loHalf.size() )
    {
        // move min of hiHalf to loHalf
        std::pop_heap(hiHalf.begin(), hiHalf.end(), std::greater<unsigned int>());
        unsigned int val = hiHalf.back();
        hiHalf.pop_back();
        loHalf.push_back(val);
        std::push_heap(loHalf.begin(), loHalf.end());
    }
}

// medianInvoice function returns median from all valid invoices
// if count is even i return the bigger middle value
unsigned int CVATRegister::medianInvoice() const
{
    if ( loHalf.empty() )
        return 0;

    // total count = loHalf.size() + hiHalf.size()
    // when even: two middles are loHalf.front() and hiHalf.front()
    //            spec says return the bigger one -> hiHalf.front()
    // when odd:  loHalf has one more -> median is loHalf.front()
    size_t total = loHalf.size() + hiHalf.size();
    if ( total % 2 == 0 )
        return hiHalf.front(); // bigger middle when even
    return loHalf.front(); // single middle when odd
}

#ifndef __PROGTEST__
int main()
{
    std::string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . invoice ( "666/666", 2000 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "666/666/666", 3000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 4000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . auditCompany ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . auditCompany ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
    assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
    assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . cancelCompany ( "666/666" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . invoice ( "123456", 100 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 300 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 230 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 830 ) );
    assert ( b1 . medianInvoice () == 830 );
    assert ( b1 . invoice ( "123456", 1830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 3200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "123456" ) );
    assert ( ! b1 . firstCompany ( name, addr ) );

    CVATRegister b2;
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . medianInvoice () == 0 );
    assert ( b2 . invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . medianInvoice () == 1000 );
    assert ( b2 . invoice ( "abcdef", 2000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . invoice ( "1234567", 100 ) );
    assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . auditCompany ( "1234567", sumIncome ) );
    assert ( ! b2 . auditCompany ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . auditCompany ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . cancelCompany ( "1234567" ) );
    assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */