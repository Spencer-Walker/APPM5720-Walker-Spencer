// A simple one dimensional array class that can use arbitrary index range
#include <unistd.h>
#include <iostream>

#ifndef Complex_H
#define Complex_H
class Darray
{
  public:
    Darray( int nc, int xbeg, int xend, int ybeg, int yend);
    Darray( int nc, int xbeg, int xend);
    Darray(int xbeg, int xend);
    Darray();
    ~Darray() {if( m_data != 0 ) delete[] m_data;}
    void define( int nc, int xbeg, int xend,int ybeg,int yend);
    void define(int xbeg, int xend, int ybeg, int yend);
    inline bool in_range( int c, int i, int j)
    {return 1 <= c && c <= m_nc && m_xb <= i && i <= m_xe;}
// overload parenthesis operator 
    inline double& operator()( int c, int i, int j){
            // Turn on array bounds check 
#ifdef BZ_DEBUG  
        try
        {  
            if (!in_range(c,i,j)) throw 10;
        }
        catch(int e) 
        {
            std::cout <<
                "Error Index (c,i,j) = (" << c << "," <<
                i << "," << j << ") not in range 1<= c <= " <<
                m_nc << " " << m_xb << " <= i <= " << m_xe << " " 
                << m_yb <<  "<= j <= " << m_ye;
        }
#endif
       return m_data[m_base+m_offc*c+m_offx*i+m_offy*j];
    }
    
    inline double& operator()( int i,int j)
    {
        return operator()(1,i,j);
    }
    
    inline double& operator()( int i)
    {
        return operator()(1,i,m_yb);
    }



    inline bool is_defined(){return m_data != NULL;}
    static bool m_corder;
    int m_xb, m_xe, m_yb, m_ye;
    ssize_t m_base;
    size_t m_offx, m_offy, m_offc, m_npts;
    void define_offsets();
    void set_value( double scalar );
    void copy( const Darray& u );
    int m_nc, m_nx,m_ny;
  private:
   double* m_data;
};
#endif