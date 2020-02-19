#include "BoundaryConditions.h"

BoundaryConditions::BoundaryConditions(char left,char right,char bottom,char top,WaveSimulation &w_sim)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	
}

void BoundaryConditions::dirchlet_y(WaveSimulation &w_sim,int i,double g)
{
	for(int j=0;j<w_sim.m_Ny;j++)
	{
		w_sim.m_u(i,j) = g;
	}
}

void BoundaryConditions::dirchlet_x(WaveSimulation &w_sim,int j,double g)
{
	for(int i=0;i<w_sim.m_Nx;i++)
  {
    w_sim.m_u(i,j) = g;
  }
}

void BoundaryConditions::neumann_x(WaveSimulation &w_sim,int j,double g)
{
	if(j==0)
	{
  	for(int i=0;i<w_sim.m_Nx;i++)
  	{
			w_sim.m_u(i,j-1) = w_sim.m_u(i,j+1)-2.0*w_sim.m_hy*g;
 		}
	}
	else if(j == w_sim.m_Ny)
	{
    for(int i=0;i<w_sim.m_Nx;i++)
    {
      w_sim.m_u(i,j+1) = w_sim.m_u(i,j-1)+2.0*w_sim.m_hy*g;
    }
	}
}

void BoundaryConditions::neumann_y(WaveSimulation &w_sim,int i,double g)
{
  if(i==0)
  {
    for(int j=0;j<w_sim.m_Ny;j++)
    {
      w_sim.m_u(i-1,j) = w_sim.m_u(i-1,j)-2.0*w_sim.m_hx*g;
    }
  }
  else if(i == w_sim.m_Nx)
  {
    for(int j=0;j<w_sim.m_Ny;j++)
    {
      w_sim.m_u(i+1,j) = w_sim.m_u(i-1,j)+2.0*w_sim.m_hx*g;
    }
  }
}

void BoundaryConditions::non_reflecting_x(WaveSimulation &w_sim,int j,double g)
{
  if(j==0)
  {
    for(int i=0;i<w_sim.m_Nx;i++)
    {
      w_sim.m_u(i,-1) = w_sim.m_u(i,1) - sqrt(w_sim.m_c2(i,0))*2.0*w_sim.m_hy/w_sim.m_dt*
												(w_sim.m_u(i,0)-w_sim.m_um(i,0))+2.0*w_sim.m_hy*g;
    }
  }
  else if(j == w_sim.m_Ny)
  {
		for(int i=0;i<w_sim.m_Nx;i++)
		{	
			w_sim.m_u(i,j+1) = w_sim.m_u(i,j-1) - sqrt(w_sim.m_c2(i,j))*2.0*w_sim.m_hy/w_sim.m_dt*
                      (w_sim.m_u(i,j)-w_sim.m_um(i,j))-2.0*w_sim.m_hy*g;
  	}
	}
}

void BoundaryConditions::non_reflecting_y(WaveSimulation &w_sim,int i,double g)
{
  if(i==0)
  {
    for(int j=0;i<w_sim.m_Ny;j++)
    {
      w_sim.m_u(-1,j) = w_sim.m_u(1,j) - sqrt(w_sim.m_c2(0,j))*2.0*w_sim.m_hx/w_sim.m_dt*
                        (w_sim.m_u(0,j)-w_sim.m_um(0,j))+2.0*w_sim.m_hx*g;
    }
  }
  else if(i == w_sim.m_Ny)
  {
    for(int j=0;i<w_sim.m_Ny;j++)
    {
      w_sim.m_u(i+1,j) = w_sim.m_u(i-1,j) - sqrt(w_sim.m_c2(i,j))*2.0*w_sim.m_hx/w_sim.m_dt*
                      (w_sim.m_u(i,j)-w_sim.m_um(i,j))-2.0*w_sim.m_hx*g;
    }
  }
}

void BoundaryConditions::apply(WaveSimulation &w_sim,double gl,double gr,double gt,double gb)
{
	if(top == 'd')
	{
		dirchlet_x(w_sim,w_sim.m_Ny,gt);
	}
	else if(top = 'n')
	{
		neumann_x(w_sim,w_sim.m_Ny,gt);
	}
	else if(top = 'r')
	{
		non_reflecting_x(w_sim,w_sim.m_Ny,gt);
	}
	else
	{
		std::cout << "top boundary condition makes no sense" << std::endl;
	}


  if(bottom == 'd')
  {
    dirchlet_x(w_sim,0,gb);
  }
  else if(bottom = 'n')
  {
    neumann_x(w_sim,0,gb);
  }
  else if(bottom = 'r')
  {
    non_reflecting_x(w_sim,0,gb);
  }
  else
  {
    std::cout << "bottom boundary condition makes no sense" << std::endl;
  }

	if(right == 'd')
  {
    dirchlet_y(w_sim,w_sim.m_Nx,gr);
  }
  else if(right = 'n')
  {
    neumann_y(w_sim,w_sim.m_Nx,gr);
  }
  else if(right = 'r')
  {
    non_reflecting_y(w_sim,w_sim.m_Nx,gr);
  }
  else
  {
    std::cout << "right boundary condition makes no sense" << std::endl;
  }

  if(left == 'd')
  {
    dirchlet_y(w_sim,0,gl);
  }
  else if(left = 'n')
  {
    neumann_y(w_sim,0,gl);
  }
  else if(left = 'r')
  {
    non_reflecting_y(w_sim,0,gl);
  }
  else
  {
    std::cout << "left boundary condition makes no sense" << std::endl;
  }
}
