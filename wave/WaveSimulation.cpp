#include "WaveSimulation.h"

// constructor
WaveSimulation::WaveSimulation(){
    m_xB = -1.0;
    m_xE =  1.0;
    m_yB = -1.0;
    m_yE =  1.0;
    m_tend = 3.0;
    m_Nx = 400;
    m_Ny = 400;
    m_CFL = 0.3;
    set_up_arrays();
    set_up_grids();
};

double WaveSimulation::compute_energy()
{
    double e2 = 0.0;
    for(int j = 0 ; j <= m_Ny; j++)
        for(int i = 0 ; i <= m_Nx; i++)
            e2 += m_hx*m_hy*(pow((m_up(i,j)-m_u(i,j))/m_dt,2)
                             -m_up(i,j)*m_lap(i,j));
    return e2;
}

void WaveSimulation::advance(){
    double dt2 = m_dt*m_dt;
    if(!m_forced){
        for(int j = 0 ; j <= m_Ny; j++)
            for(int i = 0 ; i <= m_Nx; i++)
                m_up(i,j) = 2.0*m_u(i,j)-m_um(i,j) + dt2*m_lap(i,j);
    }
    else{
        double s = m_dt*m_dt*time_source();
        for(int j = 0 ; j <= m_Ny; j++)
            for(int i = 0 ; i <= m_Nx; i++)
                m_up(i,j) = 2.0*m_u(i,j)-m_um(i,j)
                    + dt2*m_lap(i,j) + s*m_f(i,j);
    }
};

void WaveSimulation::swap_times(){
    for(int j = 0 ; j <= m_Ny; j++)
        for(int i = 0 ; i <= m_Nx; i++){
            m_um(i,j) = m_u(i,j);
            m_u(i,j) = m_up(i,j);
        }
};

void WaveSimulation::set_boundary_conditions(){

        // Left 
    for(int j = 0 ; j <= m_Ny; j++){
        int i = 0;
        m_u(i,j) = 0.0;
    }
        // Right
    for(int j = 0 ; j <= m_Ny; j++){
        int i = m_Nx;
        m_u(i,j) = 0.0;
    }
        // Bottom
/*    for(int i = 0 ; i <= m_Nx; i++){
        int j = 0;
        m_u(i,j) = 0.0;
    }
*/
    for(int i = 0 ; i <= m_Nx; i++){
        m_u(i,-1) = m_u(i,1) - sqrt(m_c2(i,0))*2.0*m_hy/m_dt*(m_u(i,0)-m_um(i,0));
    }
        // Top
    for(int i = 0 ; i <= m_Nx; i++){
        int j = m_Ny+1;
        m_u(i,j) = m_u(i,j-2);
    }
};


void WaveSimulation::compute_laplace()
{
  
    double dx2i = 0.5/m_hx/m_hx;
    double dy2i = 0.5/m_hy/m_hy;
    
    for(int j = 0 ; j <= m_Ny; j++)
        for(int i = 0 ; i <= m_Nx; i++){
            m_lap(i,j) =
                dx2i*((m_c2(i+1,j)+m_c2(i,j))*m_u(i+1,j) 
                      - (m_c2(i+1,j) + 2.0*m_c2(i,j) + m_c2(i-1,j))*m_u(i,j)
                      + (m_c2(i,j) + m_c2(i-1,j))*m_u(i-1,j)) 
                + dy2i*((m_c2(i,j+1)+m_c2(i,j))*m_u(i,j+1) 
                        - (m_c2(i,j+1) + 2.0*m_c2(i,j) + m_c2(i,j-1))*m_u(i,j)
                        + (m_c2(i,j) + m_c2(i,j-1))*m_u(i,j-1)) ;
        }
}

void WaveSimulation::set_up_arrays(){

// Include ghost-points for solution, grids and speed of sound 
    
    m_u.define(-1,m_Nx+1,-1,m_Ny+1);
    m_u.set_value(0.0);
    m_c2.define(-1,m_Nx+1,-1,m_Ny+1);
    m_c2.set_value(1.0);

    m_x.define(-1,m_Nx+1);
    m_x.set_value(0.0);
    m_y.define(-1,m_Ny+1);
    m_y.set_value(0.0);
// No need to include ghost-points elsewhere;
    m_up.define(0,m_Nx,0,m_Ny);
    m_up.set_value(0.0);
    m_um.define(0,m_Nx,0,m_Ny);
    m_um.set_value(0.0);
    m_lap.define(0,m_Nx,0,m_Ny);
    m_lap.set_value(0.0);
    m_f.define(0,m_Nx,0,m_Ny);
    m_f.set_value(0.0);
}

void WaveSimulation::set_up_grids(){
    m_hx = (m_xE-m_xB)/m_Nx;
    m_hy = (m_yE-m_yB)/m_Ny;
    for(int i = -1; i<= m_Nx+1;i++)
        m_x(i) = m_xB + m_hx*i;
    for(int i = -1; i<= m_Ny+1;i++)
        m_y(i) = m_yB + m_hy*i;
}

