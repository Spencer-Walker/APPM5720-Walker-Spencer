#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono> 
#include "silo.h"
#include "Darrays.h"
#include "WaveSimulation.h"

using namespace std::chrono;
using namespace std;

int main()
{
        // Create the simulation 
    WaveSimulation w_sim;
    double t = 0.0;
    
        // Setup of problem
        // These can also be read using cin at run time
    int initial_data = 0;
    int material_model = 2;
    int forcing = 1;
    int time_source = 1;
    
    w_sim.m_check_energy = false;        
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
    
		//w_sim.print_solution(0,t);
    for(int it = 1; it<= w_sim.m_Nt; it++){
        t = (it-1)*w_sim.m_dt;
        w_sim.set_time(t);
        w_sim.compute_laplace();
        w_sim.advance();
        t = it*w_sim.m_dt;

            // Print every so often, always print last solution
        if((it % w_sim.get_n_print() == 0) || (it == w_sim.m_Nt)){
            w_sim.set_boundary_conditions();
            //w_sim.print_solution(it,t);
        }
               
        if(w_sim.m_check_energy){
            energy = w_sim.compute_energy();
            cout <<"Energy at time " << setprecision(3) << t << " is: "
                 << setprecision(16) << scientific << right << setw(22) << energy
                 << " diff is: " << setprecision(2)
                 << scientific << right << setw(8)
                 << (energy - energyold)/energy 
                 << "\n";
            energyold = energy;
        }
            // Interchange the time levels
        w_sim.swap_times();
            // Set Boundaryy conditions
        w_sim.set_boundary_conditions();
    }
    
    return 0;
}
