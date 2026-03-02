#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

// lets use vector instead of array for better memory management and dynamic resizing
#include <vector>

using namespace std;

constexpr size_t MAX_SIZE = 100;

// struct for the car 
class Tcar
{
    string owner;
    string LP; // license plate
    unsigned int age ;
public:
    // adding constructor for Tcar
    Tcar(const string& owner, const string& LP, unsigned int age) : owner(owner), LP(LP), age(age) {}

    bool isNewerthan(const Tcar& other) const
    {
        return age < other.age; // newer cars have smaller age
    }

    bool haslicenseplate(const string& plate) 
    {
        return this->LP == plate; // check if license plate matches
    }

    friend ostream& operator<<(ostream& os, const Tcar& car)
    {
        os << "Owner: " << car.owner << ", License Plate: " << car.LP << ", Age: " << car.age;
        return os;
    }
};

// insert function (database, string owner, string LP, unsigned age) pass by reference
// insert new with vector
bool insertCar(vector<Tcar>& db, const string& owner, const string& LP, unsigned int age)
{
    if(db.size() >= MAX_SIZE)
    {
        return false;
    }

    db.push_back(Tcar(owner, LP, age)); // add new car to the database

    return true;
}

// remove car function - updated for vector with back() and pop_back()
bool removeCar(vector<Tcar>& db, const string& LP)
{
    for(size_t i = 0; i < db.size(); ++i)
    {
        if(db[i].haslicenseplate(LP))
        {
            db[i] = db.back(); // replace with last element
            db.pop_back();     // remove last element
            return true;
        }
    }
    return false;
}

// sort database using std::sort
void sortDB(vector<Tcar>& db)
{
    sort(db.begin(), db.end(), [](const Tcar& a, const Tcar& b) {
        return a.isNewerthan(b); // sort by age (newer cars first)
    });
}

// print database function
void printDB(const vector<Tcar>& db)
{
    for(size_t i = 0; i < db.size(); ++i)
    {
        cout << db[i] << endl;
    }
}
int main()
{
    vector<Tcar> db;

    // Test 1: Insert some cars
    cout << "=== Test 1: Inserting Cars ===" << endl;
    insertCar(db, "Alice", "ABC123", 5);
    insertCar(db, "Bob", "XYZ789", 2);
    insertCar(db, "Charlie", "LMN456", 8);
    insertCar(db, "Diana", "PQR999", 1);
    cout << "Inserted 4 cars." << endl;
    printDB(db);
    cout << endl;

    // Test 2: Sort the database by age
    cout << "=== Test 2: Sorting by Age ===" << endl;
    sortDB(db);
    printDB(db);
    cout << endl;

    // Test 3: Remove a car
    cout << "=== Test 3: Removing Car with LP 'XYZ789' ===" << endl;
    if(removeCar(db, "XYZ789"))
    {
        cout << "Car removed successfully." << endl;
    }
    else
    {
        cout << "Car not found." << endl;
    }
    printDB(db);
    cout << endl;

    // Test 4: Try to remove non-existent car
    cout << "=== Test 4: Removing Non-existent Car ===" << endl;
    if(removeCar(db, "NOTFOUND"))
    {
        cout << "Car removed successfully." << endl;
    }
    else
    {
        cout << "Car not found." << endl;
    }
    cout << endl;

    // Test 5: Test isNewerthan method
    cout << "=== Test 5: Testing isNewerthan ===" << endl;
    if(db.size() >= 2)
    {
        cout << "Comparing first two cars:" << endl;
        cout << db[0] << endl;
        cout << db[1] << endl;
        if(db[0].isNewerthan(db[1]))
        {
            cout << "First car is newer than second car." << endl;
        }
        else
        {
            cout << "First car is NOT newer than second car." << endl;
        }
    }

    return 0;
}