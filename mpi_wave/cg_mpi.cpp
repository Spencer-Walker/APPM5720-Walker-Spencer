#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono> 
#include "Darrays.h"
#include <mpi.h>

using namespace std::chrono;
using namespace std;

class MPI_info_Cart
{
public:
    MPI_info_Cart(int m,int n);
    MPI_info_Cart();
    ~MPI_info_Cart(){};
    int px,py,p_lin;
    int px_max,py_max,p_max=-1;
    int p_up,p_down,p_left,p_right;
    void set_procs(int nx, int ny);
};

MPI_info_Cart::MPI_info_Cart(int my_id, int nprocs){
    p_max = nprocs;
    p_lin = my_id;
}

void MPI_info_Cart::set_procs(int nx, int ny)
{
    double cost_tmp,cost;
    px_max = 1;
    py_max = p_max;
    cost = abs(nx/1.0-ny/(1.0*p_max));
    for (int p1 = 1; p1 < p_max; p1++)
    {
        int p2 = p_max/p1;
        if (p1*p2 == p_max){
            cost_tmp = abs(nx/(1.0*p1)-ny/(1.0*p2));
            if (cost_tmp < cost){
                cost = cost_tmp;
                px_max = p1; py_max = p2;
            }
        }
    }
    py = (p_lin/px_max)+1;
    px = 1 + p_lin - (py-1)*px_max;

    p_up    = (py==py_max) ? MPI_PROC_NULL : p_lin + px_max;
    p_down  = (py==1) ? MPI_PROC_NULL : p_lin - px_max;
    p_right = (px==px_max) ? MPI_PROC_NULL : p_lin + 1;
    p_left  = (px==1) ? MPI_PROC_NULL : p_lin - 1;
    if(p_lin == 0)
        std::cout << "px_max = " << px_max << ", py_max = " << py_max  << "\n";
}

