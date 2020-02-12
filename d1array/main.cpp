#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darray.h"

using namespace std;

// Main program
int main()
{
    // Make a 2 component array with physical indicies that range from -1 to 1.
    Darray u(2,-1,1);
    double a;
    for (int i = -1; i<2; i++)
        for (int c = 1; c<3; c++)
            u(c,i) = 1.; // Set everything equal to 1.

    Darray v;  // Create some new Darray v
    v.copy(u); // Copy u into v
       
    //Print everything from v
    for (int i = -1; i<2; i++)
        for (int c = 1; c<3; c++)
            cout << "v(" << c << "," << i << ") = " << v(c,i) << "\n";
    return 0;
}

