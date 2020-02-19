#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darrays.h"
#include <chrono> 

using namespace std::chrono;
using namespace std;

// Main program
// Timing W = alpha*U*V + beta*C
// The timing is for two loop orders and for the case when we extract a C pointer
// and use a blas routine
//

extern "C" void dgemm_( char *, char *, int *, int *, int *, double *, double *, int *, double *, int *, double *, double *, int * );

int main()
{
    Darray2 u,v,w;
    double* u_arr = u.c_ptr();
    double* v_arr = v.c_ptr();
    double* w_arr = w.c_ptr();
    int nmax = pow(2,10);
    int ntiming = pow(2,30);

    double alpha = 1.0;
    double beta = 0.0;
    char Transpose = 'N';

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
        v.set_value(1.0);
        w.set_value(0.0);
        
            // This is to make each size take roughly the same time
            // (if there were no memory effects).
        int n_loops = (ntiming / (n*n*n));
        int dofs = n*n*n*n_loops;
            // Start timing 
        register double s;        
        auto start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i1 = 1; i1 <= n; i1++)
                for (int i2 = 1; i2 <= n; i2++)
                {
                    s = 0.0;
                    for (int i3 = 1; i3 <= n; i3++)
                        s += u(i1,i3)*v(i3,i2);
                    w(i1,i2) = alpha*s;
                }
        
            // Stop timing and display averaged results
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        cout << right << setw(8) << n
             << right << setw(12) << dofs 
             << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << " ";

        w.set_value(0.0);
            // Another loop ordering
        start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++)
            for (int i2 = 1; i2 <= n; i2++)
                for (int i1 = 1; i1 <= n; i1++)
                {
                    s = 0.0;
                    for (int i3 = 1; i3 <= n; i3++)
                        s += u(i1,i3)*v(i3,i2);
                    w(i1,i2) = alpha*s;
                }
        
        stop = high_resolution_clock::now(); 
        duration = duration_cast<microseconds>(stop - start); 
        cout << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << " ";

        w.set_value(0.0);
            // Use blas routine
        start = high_resolution_clock::now();
        for(int j = 1 ; j <= n_loops; j++){
            dgemm_(&Transpose,&Transpose,&n,&n,&n,&alpha,u_arr,&n,v_arr,&n,&beta,w_arr,&n);
        }
       
        stop = high_resolution_clock::now(); 
        duration = duration_cast<microseconds>(stop - start); 
        cout << scientific << right << setw(14)
             << duration.count()/(1.0*dofs) << "\n";
        
    }

    cout << "\n\n";
    
    return 0;
}
