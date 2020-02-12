#include "WaveSimulation.h"
#include <math.h>
WaveSimulation::WaveSimulation(double xmin,double xmax,int nx,double ymin,double ymax,int ny)
{
  this->xmin = xmin;
  this->xmax = xmax;
  this->nx = nx;
  this->ymin = ymin;
  this->ymax = ymax;
  this->ny = ny;
  
  hx = (xmax-xmin)/(nx-1);
  hy = (ymax-ymin)/(ny-1);

  x.define(-1,nx,0,0);
  y.define(-1,ny,0,0);
  u.define(1,-1,nx,-1,ny);
  uxc2x.define(1,-1,nx,-1,ny);
  uyc2y.define(1,-1,nx,-1,ny);
  ut.define(1,-1,nx,-1,ny);
  c2.define(1,-1,nx,-1,ny);

  for(int i=-1;i<nx+1;i++)
  {
    x(i) = xmin + hx*i;    
  }
  for(int j=-1;j<ny+1;j++)
  {
    y(j) = ymin + hy*j;
  }

  for(int i = -1;i<nx+1;i++)
  {
    for(int j=-1;j<ny+1;j++)
    {
      u(i,j) = 0.0;
      uxc2x(i,j) = 0.0;
      uyc2y(i,j) = 0.0;
      ut(i,j) = 0.0;
      c2(i,j) = 0.0;
    }
  }

}

WaveSimulation::~WaveSimulation()
{

}

void WaveSimulation::rhsy()
{
  for(int j = 0; j<ny;j++)
  {
    for(int i = 0; i<nx;i++)
    {
      uyc2y(i,j) = (0.5/pow(hy,2))*((c2(i,j+1)+c2(i,j))*u(i,j+1)  
      -(c2(i,j+1)+2*c2(i,j)+c2(i,j-1))*u(i,j)+(c2(i,j)+c2(i,j-1))*u(i,j-1));
    }
  }
}

void WaveSimulation::rhsx()
{
  for(int j = 0; j<ny;j++)
  {
    for(int i = 0; i<nx;i++)
    {
      uxc2x(i,j) = (0.5/pow(hx,2))*((c2(i+1,j)+c2(i,j))*u(i+1,j)  
      -(c2(i+1,j)+2*c2(i,j)+c2(i-1,j))*u(i,j)+(c2(i,j)+c2(i-1,j))*u(i-1,j));
    }
  }
}

