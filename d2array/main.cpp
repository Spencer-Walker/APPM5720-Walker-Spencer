#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darray.h"
#include "WaveSimulation.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

// Main program
int main()
{
    // Make a 2 component array with physical indicies that range from -1 to 1.
    Darray u(2,-1,1,-3,0);
    double a;

    for (int j = -3; j<0; j++)
    {
        for (int i = -1; i<2; i++)
        {
            for (int c = 1; c<3; c++)
            {
                u(c,i,j) = 1.; // Set everything equal to 1.
#ifdef VERBOSE
                cout << "u(" << c << "," << i << "," << j << ") = " << u(c,i,j) << "\n";
#endif
            }
        }
    }
#ifdef VERBOSE
    for (int j = -3; j<0; j++)
    {
        for (int i = -1; i<2; i++)
        {
            cout << "u("  << i << ","<< j << ") = " << u(i,j) << "\n";
        }
    }
    for (int i = -1; i<2; i++)
    {
        cout << "u(" << i << ") = " << u(i) << "\n";
    }
#endif
    Darray v;  // Create some new Darray v
    v.copy(u); // Copy u into v
#ifdef VERBOSE       
    //Print everything from v
   for (int j = -3; j<0; j++)
    {
        for (int i = -1; i<2; i++)
        {
            for (int c = 1; c<3; c++)
            {
                cout << "v(" << c << "," << i << "," << j << ") = " << v(c,i,j) << "\n";
            }
        }
    }
#endif
    Darray w;
    for (int m = 5;m<=9;m++)
    {
#ifdef VERBOSE
        cout << "nx = " << pow(2,m) << endl;
#endif
        for (int n = 5; n<=9;n++)
        {
#ifdef VERBOSE 
            cout << "ny = " << pow(2,n) << endl;
#endif
            u.define(1,1,pow(2,m),1,pow(2,n));
            u.set_value(1.0);
            v.copy(u);
            w.copy(u);

#ifdef VERBOSE
            cout << "+ i outer j inner" << endl;
#endif
            auto start = high_resolution_clock::now();
            for (int i = 1; i<=pow(2,m); i++)
            {
                for (int j = 1; j<=pow(2,n);j++)
                {
                    w(i,j) = u(i,j) + v(i,j);
                }
            }
            auto stop = high_resolution_clock::now();
            auto duration =duration_cast<microseconds>(stop - start);             
#ifdef VERBOSE
            cout << "T = " << duration.count() << "ms" << endl;
            cout << "* i outer j inner" << endl;
#endif
            start = high_resolution_clock::now();
            for (int i = 1; i<=pow(2,m); i++)
            {
                for (int j = 1; j<=pow(2,n);j++)
                {
                    w(i,j) = u(i,j) * v(i,j);
                }
            }
            stop = high_resolution_clock::now();
            duration =duration_cast<microseconds>(stop - start);   
#ifdef VERBOSE
            cout << "T = " << duration.count() << "ms" << endl;
            cout << "+ j outer i inner" << endl;
#endif
            start = high_resolution_clock::now();
            for (int j = 1; j<=pow(2,n);j++)
            {
                for (int i = 1; i<=pow(2,m); i++)
                {
                    w(i,j) = u(i,j) + v(i,j);
                }
            }
            stop = high_resolution_clock::now();
            duration =duration_cast<microseconds>(stop - start);   
#ifdef VERBOSE
            cout << "T = " << duration.count() << "ms" << endl;
            cout << "* j outer i inner" << endl;
#endif
            start = high_resolution_clock::now();
            for (int j = 1; j<=pow(2,n);j++)
            {
                for (int i = 1; i<=pow(2,m); i++)
                {
                    w(i,j) = u(i,j)*v(i,j);
                }
            }
            stop = high_resolution_clock::now();
            duration =duration_cast<microseconds>(stop - start);   
#ifdef VERBOSE
            cout << "T = " << duration.count() << "ms" << endl;
#endif
        }
    }


    int nx = 100;
    int ny = 100;
    double xmin = -3.0/4.0;
    double xmax = (M_PI-3.0)/4.0;
    double ymin = (-M_PI/2.0-5.0)/3.0;
    double ymax = (M_PI/2.0-5.0)/3.0;
    double hx = (xmax-xmin)/(nx-1);
    double hy = (ymax-ymin)/(ny-1);

    WaveSimulation sim1(xmin,xmax,nx,ymin,ymax,ny);
    WaveSimulation sim2(xmin,xmax,nx,ymin,ymax,ny);
    Darray dxc2x1(1,-1,nx+1,-1,ny+1);
    Darray dyc2y1(1,-1,nx+1,-1,ny+1);
    Darray dxc2x2(1,-1,nx+1,-1,ny+1);
    Darray dyc2y2(1,-1,nx+1,-1,ny+1);

    double x;
    double y;
    for(int j=-1; j<ny+1; j++)
    {
        for(int i=-1;i<nx+1;i++)
        {   
            x = sim1.x(i);
            y = sim1.y(j);
            sim1.u(i,j) = sin(4.0*x+3.0)*cos(3.0*y+5.0);
            sim2.u(i,j) = sin(4.0*sim2.x(i)+3.0)*cos(3.0*sim2.y(j)+5.0);
            sim1.c2(i,j) = 1.0;    
            sim2.c2(i,j) = pow(1.0+0.3*sin(1.3*sim2.x(i)+1.2*y),2);
            dxc2x1(i,j) = -16.0*cos(5.0 + 3.0*y)*sin(3.0 + 4.0*x);  
            dyc2y1(i,j) = -9.0*cos(5.0 + 3.0*y)*sin(3.0 + 4.0*x);
            dxc2x2(i,j) = 3.12*cos(3 + 4*x)*cos(1.3*x + 1.2*y)*cos(5 + 3*y)*(1 + 0.3*sin(1.3*x + 1.2*y)) - 
                        16*cos(5 + 3*y)*sin(3 + 4*x)*pow(1 + 0.3*sin(1.3*x + 1.2*y),2);
            dyc2y2(i,j) = 3.12*cos(3 + 4*x)*cos(1.3*x + 1.2*y)*cos(5 + 3*y)*(1 + 0.3*sin(1.3*x + 1.2*y)) - 
                        16*cos(5 + 3*y)*sin(3 + 4*x)*pow(1 + 0.3*sin(1.3*x + 1.2*y),2);
        }
    }
    sim1.rhsx();
    sim1.rhsy();
    sim2.rhsx();
    sim2.rhsy();

#ifdef VERBOSE
    cout << "c2 = 1.0" << endl;
#endif
    double error2x1 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error2x1 += pow(sim1.uxc2x(i,j)-dxc2x1(i,j),2);
        }
    }