void WaveSimulation::set_up_material(int k){
    if(k==1){
        for(int j = -1; j<= m_Ny+1;j++)
            for(int i = -1; i<= m_Nx+1;i++)
                m_c2(i,j) = 1.0 + 0.9*sin(3.0*m_x(i))*cos(2*m_y(j)+1.2);
    }
    else if(k==2){
        for(int j = -1; j<= m_Ny+1;j++)
            for(int i = -1; i<= m_Nx+1;i++)
                m_c2(i,j) = 1.0 + 0.9*sin(5.0*m_x(i))*cos(7.0*m_y(j)+1.2);
    }
    else{
        cout << "\nWarning material for case " << k << " not defined. Using c=1\n";
        m_c2.set_value(1.0);
    }
}

void WaveSimulation::set_up_forcing(int k){
    if ( k<=0 ){
        m_f.set_value(0.0);
    }
    else if (k == 1){
        m_forced = true;
        double sig = 144.0;
        for(int j = 0; j<= m_Ny;j++)
            for(int i = 0; i<= m_Nx;i++)
                m_f(i,j) = exp(-sig*(pow(m_x(i),2)+pow(m_y(j),2)));
    }
    else{
        cout << "\nWarning forcing for case " << k << " not defined. Using f=0\n";
        m_f.set_value(0.0);
    }
}

void WaveSimulation::set_up_time_step(){
    double cmax = 0.0;
    for(int j = -1; j<= m_Ny+1;j++)
        for(int i = -1; i<= m_Nx+1;i++)
            cmax = (m_c2(i,j) > cmax) ? m_c2(i,j) : cmax;
    cmax = sqrt(cmax);
        // Make sure we take an integer number of timesteps
    m_dt = m_CFL*min(m_hx,m_hy);
    m_Nt = int (m_tend/m_dt)+1;
    m_dt = m_tend/m_Nt;
}

void WaveSimulation::set_up_initial_data(int k){

    Darray2 ut(0,m_Nx,0,m_Ny);
    if(k==0){
        m_up.set_value(0.0);
        m_u.set_value(0.0);
        m_um.set_value(0.0);
    }
    else if(k==1){
        m_u.set_value(0.0);
        double y2;
        for(int j = 0; j<=m_Ny; j++){
            y2 = m_y(j)*m_y(j);
            for(int i = 0; i<= m_Nx; i++){
                m_up(i,j) = m_u(i,j) = exp(-36.0*(m_x(i)*m_x(i)+y2));
                ut(i,j) = 0.0;
                m_um(i,j) = m_u(i,j) - m_dt*ut(i,j);
            }
        }
    }
    else
    {
        cout << "\nWarning initial data for case " << k << " not defined. Using u=0, u_t = 0.\n";
        m_u.set_value(0.0);
        m_um.set_value(0.0);
    }
}

double WaveSimulation::time_source(){

    double s;
    if (m_ts_type == 1){
        double w = 35.0;
        s = 10.0*w*sin(m_t*w);
    }
    else if (m_ts_type == 2){
        double t0 = 0.5;
        double sig = 6.0/t0;
        s = 100.0*sig*sig*(m_t-t0)*exp(-pow(sig*(m_t-t0),2));
    }
    return s;
}
#if 0
void WaveSimulation::print_solution(){

    cout << "\n------------------------------\n";
    for (int j = 0; j<= m_Ny; j++){
        for (int i = 0; i<= m_Nx; i++)
            cout << m_u(i,j) << " ";
        cout << "\n";
    }
    cout << "------------------------------\n";
            
}
 
void WaveSimulation::print_solution(int cycle, double time){

    const char *coordnames[2]={"x", "y"};
    double    *coords[2];
    char      filename[80];
    int       dims[2];
    int       ndims;
    DBfile    *dbfile;
    DBoptlist *optList;
    
    ndims = 2;
    dims[0] = m_Nx + 1;
    dims[1] = m_Ny + 1;
    
    double* u_ptr = m_up.c_ptr();
    coords[0] = m_x.c_ptr();
    coords[1] = m_y.c_ptr();

    int driver = DB_PDB;
    
    sprintf(filename, "sol%.4d.silo", cycle);
    dbfile = DBCreate(filename, 0, DB_LOCAL, "Solution", driver);
    
    optList = DBMakeOptlist(10);
    DBAddOption(optList, DBOPT_DTIME, &time);
    DBAddOption(optList, DBOPT_CYCLE, &cycle);
    
    DBPutQuadmesh(dbfile, "quadmesh", (DBCAS_t) coordnames,
                  coords, dims, ndims, DB_DOUBLE, DB_COLLINEAR, optList);
    DBPutQuadvar1(dbfile, "solution", "quadmesh", u_ptr, dims, ndims, NULL,
                  0, DB_DOUBLE, DB_NODECENT, optList);
    DBFreeOptlist(optList);
    DBClose(dbfile);
}
#endif 
