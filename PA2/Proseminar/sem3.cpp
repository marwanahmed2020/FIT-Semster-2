#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

constexpr size_t MAX_SIZE = 100;

// struct for the car 
struct Tcar
{
    string owner;
    string LP; // license plate
    unsigned int age ;

    // comparator for sorting by age it should be friend function of Tcar
    bool compareByAge(const Tcar& a, const Tcar& b)
    {
        return a.age < b.age; // sort by age
    }
};



// insert function (datebase, database size, string owner, string LP, unsigned age) pass by reference
{
    if(dbSize > MAX_SIZE)
    {
        return false;
    }

    db [ dbSize ].owner = owner;
    db [ dbSize ].LP = LP;
    db [ dbSize ++ ].age = age;

    return true;
}

// remove car function
void removeCar(Tcar db[], size_t& dbSize, const string& LP)
{
    for(size_t i = 0; i < dbSize; ++i)
    {
        if(db[i].LP == LP) // if license plate matches
        {
            // shift all elements after the removed car to the left
            for(size_t j = i; j < dbSize - 1; ++j)
            {
                db[j] = db[j + 1];
            }
            --dbSize; // decrease database size
            return;
        }
    }
}



// sort databse using std::sort
void sortDB(Tcar db[], size_t dbSize)
{
    sort(db, db + dbSize, [](const Tcar& a, const Tcar& b) { // labda function for sorting by owner age
        return a.age < b.age; // sort by age
    });
}



// print car 



// print database function
void printDB(const Tcar db[], size_t dbSize)
{    for(size_t i = 0; i < dbSize; ++i)
    {
        cout << "Owner: " << db[i].owner << ", License Plate: " << db[i].LP << ", Age: " << db[i].age << endl;
    }
}   

int main()
{
    Tcar db[MAX_SIZE];
    size_t dbSize ;
}