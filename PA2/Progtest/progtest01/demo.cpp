#include <iostream>

using namespace std;





int main()
{
   
    int i, * a = new int [67];

    for ( i = 0; i < 67; i ++ )
    a[i] = i;
    delete a; 
    cout << i;

}