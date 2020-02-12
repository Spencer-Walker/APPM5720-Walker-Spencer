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
Darray::Darray( int nc, int ibeg, int iend)
{
    m_nc = nc;      // number of rows (components)
    m_ib = ibeg;    // index of first column (physical index can be negative)
    m_ie = iend;    // index of last column (physical index )
    m_ni = m_ie-m_ib+1; // compute the number of columns 
    if( m_nc*m_ni > 0 ) // check to see if the number of rows/columns make sense
        m_data = new double[m_nc*m_ni]; // If it makes snse allocate rows*columns
    else
        m_data = NULL; // If it doesnt make sense set the pointer to null
    define_offsets();  // Set offsets such that we can address properly
}
/**
 * Same as above but assume only one component.
**/
Darray::Darray(int ibeg, int iend)
{
    m_nc = 1;           // number of rows (components).
    m_ib = ibeg;        // index of first column (physical).
    m_ie = iend;        // index of last column (physical).
    m_ni = m_ie-m_ib+1; // number of colums.
    if( m_nc*m_ni > 0 ) // Check to make the number of rows/columns make sense
        m_data = new double[m_nc*m_ni]; // Allocate a large 1D array with enough values
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
    m_nc = m_ib = m_ie = 0;
    m_data = NULL;
}

//
/**
 * Redefine everything basically the same as our constructor
**/
void Darray::define( int nc, int ibeg, int iend)
{
    //SAME AS BEFORE 
    if( m_data != NULL )
        delete[] m_data;

    m_nc = nc;
    m_ib = ibeg;
    m_ie = iend;
    m_ni = m_ie-m_ib+1;
    if( m_nc*m_ni > 0 )
        m_data = new double[m_nc*m_ni];
    else
        m_data = NULL;
    define_offsets();
}

/**
 * Redefine everything basically the same as our constructor
**/
void Darray::define(int ibeg, int iend)
{
    //SAME AS BEFORE 
    if( m_data != NULL )
        delete[] m_data;

    m_nc = 1;
    m_ib = ibeg;
    m_ie = iend;
    m_ni = m_ie-m_ib+1;
    if( m_nc*m_ni > 0 )
        m_data = new double[m_nc*m_ni];
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
    m_npts = static_cast<size_t>(m_ni)*m_nc;
    if( m_corder )
    {
            // (i,c)=i-ib+ni*(c-1)
        m_base = -m_ib-m_ni;
        m_offc = m_ni;
        m_offi = 1;
    }
    else
    {
            // (c,i)=c-1+nc*(i-ib)
        m_base = -1-m_nc*m_ib;
        m_offc = 1;
        m_offi = m_nc;
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
    m_ib = u.m_ib;
    m_ie = u.m_ie;
    m_ni = m_ie-m_ib+1;
    if( m_nc*m_ni > 0 )
    {
        m_data = new double[m_nc*m_ni];
        for( int i = 0 ; i < m_nc*m_ni ; i++ )
            m_data[i] = u.m_data[i];
    }
    else
        m_data = NULL;
    define_offsets();
}


