#ifndef CHASKII_MATH_MATRIX4_DEF
#define CHASKII_MATH_MATRIX4_DEF 1

#include "vec3.h"
#include "vec4.h"
#include "matrix4d.h"
#include <string>

#define SET_ROW(row, v1, v2, v3, v4 )    \
    _mat[(row)][0] = (v1); \
    _mat[(row)][1] = (v2); \
    _mat[(row)][2] = (v3); \
    _mat[(row)][3] = (v4);

#define INNER_PRODUCT(a,b,r,c) \
     ((a)._mat[r][0] * (b)._mat[0][c]) \
    +((a)._mat[r][1] * (b)._mat[1][c]) \
    +((a)._mat[r][2] * (b)._mat[2][c]) \
    +((a)._mat[r][3] * (b)._mat[3][c])

namespace iiMath {

template <class T>
class  matrix4T
{
    public:
        T _mat[4][4];

        matrix4T() { makeIdentity(); }
        matrix4T(T const * const ptr ) { set(ptr); }

        matrix4T( T a00, T a01, T a02, T a03,
                T a10, T a11, T a12, T a13,
                T a20, T a21, T a22, T a23,
                T a30, T a31, T a32, T a33)
        {
                SET_ROW(0, a00, a01, a02, a03 )
                SET_ROW(1, a10, a11, a12, a13 )
                SET_ROW(2, a20, a21, a22, a23 )
                SET_ROW(3, a30, a31, a32, a33 )
        }

        void makeIdentity()
        {
                SET_ROW(0,    1, 0, 0, 0 )
                SET_ROW(1,    0, 1, 0, 0 )
                SET_ROW(2,    0, 0, 1, 0 )
                SET_ROW(3,    0, 0, 0, 1 )
        }

       void set( T a00, T a01, T a02, T a03,
                 T a10, T a11, T a12, T a13,
                 T a20, T a21, T a22, T a23,
                 T a30, T a31, T a32, T a33)
        {
                SET_ROW(0, a00, a01, a02, a03 )
                SET_ROW(1, a10, a11, a12, a13 )
                SET_ROW(2, a20, a21, a22, a23 )
                SET_ROW(3, a30, a31, a32, a33 )
        }

        void set(double const * const ptr )
        {
            T* local_ptr = (T*)_mat;
            for(int i=0;i<16;++i) local_ptr[i]=(T)ptr[i];
        }

        void set(float const * const ptr )
        {
            T* local_ptr = (T*)_mat;
            for(int i=0;i<16;++i) local_ptr[i]=(T)(ptr[i]);
        }

        template <class U>
        void copy( const matrix4T<U> &m )
        {
            for( int i = 0; i < 4; i++ )
                for( int j = 0; j < 4; j++ )
                    _mat[i][j] = (T)m._mat[i][j];
        }

        void copy( const matrix4D &m )
        {
            for( int i = 0; i < 4; i++ )
                for( int j = 0; j < 4; j++ )
                    _mat[i][j] = (T)m._mat[i][j];
        }

        T * ptr() { return (T*)&_mat[0][0]; }
        const T * ptr() const { return (const T *)&_mat[0][0]; }

        void preMult( const matrix4T& other )
        {
            T t[4];
            for(int col=0; col<4; ++col)
            {
                t[0] = INNER_PRODUCT( other, *this, 0, col );
                t[1] = INNER_PRODUCT( other, *this, 1, col );
                t[2] = INNER_PRODUCT( other, *this, 2, col );
                t[3] = INNER_PRODUCT( other, *this, 3, col );
                _mat[0][col] = t[0];
                _mat[1][col] = t[1];
                _mat[2][col] = t[2];
                _mat[3][col] = t[3];
            }
        }

        void postMult( const matrix4T &other )
        {
            T t[4];
            for(int row=0; row<4; ++row)
            {
                t[0] = INNER_PRODUCT( *this, other, row, 0 );
                t[1] = INNER_PRODUCT( *this, other, row, 1 );
                t[2] = INNER_PRODUCT( *this, other, row, 2 );
                t[3] = INNER_PRODUCT( *this, other, row, 3 );
                SET_ROW(row, t[0], t[1], t[2], t[3] )
            }
        }

        inline vec3T<T> preMult( const vec3T<T>& v ) const;
        inline vec4T<T> preMult( const vec4T<T>& v ) const;

        void makeFrustum( T left, T right, T bottom, T top, T zNear, T zFar )
        {
            double A = (right+left)/(right-left);
            double B = (top+bottom)/(top-bottom);
            double C = -(zFar+zNear)/(zFar-zNear);
            double D = -2.0*zFar*zNear/(zFar-zNear);
            SET_ROW(0, 2.0*zNear/(right-left),                    0.0, 0.0,  0.0 )
            SET_ROW(1,                    0.0, 2.0*zNear/(top-bottom), 0.0,  0.0 )
            SET_ROW(2,                      A,                      B,   C, -1.0 )
            SET_ROW(3,                    0.0,                    0.0,   D,  0.0 )
        }

