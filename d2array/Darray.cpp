#include <iostream>
#include "Darray.h"

using namespace std;

// Default value of array ordering
bool Darray::m_corder = false;

// Constructors
//-----------------------------------------------------------------------
/**
 * Constructor for arbituary physical indicies and some number of components 
 * at each physical indicies 
**/
Darray::Darray( int nc, int xbeg, int xend, int ybeg, int yend)
{
    m_nc = nc;      // number of rows (components)
    m_xb = xbeg;        // index of first column (physical).
    m_xe = xend;        // index of last column (physical).
    m_nx = m_xe-m_xb+1; // number of colums.
    m_yb = ybeg;
    m_ye = yend;
    m_ny = yend-ybeg+1;
    if( m_nc > 0 and m_nx > 0 and m_ny > 0 ) // Check to make the number of rows/columns make sense
        m_data = new double[m_nc*m_nx*m_ny]; // Allocate a large 1D array with enough values
    else
        m_data = NULL; // If the dimensions dont make sense set the pointer to null
    define_offsets();  // Set offsets such that we can address properly
}
/**
 * Constructor for arbituary physical indicies and some number of components 
 * at each physical indicies 
**/
Darray::Darray( int nc, int xbeg, int xend)
{
    m_nc = nc;      // number of rows (components)
    m_xb = xbeg;        // index of first column (physical).
    m_xe = xend;        // index of last column (physical).
    m_nx = m_xe-m_xb+1; // number of colums.
    m_yb = 0;
    m_ye = 0;
    m_ny = 1;
    if( m_nc > 0 and m_nx > 0 and m_ny > 0 ) // Check to make the number of rows/columns make sense
        m_data = new double[m_nc*m_nx*m_ny]; // Allocate a large 1D array with enough values
    else
        m_data = NULL; // If the dimensions dont make sense set the pointer to null
    define_offsets();  // Set offsets such that we can address properly
}
/**
 * Same as above but assume only one component.
**/
Darray::Darray(int xbeg, int xend)
{
    m_nc = 1;           // number of rows (components).
    m_xb = xbeg;        // index of first column (physical).
    m_xe = xend;        // index of last column (physical).
    m_nx = m_xe-m_xb+1; // number of colums.
    m_yb = 0;
    m_ye = 0;
    m_ny = 1;
    if( m_nc > 0 and m_nx > 0 and m_ny > 0 ) // Check to make the number of rows/columns make sense
        m_data = new double[m_nc*m_nx*m_ny]; // Allocate a large 1D array with enough values
    else
        m_data = NULL; // If the dimensions dont make sense set the pointer to null
    define_offsets();  // Set offsets such that we can address properly
}

//-----------------------------------------------------------------------

//Default constructor
/**
 * By default theres no date everything 0, pointer NULL
**/
Darray::Darray()
{
    m_nc = m_xb = m_xe = m_yb = m_ye = 0;
    m_data = NULL;
}

//
/**
 * Redefine everything basically the same as our constructor
**/
void Darray::define( int nc, int xbeg, int xend,int ybeg,int yend)
{
    //SAME AS BEFORE 
    if( m_data != NULL )
        delete[] m_data;

    m_nc = nc;
    m_xb = xbeg;
    m_xe = xend;
    m_yb = ybeg;
    m_ye = yend;
    m_nx = m_xe-m_xb+1;
    m_ny = m_ye-m_yb+1;
    if( m_nc > 0 and m_nx > 0 and m_ny > 0 )
        m_data = new double[m_nc*m_nx*m_ny];
    else
        m_data = NULL;
    define_offsets();
}

/**
 * Redefine everything basically the same as our constructor
**/
void Darray::define(int xbeg, int xend, int ybeg, int yend)
{
    //SAME AS BEFORE 
    if( m_data != NULL )
        delete[] m_data;

    m_nc = 1;
    m_xb = xbeg;
    m_xe = xend;
    m_yb = ybeg;
    m_ye = yend;
    m_nx = m_xe-m_xb+1;
    m_ny = m_ye-m_yb+1;
    if( m_nc > 0 and m_nx > 0 and m_ny > 0 )
        m_data = new double[m_nc*m_nx*m_ny];
    else
        m_data = NULL;
    define_offsets();
}



//-----------------------------------------------------------------------
/**
 * Set every component to some scalar.
**/
void Darray::set_value(double scalar)
{
    for( size_t i = 0 ; i < m_npts ; i++ )
        m_data[i] = scalar;
}

//-----------------------------------------------------------------------
/**
 * Set offsets (physical -> computer indicies)
**/ 
void Darray::define_offsets()
{
    m_npts = static_cast<size_t>(m_nx)*m_ny*m_nc;
    if( m_corder )
    {
        m_base = -m_yb-m_ny*m_xb-m_nx*m_ny;
        m_offc = m_nx*m_ny;
        m_offx = m_ny;
        m_offy = 1;
    }
    else
    {
        m_base = -1-m_nc*m_xb-m_nc*m_nx*m_yb;
        m_offc = 1;
        m_offx = m_nc;
        m_offy = m_nc*m_nx;
    }
}

//-----------------------------------------------------------------------
/**
 * Delete everything, allocate memeory and set fields to some other array.
**/
void Darray::copy( const Darray& u )
{
    if( m_data != NULL )
        delete[] m_data;
    
    m_nc = u.m_nc;
    m_xb = u.m_xb;
    m_xe = u.m_xe;
    m_nx = m_xe-m_xb+1;
    m_yb = u.m_yb;
    m_ye = u.m_ye;
    m_ny = m_ye-m_yb+1;
    if( m_nc > 0 and m_nx > 0 and m_ny > 0 )
    {
        m_data = new double[m_nc*m_nx*m_ny];
        for( int i = 0 ; i < m_nc*m_nx*m_ny ; i++ )
            m_data[i] = u.m_data[i];
    }
    else
        m_data = NULL;
    define_offsets();
}


