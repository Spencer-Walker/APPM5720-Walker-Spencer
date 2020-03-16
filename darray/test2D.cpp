#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darrays.h"
#include <chrono> 

using namespace std::chrono;
using namespace std;

// Main program
// The first paert of the program times w = u + v for two dimensional arrays w,u,v.
// The timing is for two loop orders and for the case when we extract a C pointer
// and adress directly into the arrays  
//
// The second part of the program times the evaluation of the five point Laplacian
// for the two loop orderings.
//
//  Array bounds checking and other optimization can be turned on inside the makefile.

int main()
{
    Darray2 u,v,w;
    double* u_arr = u.c_ptr();
    double* v_arr = v.c_ptr();
    double* w_arr = w.c_ptr();
    int nmax = pow(2,13);
    int ntiming = pow(2,27);

    cout << "\n    n     number of op.      time1          time2          time3      \n";
    cout << "-----------------------------------------------------------------\n";    
    for (int n = 2 ; n <= nmax ; n *= 2)
    {
        u.define(1,n,1,n);
        v.define(1,n,1,n);
        w.define(1,n,1,n);

        u_arr = u.c_ptr();
        v_arr = v.c_ptr();
        w_arr = w.c_ptr();
        
        u.set_value(1.0);
        v.set_value(2.0);

            // This is to make each size take roughly the same time
            // (if there were no memory effects).
        int n_loops = (ntiming / (n*n));
        int dofs = n*n*n_loops;
            // Start timing 
        auto start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i1 = 1; i1 <= n; i1++)
                for (int i2 = 1; i2 <= n; i2++)
                    w(i1,i2) = v(i1,i2) + u(i1,i2);

            // Stop timing and display averaged results
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        cout << right << setw(8) << n
             << right << setw(12) << dofs 
             << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << " ";

            // Another loop ordering
        start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i2 = 1; i2 <= n; i2++)
                for (int i1 = 1; i1 <= n; i1++)
                    w(i1,i2) = v(i1,i2) + u(i1,i2);
        
        stop = high_resolution_clock::now(); 
        duration = duration_cast<microseconds>(stop - start); 
        cout << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << " ";

            // Direct access, maybe faster but hard to debug
            // for more complicated operations
        start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i1 = 0; i1 < u.npts(); i1++)
                w_arr[i1] = v_arr[i1] + u_arr[i1];
        
        stop = high_resolution_clock::now(); 
        duration = duration_cast<microseconds>(stop - start); 
        cout << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << "\n";
        
    }
    

        // Here we time the 5-point Laplacian for the two loop orderings
        //  
    
    cout << "\n    n     number of op.      time1          time2     \n";
    cout << "-----------------------------------------------------\n";         
    for (int n = 2 ; n <= nmax ; n *= 2)
    {
        int np = n+1;
        u.define(0,np,0,np); // Let u have a layer of ghost points
        w.define(1,n,1,n);
        
        u.set_value(1.0);

        double h2i = 1.0;
                
        int n_loops = (ntiming / (n*n));
        int dofs = n*n*n_loops;
        
        auto start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i1 = 1; i1 <= n; i1++)
                for (int i2 = 1; i2 <= n; i2++)
                    w(i1,i2) = h2i*(-4.0*u(i1,i2)
                                    +u(i1-1,i2)+u(i1+1,i2)+u(i1,i2-1)+u(i1,i2+1));
        
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        cout << right << setw(8) << n
             << right << setw(12) << dofs 
             << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << " ";

        start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i2 = 1; i2 <= n; i2++)
                for (int i1 = 1; i1 <= n; i1++)
                    w(i1,i2) = h2i*(-4.0*u(i1,i2)
                                    +u(i1-1,i2)+u(i1+1,i2)+u(i1,i2-1)+u(i1,i2+1));

        
        stop = high_resolution_clock::now(); 
        duration = duration_cast<microseconds>(stop - start); 
        cout << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << "\n";
       
    }

    cout << "\n\n";
    
    return 0;
}