        void makeLookat( const vec3T<T>& eye,
                         const vec3T<T>& center,
                         const vec3T<T>& up )
        {
            vec3T<T> F = center - eye;
            vec3T<T> UP = up;
            F.normalize();
            UP.normalize();

            vec3T<T> s = F ^ UP;
            s.normalize();
            vec3T<T> u = s ^ F;
            u.normalize();

            set( s[0], u[0], -F[0], 0,
                 s[1], u[1], -F[1], 0,
                 s[2], u[2], -F[2], 0,
                 0, 0, 0, 1
            );
            preMult(matrix4T::translate(-eye ));
        }

        void makeTranslate( T x, T y, T z )
        {
                SET_ROW(0,    1, 0, 0, 0 )
                SET_ROW(1,    0, 1, 0, 0 )
                SET_ROW(2,    0, 0, 1, 0 )
                SET_ROW(3,    x, y, z, 1 )
        }


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

            makeRotate( fv );
        }

        // Special case for quaternions
        void makeRotate( const T fv[4] )
        {
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
            _mat[3][0] = 0.0f;

            _mat[0][1] = xy + wz;
            _mat[1][1] = 1.0f - (xx + zz);
            _mat[2][1] = yz - wx;
            _mat[3][1] = 0.0f;

            _mat[0][2] = xz - wy;
            _mat[1][2] = yz + wx;
            _mat[2][2] = 1.0f - (xx + yy);
            _mat[3][2] = 0.0f;

            _mat[0][3] = 0;
            _mat[1][3] = 0;
            _mat[2][3] = 0;
            _mat[3][3] = 1;

    #undef QX
    #undef QY
    #undef QZ
    #undef QW
        }

        void makeScale( T x, T y, T z )
        {
            SET_ROW(0,    x, 0, 0, 0 )
            SET_ROW(1,    0, y, 0, 0 )
            SET_ROW(2,    0, 0, z, 0 )
            SET_ROW(3,    0, 0, 0, 1 )
        }

        void mult( const matrix4T &lhs, const matrix4T &rhs )
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
            _mat[0][3] = INNER_PRODUCT(lhs, rhs, 0, 3);
            _mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
            _mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
            _mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
            _mat[1][3] = INNER_PRODUCT(lhs, rhs, 1, 3);
            _mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
            _mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
            _mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
            _mat[2][3] = INNER_PRODUCT(lhs, rhs, 2, 3);
            _mat[3][0] = INNER_PRODUCT(lhs, rhs, 3, 0);
            _mat[3][1] = INNER_PRODUCT(lhs, rhs, 3, 1);
            _mat[3][2] = INNER_PRODUCT(lhs, rhs, 3, 2);
            _mat[3][3] = INNER_PRODUCT(lhs, rhs, 3, 3);
        }



        inline matrix4T operator * ( const matrix4T &m ) const
        {
            matrix4T r;
            r.mult(*this,m);
            return  r;
        }

        inline void operator *= ( const matrix4T& other )
        {
            if( this == &other )
            {
                matrix4T temp(other);
                postMult( temp );
            }
            else postMult( other );
        }

    #ifndef SGL_SWAP
    #define SGL_SWAP(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
    #endif

        bool invert()
        {
            return invert(*this);
        }

        bool invert( const matrix4T& mat )
        {
            if (&mat==this)
            {
               matrix4T tm(mat);
               return invert(tm);
            }

            unsigned int indxc[4], indxr[4], ipiv[4];
            unsigned int i,j,k,l,ll;
            unsigned int icol = 0;
            unsigned int irow = 0;
            T temp, pivinv, dum, big;

            // copy in place this may be unnecessary
            *this = mat;

            for (j=0; j<4; j++) ipiv[j]=0;

            for(i=0;i<4;i++)
            {
               big=(T)0.0;
               for (j=0; j<4; j++)
                  if (ipiv[j] != 1)
                     for (k=0; k<4; k++)
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
                  for (l=0; l<4; l++) SGL_SWAP(operator()(irow,l),
                                               operator()(icol,l),
                                               temp);

               indxr[i]=irow;
               indxc[i]=icol;
               if (operator()(icol,icol) == 0)
                  return false;

               pivinv = 1.0/operator()(icol,icol);
               operator()(icol,icol) = 1;
               for (l=0; l<4; l++) operator()(icol,l) *= pivinv;
               for (ll=0; ll<4; ll++)
                  if (ll != icol)
                  {
                     dum=operator()(ll,icol);
                     operator()(ll,icol) = 0;
                     for (l=0; l<4; l++) operator()(ll,l) -= operator()(icol,l)*dum;
                  }
            }
            for (int lx=4; lx>0; --lx)
            {
               if (indxr[lx-1] != indxc[lx-1])
                  for (k=0; k<4; k++) SGL_SWAP(operator()(k,indxr[lx-1]),
                                               operator()(k,indxc[lx-1]),temp);
            }

            return true;
        }

        void transpose()
        {
            transpose(*this);
        }

        void transpose( matrix4T& mat )
        {
            if (&mat==this)
            {
               matrix4T tm(mat);
               transpose(tm);
            }

            for( int i = 0; i < 4; i++ )
            {
                for( int j = 0; j < 4; j++ )
                {
                    if( i == j ) continue;
                    mat._mat[i][j] = mat._mat[j][i];
                }
            }
        }

        inline void decompose( iiMath::vec3T<T>& translation, iiMath::vec4T<T>& rotation,
                               iiMath::vec3T<T>& scale, iiMath::vec4T<T>& so );

        inline static matrix4T identity();
        inline static matrix4T translate( const vec3T<T> &);
        inline static matrix4T<T> translate( T x, T y, T z );
        inline static matrix4T rotate( T angle, T x, T y, T z);
        inline static matrix4T rotate(T angle, const vec3T<T>& axis );
        inline static matrix4T scale( T sx, T sy, T sz);
        inline static matrix4T scale( T s );

        inline T& operator()(int row, int col) { return _mat[row][col]; }
        inline T operator()(int row, int col) const { return _mat[row][col]; }

        int numcomponents() { return 16; }
        size_t typesize() { return sizeof(T); }

		std::string toString()
		{
			char buf[512];
			sprintf(buf, "%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
					_mat[0][0],_mat[0][1],_mat[0][2],_mat[0][3],
					_mat[1][0],_mat[1][1],_mat[1][2],_mat[1][3],
					_mat[2][0],_mat[2][1],_mat[2][2],_mat[2][3],
					_mat[3][0],_mat[3][1],_mat[3][2],_mat[3][3]);
			return buf ;
		}

    protected:

        inline T SGL_ABS(T a)
        {
           return (a >= 0.0f ? a : -a);
        }
};


