#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <chrono> 
#include "silo.h"
#include "Darrays.h"
#include "WaveSimulation.h"
#include <omp.h>
using namespace std::chrono;
using namespace std;

int main(int argc, char **argv)
{
    auto start = high_resolution_clock::now(); 
    std::size_t pos;
    int n = std::stoi(argv[1], &pos);
    omp_set_num_threads(n);
    #pragma omp parallel
    {
        cout << "Num threads: " << omp_get_num_threads() << "\n";
    }
    // Create the simulation 
    WaveSimulation w_sim;
    double t = 0.0;
    
        // Setup of problem
        // These can also be read using cin at run time
    int initial_data = 0;
    int material_model = 2;
    int forcing = 1;
    int time_source = 1;
    
    w_sim.m_check_energy = true; // Will print to file and not stdout 
    ofstream energyfile;
    energyfile.open ("energy.out");
    ofstream timefile;
    timefile.open ("time.out");
    w_sim.m_tend = 10;
    w_sim.set_n_print(40);        
    
        // Call methods that set up problem
    w_sim.set_up_initial_data(initial_data);
    w_sim.set_up_material(material_model);
    w_sim.set_time_source(time_source);
    w_sim.set_up_forcing(forcing);

        // The timestep of the method must not break the physics
    w_sim.set_up_time_step();

// Make sure boundary conditions are set.
    w_sim.set_boundary_conditions();
// For checking energy
    double energy, energyold = 1.0;
    w_sim.print_solution(0,t);
    for(int it = 1; it<= w_sim.m_Nt; it++){
        t = (it-1)*w_sim.m_dt;
        w_sim.set_time(t);
        w_sim.compute_laplace();
        w_sim.advance();
        t = it*w_sim.m_dt;

            // Print every so often, always print last solution
        if((it % w_sim.get_n_print() == 0) || (it == w_sim.m_Nt)){
            w_sim.set_boundary_conditions();
            w_sim.print_solution(it,t);
        }
               
        if(w_sim.m_check_energy){
            energy = w_sim.compute_energy();
            
            energyfile << setprecision(16) << scientific << right << setw(22) << energy
                 << "\n";
            timefile << setprecision(16) << scientific << right << setw(22) << t 
                 << "\n";
    
            energyold = energy;
        }
            // Interchange the time levels
        w_sim.swap_times();
            // Set Boundaryy conditions
        w_sim.set_boundary_conditions();
    }
    energyfile.close(); 
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
    ofstream clocktimefile;
    clocktimefile.open ("clock_time.out");
    clocktimefile << duration.count();
    clocktimefile.close();
    timefile.close();
    return 0;
}
