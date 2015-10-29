#ifndef CHASKII_MATH_MATRIX3_DEF
#define CHASKII_MATH_MATRIX3_DEF 1

#include "vec3.h"
#include "vec4.h"

#define SET_ROW(row, v1, v2, v3 )    \
    _mat[(row)][0] = (v1); \
    _mat[(row)][1] = (v2); \
    _mat[(row)][2] = (v3); 

#define INNER_PRODUCT(a,b,r,c) \
     ((a)._mat[r][0] * (b)._mat[0][c]) \
    +((a)._mat[r][1] * (b)._mat[1][c]) \
    +((a)._mat[r][2] * (b)._mat[2][c]) 

namespace iiMath {

template <class T>
class  matrix3T 
{
    public:
        T _mat[3][3];

        matrix3T() { makeIdentity(); }
        matrix3T(T const * const ptr ) { set(ptr); }

        matrix3T( T a00, T a01, T a02, 
                  T a10, T a11, T a12, 
                  T a20, T a21, T a22 )
        {
            SET_ROW(0, a00, a01, a02 )
            SET_ROW(1, a10, a11, a12 )
            SET_ROW(2, a20, a21, a22 )
        }

        void makeIdentity()
        {
            SET_ROW(0,    1, 0, 0 )
            SET_ROW(1,    0, 1, 0 )
            SET_ROW(2,    0, 0, 1 )
        }

       void set( T a00, T a01, T a02,
                 T a10, T a11, T a12,
                 T a20, T a21, T a22 )
        {
                SET_ROW(0, a00, a01, a02 )
                SET_ROW(1, a10, a11, a12 )
                SET_ROW(2, a20, a21, a22 )
        }

        void set(double const * const ptr )
        {
            T* local_ptr = (T*)_mat;
            for(int i=0;i<9;++i) local_ptr[i]=(T)ptr[i];
        }

        void set(float const * const ptr )
        {
            T* local_ptr = (T*)_mat;
            for(int i=0;i<9;++i) local_ptr[i]=(T)ptr[i];
        }

        template <class U>
        void copy( const matrix3T<U> &m )
        {
            for( int i = 0; i < 3; i++ )
                for( int j = 0; j < 3; j++ )
                    _mat[i][j] = (T)m._mat[i][j];
        }


        T * ptr() { return (T*)&_mat[0][0]; }
        const T * ptr() const { return (const T *)&_mat[0][0]; }

        void preMult( const matrix3T& other )
        {
            T t[3];
            for(int col=0; col<3; ++col) 
            {
                t[0] = INNER_PRODUCT( other, *this, 0, col );
                t[1] = INNER_PRODUCT( other, *this, 1, col );
                t[2] = INNER_PRODUCT( other, *this, 2, col );
                _mat[0][col] = t[0];
                _mat[1][col] = t[1];
                _mat[2][col] = t[2];
            }
        }   

        void postMult( const matrix3T &other )
        {
            T t[3];
            for(int row=0; row<4; ++row)
            {
                t[0] = INNER_PRODUCT( *this, other, row, 0 );
                t[1] = INNER_PRODUCT( *this, other, row, 1 );
                t[2] = INNER_PRODUCT( *this, other, row, 2 );
                SET_ROW(row, t[0], t[1], t[2] )
            }
        }

        inline vec3T<T> preMult( const vec3T<T>& v ) const;
        inline vec4T<T> preMult( const vec4T<T>& v ) const;

        void makeRotate( T angle, T x, T y, T z )
        {
            T inversenorm  = 1.0/sqrt( x*x + y*y + z*z );
            T coshalfangle = cos( 0.5*angle );
            T sinhalfangle = sin( 0.5*angle );

            T fv[4];
            fv[0] = x * sinhalfangle * inversenorm;
            fv[1] = y * sinhalfangle * inversenorm;
            fv[2] = z * sinhalfangle * inversenorm;
            fv[3] = coshalfangle;

            // Source: Gamasutra, Rotating Objects Using Quaternions
            //
            //http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
#define QX  fv[0]
#define QY  fv[1]
#define QZ  fv[2]
#define QW  fv[3]

            T wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

            // calculate coefficients
            x2 = QX + QX;
            y2 = QY + QY;
            z2 = QZ + QZ;

            xx = QX * x2;
            xy = QX * y2;
            xz = QX * z2;

            yy = QY * y2;
            yz = QY * z2;
            zz = QZ * z2;

            wx = QW * x2;
            wy = QW * y2;
            wz = QW * z2;

            // Note.  Gamasutra gets the matrix assignments inverted, resulting
            // in left-handed rotations, which is contrary to OpenGL and OSG's 
            // methodology.  The matrix assignment has been altered in the next
            // few lines of code to do the right thing.
            // Don Burns - Oct 13, 2001
            _mat[0][0] = 1.0f - (yy + zz);
            _mat[1][0] = xy - wz;
            _mat[2][0] = xz + wy;

            _mat[0][1] = xy + wz;
            _mat[1][1] = 1.0f - (xx + zz);
            _mat[2][1] = yz - wx;

            _mat[0][2] = xz - wy;
            _mat[1][2] = yz + wx;
            _mat[2][2] = 1.0f - (xx + yy);

    #undef QX 
    #undef QY 
    #undef QZ 
    #undef QW 
        }

