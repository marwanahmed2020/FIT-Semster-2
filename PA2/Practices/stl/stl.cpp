// sum input numbers
#include <iostream>
#include <numeric>
#include <iterator>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;


void oldSum()
{
    int sum = 0,x;
    while(cin>>x)
    {
        if(x==-1)
            break;
        sum+=x;
    }
    cout<<sum<<endl;
}

void newSum()
{
    // iterator and generic function
    int newSum = accumulate ( istream_iterator<int> (cin) , istream_iterator<int> (),0);
    cout<<newSum<<endl;
}

int multiply(int a, int b)
{
    return a*b;
} 

void accumulateorMulti()
{
    int x = accumulate(istream_iterator<int>(cin) ,istream_iterator<int> (),1,multiply);
    cout<<x<<endl;
}

// average = sum / count 
// lets get sum and conut 
pair<int,int> avg(const pair<int,int>& a, int b)
{
    return make_pair(a.first + b, a.second + 1);
}

void printAvg()
{
    pair<int,int> x = accumulate
    (
        istream_iterator<int> (cin),
        istream_iterator<int> (),
        make_pair(0,0),
        avg
    );

    cout<< x.first / x.second <<endl;
}

int maximum(int a, int b)
{
    return max(a,b);
}

int main()
{
    
    

    // Count frequency of words.
    // Input: cat dog cat cat dog
    // Output:
    // cat -> 3
    // dog -> 2
    map<string,int> freq;

    string word;
    while(cin>>word)
    {
        freq[word]++;
    }
    for(auto p : freq)
    {
        cout <<
        p.first
        <<"->"
        <<p.second
        <<endl;
    }


    return 0;
}