#ifdef VERBOSE
    cout << "L2x = " << sqrt(error2x1)*hx*hy << endl;
#endif
    double error1x1 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error1x1 += abs(sim1.uxc2x(i,j)-dxc2x1(i,j));
        }
    }
#ifdef VERBOSE
    cout << "L1x = " << (error1x1)*hx*hy << endl;
#endif
    double errorinfx1 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            if (errorinfx1 <abs(sim1.uxc2x(i,j)-dxc2x1(i,j)) )
            errorinfx1 = abs(sim1.uxc2x(i,j)-dxc2x1(i,j));
        }
    }
#ifdef VERBOSE
    cout << "Linfx = " << (errorinfx1)<< endl;
#endif

    double error2y1 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error2y1 += pow(sim1.uyc2y(i,j)-dyc2y1(i,j),2);
        }
    }
#ifdef VERBOSE
    cout << "L2y = " << sqrt(error2y1)*hx*hy << endl;
#endif
    double error1y1 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error1y1 += abs(sim1.uyc2y(i,j)-dyc2y1(i,j));
        }
    }
#ifdef VERBOSE
    cout << "L1y = " << (error1y1)*hx*hy << endl;
#endif
    double errorinfy1 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            if (errorinfy1 <abs(sim1.uyc2y(i,j)-dyc2y1(i,j)) )
            errorinfy1 = abs(sim1.uyc2y(i,j)-dyc2y1(i,j));
        }
    }
#ifdef VERBOSE
    cout << "Linfy = " << (errorinfy1)<< endl;
#endif
    double error2x2 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error2x2 += pow(sim2.uxc2x(i,j)-dxc2x2(i,j),2);
        }
    }
#ifdef VERBOSE
    cout << "c2 = pow(1.0+0.3*sin(1.3*sim2.x(i)+1.2*y),2)" << endl;
    cout << "L2x = " << sqrt(error2x2)*hx*hy << endl;
#endif
    double error1x2 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error1x2 += abs(sim2.uxc2x(i,j)-dxc2x2(i,j));
        }
    }
#ifdef VERBOSE
    cout << "L1x = " << (error1x2)*hx*hy << endl;
#endif
    double errorinfx2 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            if (errorinfx2 <abs(sim2.uxc2x(i,j)-dxc2x2(i,j)) )
            errorinfx2 = abs(sim2.uxc2x(i,j)-dxc2x2(i,j));
        }
    }
#ifdef VERBOSE
    cout << "Linfx = " << (errorinfx2)<< endl;
#endif

    double error2y2 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error2y2 += pow(sim2.uyc2y(i,j)-dyc2y2(i,j),2);
        }
    }
#ifdef VERBOSE
    cout << "L2y = " << sqrt(error2y2)*hx*hy << endl;
#endif
    double error1y2 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            error1y2 += abs(sim2.uyc2y(i,j)-dyc2y2(i,j));
        }
    }
#ifdef VERBOSE
    cout << "L1y = " << (error1y2)*hx*hy << endl;
#endif
    double errorinfy2 = 0.0;
    for(int j = 0; j<ny; j++)
    {
        for(int i = 0; i<nx; i++)
        {
            if (errorinfy2 <abs(sim2.uyc2y(i,j)-dyc2y2(i,j)) )
            errorinfy2 = abs(sim2.uyc2y(i,j)-dyc2y2(i,j));
        }
    }
#ifdef VERBOSE
    cout << "Linfy = " <<  (errorinfy2)<< endl;
#endif 
}