        void makeScale( T x, T y, T z )
        {
            SET_ROW(0,    x, 0, 0 )
            SET_ROW(1,    0, y, 0 )
            SET_ROW(2,    0, 0, z )
        }

        void mult( const matrix3T &lhs, const matrix3T &rhs )
        {               
            if (&lhs==this)
            {
                postMult(rhs);
                return;
            }
            if (&rhs==this)
            {
                preMult(lhs);
                return;
            }

        // PRECONDITION: We assume neither &lhs nor &rhs == this
        // if it did, use preMult or postMult instead
            _mat[0][0] = INNER_PRODUCT(lhs, rhs, 0, 0);
            _mat[0][1] = INNER_PRODUCT(lhs, rhs, 0, 1);
            _mat[0][2] = INNER_PRODUCT(lhs, rhs, 0, 2);
            _mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
            _mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
            _mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
            _mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
            _mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
            _mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
        }



        inline matrix3T operator * ( const matrix3T &m ) const
        {
            matrix3T r;
            r.mult(*this,m);
            return  r;
        }

        inline void operator *= ( const matrix3T& other )
        {    
            if( this == &other ) 
            {
                matrix3T temp(other);
                postMult( temp );
            }
            else postMult( other );
        }


#if 0
        bool invert()
        {
            data[0][1] /= data[0]; // normalize row 0
            data[0][2] /= data[0]; // 

            for (int i = 1; i < 3; i++)
            {
                for (int j = i; j < 3; j++)  // do a column of L
                {
                    T sum = 0.0;
                    for( int k = 0; k < i; k++)
                        sum += data[j * maxsize + k] * data[k * maxsize + i];

                    data[j * maxsize + i] -= sum;
                }

                if (i == actualsize-1)
                    continue;

                for (int j= i + 1; j < actualsize; j++)  // do a row of U
                {
                    D sum = 0.0;
                    for (int k = 0; k < i; k++)
                        sum += data[i * maxsize + k]*data[k * maxsize + j];

                    data[i * maxsize + j] = (data[i * maxsize + j] - sum) / data[i * maxsize + i];
                }
            }

            for ( int i = 0; i < actualsize; i++ )  // invert L
            {
                for ( int j = i; j < actualsize; j++ )
                {
                    D x = 1.0;
                    if ( i != j )
                    {
                        x = 0.0;
                        for ( int k = i; k < j; k++ )
                            x -= data[j * maxsize + k] * data[k * maxsize + i];
                    }
                    data[j * maxsize + i] = x / data[j * maxsize + j];
                }
            }
    
            for ( int i = 0; i < actualsize; i++ )   // invert U
            {
                for ( int j = i; j < actualsize; j++ )
                {
                    if ( i == j )
                        continue;
                    D sum = 0.0;
    
                    for ( int k = i; k < j; k++ )
                        sum += data[k * maxsize + j]*( (i == k) ? 1.0 : data[i * maxsize + k] );
                    data[i * maxsize + j] = -sum;
                }
            }
            for ( int i = 0; i < actualsize; i++ )   // final inversion
            {
                for ( int j = 0; j < actualsize; j++ )
                {
                    D sum = 0.0;
                    for ( int k = ((i > j) ? i : j); k < actualsize; k++ )
                        sum += ( (j == k) ? 1.0 : data[j * maxsize + k]) * data[k * maxsize + i];
                    data[j * maxsize + i] = sum;
                }
            }
        }
#else
        bool invert()
        {
            return invert(*this);
        }

