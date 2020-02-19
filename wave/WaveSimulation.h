#ifndef __WAVESIMULATION_H_INCLUDED__   

#define __WAVESIMULATION_H_INCLUDED__   

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darrays.h"
#include <chrono> 
#include "silo.h"

using namespace std::chrono;
using namespace std;

class WaveSimulation
{
public:
        //// Constructor & destructor 
    WaveSimulation();
    ~WaveSimulation(){}
        // Number of points in space and time
    int m_Nx,m_Ny,m_Nt;
        // Geometry information
    double m_xB,m_xE,m_yB,m_yE,m_tend;
        // Solution arrays at three levels
    Darray2 m_u,m_up,m_um;
        // Arrays for right hand side and material
    Darray2 m_lap,m_f,m_c2;
        // Grids
    Darray1 m_x,m_y;
        // Discretization sizes
    double m_dt,m_hx,m_hy;
        // CFL Condition
    double m_CFL;
        // Time of the simulation
    double m_t;

        // Add some infrastructure to handle BC here...
        
    bool m_forced = false;
    bool m_check_energy = false;
    int m_ts_type;
    int m_n_print = -1; // Default is not to print, else print every m_n_print

#if 0        
    void print_solution();
    void print_solution(int cycle, double time);
#endif 
    void set_up_forcing(int k);    
    void set_up_material(int k);    
    void set_up_initial_data(int k);
    void compute_laplace();
    void set_boundary_conditions();
    void advance();
    void swap_times();
    void set_up_time_step();
    double compute_energy();
    double time_source();
    void set_time(double t){m_t = t;};
    void set_time_source(int k){m_ts_type = k;};    
    void set_n_print(int k){m_n_print = k;};
    int get_n_print(){return m_n_print;};
    
private:
    void set_up_arrays();
    void set_up_grids();
};

#endif