template <class T>
inline matrix4T<T> matrix4T<T>::translate(const vec3T<T>& v )
{
    return translate(v.x(), v.y(), v.z() );
}


template <class T>
inline matrix4T<T> matrix4T<T>::translate( T tx, T ty, T tz)
{
    matrix4T m;
    m.makeTranslate(tx,ty,tz);
    return m;
}

template <class T>
inline matrix4T<T> matrix4T<T>::identity()
{
    matrix4T m;
    return m;
}

template <class T>
inline matrix4T<T> matrix4T<T>::rotate(T angle, T x, T y, T z )
{
    matrix4T m;
    m.makeRotate(angle,x,y,z);
    return m;
}

template <class T>
inline matrix4T<T> matrix4T<T>::rotate(T angle, const vec3T<T>& axis )
{
    return rotate(angle,axis.x(),axis.y(),axis.z());
}

template <class T>
inline matrix4T<T> matrix4T<T>::scale(T sx, T sy, T sz)
{
    matrix4T m;
    m.makeScale(sx,sy,sz);
    return m;
}

template <class T>
inline matrix4T<T> matrix4T<T>::scale(T s )
{
    matrix4T m;
    m.makeScale(s,s,s);
    return m;
}

template <class T>
inline vec3T<T> operator* (const vec3T<T>& v, const matrix4T<T>& m )
{
    return m.preMult(v);
}

template <class T>
inline vec4T<T> operator* (const vec4T<T>& v, const matrix4T<T>& m )
{
    return m.preMult(v);
}

template <class T>
inline vec3T<T> matrix4T<T>::preMult( const vec3T<T>& v ) const
{
    T d = 1.0f/(_mat[0][3]*v.x()+_mat[1][3]*v.y()+_mat[2][3]*v.z()+_mat[3][3]) ;

    return vec3T<T>( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0])*d,
                     (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1])*d,
                     (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2])*d);
}

template <class T>
inline vec4T<T> matrix4T<T>::preMult( const vec4T<T>& v ) const
{
    return vec4T<T>( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0]*v.w()),
                     (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1]*v.w()),
                     (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2]*v.w()),
                     (_mat[0][3]*v.x() + _mat[1][3]*v.y() + _mat[2][3]*v.z() + _mat[3][3]*v.w())
            );
}

#include <Chaskii/Math/matrixDecompose.h>

typedef matrix4T<short>             matrix4s;
typedef matrix4T<unsigned short>    matrix4us;
typedef matrix4T<int>               matrix4i;
typedef matrix4T<unsigned int>      matrix4ui;
typedef matrix4T<float>             matrix4f;
typedef matrix4T<double>            matrix4d;

//typedef matrix4D matrix4;

}

#undef SET_ROW
#undef INNER_PRODUCT

#endif

