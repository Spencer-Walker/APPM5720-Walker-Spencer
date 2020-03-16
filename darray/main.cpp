#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darray.h"

using namespace std;

// Main program
int main()
{
    Darray u(2,-1,1);
    double a;
    for (int i = -1; i<2; i++)
        for (int c = 1; c<3; c++)
            u(c,i) = 1.;

    Darray v;
    v.copy(u);
       
    for (int i = -1; i<2; i++)
        for (int c = 1; c<3; c++)
            cout << " " << c << " " << i << " " << v(c,i) << "\n";
    return 0;
}