        bool invert( const matrix3T& mat )
        {
    #ifndef SGL_SWAP
    #define SGL_SWAP(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
    #endif
            if (&mat==this) 
            {
               matrix3T tm(mat);
               return invert(tm);
            }

            unsigned int indxc[3], indxr[3], ipiv[3];
            unsigned int i,j,k,l,ll;
            unsigned int icol = 0;
            unsigned int irow = 0;
            T temp, pivinv, dum, big;

            // copy in place this may be unnecessary
            *this = mat;

            for (j=0; j<3; j++) ipiv[j]=0;

            for(i=0;i<3;i++)
            {
               big=(T)0.0;
               for (j=0; j<3; j++)
                  if (ipiv[j] != 1)
                     for (k=0; k<3; k++)
                     {
                        if (ipiv[k] == 0)
                        {
                           if (SGL_ABS(operator()(j,k)) >= big)
                           {
                              big = SGL_ABS(operator()(j,k));
                              irow=j;
                              icol=k;
                           }
                        }
                        else if (ipiv[k] > 1)
                           return false;
                     }
               ++(ipiv[icol]);
               if (irow != icol)
                  for (l=0; l<3; l++) SGL_SWAP(operator()(irow,l),
                                               operator()(icol,l),
                                               temp);

               indxr[i]=irow;
               indxc[i]=icol;
               if (operator()(icol,icol) == 0)
                  return false;

               pivinv = 1.0/operator()(icol,icol);
               operator()(icol,icol) = 1;
               for (l=0; l<3; l++) operator()(icol,l) *= pivinv;
               for (ll=0; ll<3; ll++)
                  if (ll != icol)
                  {
                     dum=operator()(ll,icol);
                     operator()(ll,icol) = 0;
                     for (l=0; l<3; l++) operator()(ll,l) -= operator()(icol,l)*dum;
                  }
            }
            for (int lx=3; lx>0; --lx)
            {
               if (indxr[lx-1] != indxc[lx-1])
                  for (k=0; k<3; k++) SGL_SWAP(operator()(k,indxr[lx-1]),
                                               operator()(k,indxc[lx-1]),temp);
            }

            return true;
        }
#endif

        void transpose()
        {
            transpose(*this);
        }

        void transpose( matrix3T& mat )
        {
            if (&mat==this) 
            {
               matrix3T tm(mat);
               transpose(tm);
            }

            for( int i = 0; i < 3; i++ )
            {
                for( int j = 0; j < 3; j++ )
                {
                    if( i == j ) continue;
                    mat._mat[i][j] = mat._mat[j][i];
                }
            }
        }

        inline static matrix3T identity();
        inline static matrix3T rotate( T angle, T x, T y, T z);
        inline static matrix3T rotate(T angle, const vec3T<T>& axis );
        inline static matrix3T scale( T sx, T sy, T sz);
        inline static matrix3T scale( T s );

        inline T& operator()(int row, int col) { return _mat[row][col]; }
        inline T operator()(int row, int col) const { return _mat[row][col]; }

        int numcomponents() { return 9; }
        size_t typesize() { return sizeof(T); }

    protected:
    
        inline T SGL_ABS(T a)
        {
           return (a >= 0.0f ? a : -a);
        }
};


template <class T>
inline matrix3T<T> matrix3T<T>::identity()
{
    matrix3T m;
    return m;
}

template <class T>
inline matrix3T<T> matrix3T<T>::rotate(T angle, T x, T y, T z )
{               
    matrix3T m;
    m.makeRotate(angle,x,y,z);
    return m;
}

template <class T>
inline matrix3T<T> matrix3T<T>::rotate(T angle, const vec3T<T>& axis )
{
    return rotate(angle,axis.x(),axis.y(),axis.z());
}

template <class T>
inline matrix3T<T> matrix3T<T>::scale(T sx, T sy, T sz)
{       
    matrix3T m;
    m.makeScale(sx,sy,sz);
    return m;
}

template <class T>
inline matrix3T<T> matrix3T<T>::scale(T s )
{       
    matrix3T m;
    m.makeScale(s,s,s);
    return m;
}

template <class T>
inline vec3T<T> operator* (const vec3T<T>& v, const matrix3T<T>& m )
{
    return m.preMult(v);
}

template <class T>
inline vec3T<T> matrix3T<T>::preMult( const vec3T<T>& v ) const
{
    T d = 1.0f/(_mat[0][3]*v.x()+_mat[1][3]*v.y()+_mat[2][3]*v.z()) ;

    return vec3T<T>( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + d),
                     (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + d),
                     (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + d) );
}

template <class T>
inline vec4T<T> matrix3T<T>::preMult( const vec4T<T>& v ) const
{
    return vec4T<T>( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0]*v.w()),
                     (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1]*v.w()),
                     (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2]*v.w()),
                     1.0
            );
}

typedef matrix3T<short>             matrix3s;
typedef matrix3T<unsigned short>    matrix3us;
typedef matrix3T<int>               matrix3i;
typedef matrix3T<unsigned int>      matrix3ui;
typedef matrix3T<float>             matrix3f;
typedef matrix3T<double>            matrix3d;

typedef matrix3d matrix3;

}

#undef SET_ROW
#undef INNER_PRODUCT

#endif

