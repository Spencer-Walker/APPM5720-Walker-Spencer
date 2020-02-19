#ifndef __BoundaryConditions_H_INCLUDED__

#define __BoundaryConditions_H_INCLUDED__

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Darrays.h"
#include <chrono>
#include "silo.h"
#include "WaveSimulation.h"
using namespace std;

class BoundaryConditions
{
	public:
	char top;
  char bottom;
  char left;
  char right;


	BoundaryConditions(char left,char right,char bottom,char top,WaveSimulation &w_sim);

	void dirchlet_y(WaveSimulation &w_sim,int i,double g);

	void dirchlet_x(WaveSimulation &w_sim,int j,double g);

  void neumann_y(WaveSimulation &w_sim,int i,double g);

  void neumann_x(WaveSimulation &w_sim,int j,double g);

	void non_reflecting_x(WaveSimulation &w_sim,int j,double g);

	void non_reflecting_y(WaveSimulation &w_sim,int i,double g);

	void apply(WaveSimulation &w_sim,double gl,double gr,double gt,double gb);
	
	

};
#endif
