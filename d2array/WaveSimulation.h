#include "Darray.h"
class WaveSimulation
{
  public:
    Darray grid;
    Darray x;
    Darray y;
    double hx;
    double hy;
    double nx;
    double ny;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    Darray ut;
    Darray u;
    Darray uxc2x;
    Darray uyc2y;
    Darray c2;

    WaveSimulation(double xmin,double xmax,int nx,double ymin,double ymax,int ny);
    ~WaveSimulation();
    void rhsy();
    void rhsx();
};