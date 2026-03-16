#include <iostream>

using namespace std;


int & foo ( int b )
 {
   static int a = 60;

   a += b;  
   return a;
 } 



int main()
{
    int x = foo( 0 );
    int y = foo( 1 );
    cout << (x + y);
}