int main(int argc, char** argv){

    int my_id, nprocs, ierr;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

// Problem setup
    
    double timing[100];
    timing[0] = MPI_Wtime();
    
    MPI_info_Cart mpi_info(my_id,nprocs);

    const double tol = 1.0e-5;
    
    const double Lx = 1.0;
    const double Ly = 1.0;
    int nx = 99;
    int ny = 99;
    int Nt = 10;
    int n_olp = 1;

    mpi_info.set_procs(nx,ny);
    
    double hx, hy;
    hx = Lx/(1.0*nx);
    hy = Ly/(1.0*ny);
    double dt;
    if(hx<hy)
    {
        dt = hx/10.0;
    }
    else
    {
        dt = hy/10.0;
    }
    
    double hxi2,hyi2,hi_center;

    hxi2 = 1.0/hx/hx;
    hyi2 = 1.0/hy/hy;
    hi_center = -(2.0/hx/hx+2.0/hy/hy);
        
    int nxl,nyl;
    int ix_off,iy_off;
        /**
           The global grids are 0,hx,2*hx,...,nx*hx = Lx
           The local arrays have interior indices 1,2,3,...nxl
           and n_olp overlapping points.
           
           Suppose the processes in the x direction has x values
           px = 1: x = 0,hx,2*hx...
           px = 2: x = 15*hx,...
           then ix_off is 0 and 15 etc. 
        **/

    nxl = (nx+1) / mpi_info.px_max;
    int rem = (nx+1 - nxl*mpi_info.px_max);
    if (mpi_info.px <= rem){
        nxl++;
        ix_off = (mpi_info.px-1)*nxl;
    }
    else{
        ix_off = (rem-1)*(nxl+1)+(mpi_info.px-rem)*nxl;
    }
        // Ditto for y
    nyl = (ny+1) / mpi_info.py_max;
    rem = (ny+1 - nyl*mpi_info.py_max);
    if (mpi_info.py <= rem){
        nyl++;
        iy_off = (mpi_info.py-1)*nyl;
    }
    else{
        iy_off = (rem-1)*(nyl+1)+(mpi_info.py-rem)*nyl;
    }

        // Loop indices over "unknowns" in Ax=b.
    int i_start = (mpi_info.px == 1) ? 2 : 1;
    int j_start = (mpi_info.py == 1) ? 2 : 1;
    int i_end = (mpi_info.px == mpi_info.px_max) ? nxl-1 : nxl;
    int j_end = (mpi_info.py == mpi_info.py_max) ? nyl-1 : nyl;
    char fName[100];    
    Darray2 r,b,q,d,u,x_sol,u_old,u_tmp;
        // u is for computing u_xx+u_yy so we need ghost points
    u.define(-(n_olp-1),nxl+n_olp,-(n_olp-1),nyl+n_olp);
    u_old.define(-(n_olp-1),nxl+n_olp,-(n_olp-1),nyl+n_olp);
    u_tmp.define(-(n_olp-1),nxl+n_olp,-(n_olp-1),nyl+n_olp);
        // CG variables NOTE that b is not used currently 
    r.define(i_start,i_end,j_start,j_end);
    b.define(i_start,i_end,j_start,j_end);
    q.define(i_start,i_end,j_start,j_end);
    d.define(i_start,i_end,j_start,j_end);
    x_sol.define(i_start,i_end,j_start,j_end);
    
    Darray2 x,y;
    x.define(-(n_olp-1),nxl+n_olp,-(n_olp-1),nyl+n_olp);
    y.define(-(n_olp-1),nxl+n_olp,-(n_olp-1),nyl+n_olp);
    
        // Add grids here. Use ix_off and iy_off?
    for (int j = -(n_olp-1); j <= nyl+n_olp; j++ )
        for (int i = -(n_olp-1); i <= nxl+n_olp; i++ ){
            x(i,j) = hx*(ix_off+(i-1));
            y(i,j) = hy*(iy_off+(j-1));
            u(i,j) = hx*(ix_off+(i-1))*(hx*(ix_off+(i-1))-1)*hy*(iy_off+(j-1))*(hy*(iy_off+(j-1))-1);
            u_old(i,j) = u(i,j);
        }

    sprintf(fName, "x%6.6i.txt", mpi_info.p_lin);
    x.writeToFile(fName,1,nxl,1,nyl);
    sprintf(fName, "y%6.6i.txt", mpi_info.p_lin);
    y.writeToFile(fName,1,nxl,1,nyl);

    MPI_Status status;
    int tag, mess_size;
    
    Darray2 x_ghost,y_ghost,x_recv,y_recv;
    x_ghost.define(1,n_olp,1,nyl);
    y_ghost.define(1,nxl,1,n_olp);
    x_recv.define(1,n_olp,1,nyl);
    y_recv.define(1,nxl,1,n_olp);
    x_recv.set_value(0.0);
    y_recv.set_value(0.0);
        // Pointers to the data as needed by MPI   
    double* x_ghost_ptr = x_ghost.c_ptr();
    double* x_recv_ptr = x_recv.c_ptr();
    double* y_ghost_ptr = y_ghost.c_ptr();
    double* y_recv_ptr = y_recv.c_ptr();

        // CG iteration starts here 
    int iter_cg = 0;
    
        // CG startup 
    r.set_value(0.0);
        // Start with zero guess (no commmunication needed)
    x_sol.set_value(0.0);

    
        // Fix boundary conditions
    if (mpi_info.px == 1)
        for (int j = 1; j <= nyl; j++)
            u(1,j) = 0.0;
    if (mpi_info.px == mpi_info.px_max)
        for (int j = 1; j <= nyl; j++)
            u(nxl,j) = 0.0;
    if (mpi_info.py == 1)
        for (int i = 1; i <= nxl; i++)
            u(i,1) = 0.0;
    if (mpi_info.py == mpi_info.py_max)
        for (int i = 1; i <= nxl; i++)
            u(i,nyl) = 0.0;

    double mpi_totaltime = 0;
    for(int it=0;it<Nt;it++)
    {
   
            // q <= Au
            // Before you can do the action of A onto u you need to communicate
            // and set BC.
            // Copy data on the left for sending to the left neigh. 
        for (int j = 1; j <= nyl; j++ )
            for (int i = 1; i <= n_olp; i++ )
                x_ghost(i,j) = u(i,j);
        tag = 21;
        mess_size = n_olp*nyl;

        timing[2] = MPI_Wtime();
        MPI_Send(x_ghost_ptr, mess_size, MPI_DOUBLE, mpi_info.p_left, tag, MPI_COMM_WORLD);
        MPI_Recv(x_recv_ptr, mess_size, MPI_DOUBLE, mpi_info.p_right, tag, MPI_COMM_WORLD,&status);
        timing[52] = MPI_Wtime();
        mpi_totaltime += timing[52] - timing[2];

            // Copy data back into the right part of u
        for (int j = 1; j <= nyl; j++ )
            for (int i = 1; i <= n_olp; i++ )
                u(nxl+i,j) = x_recv(i,j);
        
            // Copy data on the right for sending to the right neigh. 
        for (int j = 1; j <= nyl; j++ )
            for (int i = 1; i <= n_olp; i++ )
                x_ghost(i,j) = u(nxl-n_olp+i,j);
        tag = 22;
        mess_size = n_olp*nyl;

        timing[3] = MPI_Wtime();
        MPI_Send(x_ghost_ptr, mess_size, MPI_DOUBLE, mpi_info.p_right, tag, MPI_COMM_WORLD);
        MPI_Recv(x_recv_ptr, mess_size, MPI_DOUBLE, mpi_info.p_left, tag, MPI_COMM_WORLD,&status);
        timing[53] = MPI_Wtime();
        mpi_totaltime += timing[53] - timing[3];

            // Copy data back into the right part of u
        for (int j = 1; j <= nyl; j++ )
            for (int i = 1; i <= n_olp; i++ )
                u(-n_olp+i,j) = x_recv(i,j);
        
            // Copy data on the bottom for sending to the bottom neigh. 
        for (int j = 1; j <= n_olp; j++ )
            for (int i = 1; i <= nxl; i++ )
                y_ghost(i,j) = u(i,j);
        tag = 23;
        mess_size = n_olp*nxl;

        timing[4] = MPI_Wtime();
        MPI_Send(y_ghost_ptr, mess_size, MPI_DOUBLE, mpi_info.p_down, tag, MPI_COMM_WORLD);
        MPI_Recv(y_recv_ptr, mess_size, MPI_DOUBLE, mpi_info.p_up, tag, MPI_COMM_WORLD,&status);
        timing[54] = MPI_Wtime();
        mpi_totaltime += timing[54] - timing[4];

            // Copy data back into the right part of u
        for (int j = 1; j <= n_olp; j++ )
            for (int i = 1; i <= nxl; i++ )
                u(i,nyl+j) = y_recv(i,j);
        
            // Copy data on the top for sending to the top neigh. 
        for (int j = 1; j <= n_olp; j++ )
            for (int i = 1; i <= nxl; i++ )
                y_ghost(i,j) = u(i,nyl-n_olp+j);
        tag = 23;
        mess_size = n_olp*nxl;

        timing[5] = MPI_Wtime();
        MPI_Send(y_ghost_ptr, mess_size, MPI_DOUBLE, mpi_info.p_up, tag, MPI_COMM_WORLD);
        MPI_Recv(y_recv_ptr, mess_size, MPI_DOUBLE, mpi_info.p_down, tag, MPI_COMM_WORLD,&status);
        timing[55] = MPI_Wtime();
        mpi_totaltime += timing[55] - timing[5];

            // Copy data back into the right part of u
        for (int j = 1; j <= n_olp; j++ )
            for (int i = 1; i <= nxl; i++ )
                u(i,-n_olp+j) = y_recv(i,j);

            // Fix boundary conditions
        if (mpi_info.px == 1)
            for (int j = 1; j <= nyl; j++)
                u(1,j) = 0.0;
        if (mpi_info.px == mpi_info.px_max)
            for (int j = 1; j <= nyl; j++)
                u(nxl,j) = 0.0;
        if (mpi_info.py == 1)
            for (int i = 1; i <= nxl; i++)
                u(i,1) = 0.0;
        if (mpi_info.py == mpi_info.py_max)
            for (int i = 1; i <= nxl; i++)
                u(i,nyl) = 0.0;
        
            // Compute action of A at interior points,
            // NOTE that there is no forcing
        for (int j = j_start; j <= j_end; j++ )
            for (int i = i_start; i <= i_end; i++ ){
                u_tmp(i,j) = dt*dt*(hi_center*u(i,j)
                           +hxi2*u(i-1,j)
                           +hxi2*u(i+1,j)
                           +hyi2*u(i,j-1)
                           +hyi2*u(i,j+1))
                           +2.0*u(i,j)-u_old(i,j);
            }
        
        for (int j = j_start; j <= j_end; j++ )
            for (int i = i_start; i <= i_end; i++ ){
                u_old(i,j) = u(i,j);
                u(i,j) = u_tmp(i,j);
            }
    
        double local_norm2;
        std::cout << local_norm2 << std::endl;
        for (int j = j_start; j <= j_end; j++ )
            for (int i = i_start; i <= i_end; i++ )
                local_norm2 += u(i,j) * u(i,j);

        timing[6] = MPI_Wtime();
        MPI_Allreduce(MPI_IN_PLACE,&local_norm2,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

        if (mpi_info.p_lin == 0)
        {
            
            std::cout <<  " norm "
                      << pow(local_norm2,0.5) << "\n";
        }
    }
    
    
        // Print the final solution with boundary conditions updated
    for (int j = j_start; j <= j_end; j++ )
        for (int i = i_start; i <= i_end; i++ )
            u(i,j) = x_sol(i,j);
    sprintf(fName, "u%6.6i.txt", mpi_info.p_lin);
    u.writeToFile(fName,1,nxl,1,nyl);

    timing[50] = MPI_Wtime();
    if (mpi_info.p_lin == 0){
        std::cout << "Total timing [s] " << timing[50]-timing[0] << "\n";
        std::cout << "MPI timing [s] " << mpi_totaltime << "\n";
        std::cout << "Difference [s] " << timing[50]-timing[0] - mpi_totaltime  << "\n";
    }
    
    MPI_Finalize();
    return 0;
}

