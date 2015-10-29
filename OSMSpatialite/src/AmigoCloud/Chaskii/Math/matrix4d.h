#ifndef CHASKII_MATH_MATRIX4D_DEF
#define CHASKII_MATH_MATRIX4D_DEF 1

#include "vec3d.h"
#include "vec4d.h"
#include <string>
#include <stdio.h>

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

//
class  matrix4D
{
    public:
        double _mat[4][4];

        matrix4D() { makeIdentity(); }

        matrix4D( const matrix4D& m) { set( &m._mat[0][0] ); }

        matrix4D(double const * const ptr ) { set(ptr); }

        matrix4D( double a00, double a01, double a02, double a03,
                double a10, double a11, double a12, double a13,
                double a20, double a21, double a22, double a23,
                double a30, double a31, double a32, double a33)
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

       void set( double a00, double a01, double a02, double a03,
                 double a10, double a11, double a12, double a13,
                 double a20, double a21, double a22, double a23,
                 double a30, double a31, double a32, double a33)
        {
                SET_ROW(0, a00, a01, a02, a03 )
                SET_ROW(1, a10, a11, a12, a13 )
                SET_ROW(2, a20, a21, a22, a23 )
                SET_ROW(3, a30, a31, a32, a33 )
        }

        void set(double const * const p )
        {
            SET_ROW(0, p[0], p[1], p[2], p[3] )
            SET_ROW(1, p[4], p[5], p[6], p[7] )
            SET_ROW(2, p[8], p[9], p[10], p[11] )
            SET_ROW(3, p[12], p[13], p[14], p[15] )
        }

        void set(float const * const p )
        {
            double* local_ptr = (double*)_mat;
            for(int i=0;i<16;++i) local_ptr[i]=(double)p[i];
        }

        void copy( const matrix4D &m )
        {
            for( int i = 0; i < 4; i++ )
                for( int j = 0; j < 4; j++ )
                    _mat[i][j] = (double)m._mat[i][j];
        }

        double * ptr() { return (double*)&_mat[0][0]; }
        const double * ptr() const { return (const double *)&_mat[0][0]; }

        void preMult( const matrix4D& other )
        {
            double t[4];
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

        void postMult( const matrix4D &other )
        {
            double t[4];
            for(int row=0; row<4; ++row)
            {
                t[0] = INNER_PRODUCT( *this, other, row, 0 );
                t[1] = INNER_PRODUCT( *this, other, row, 1 );
                t[2] = INNER_PRODUCT( *this, other, row, 2 );
                t[3] = INNER_PRODUCT( *this, other, row, 3 );
                SET_ROW(row, t[0], t[1], t[2], t[3] )
            }
        }

        inline vec3D preMult( const vec3D& v ) const;
        inline vec4D preMult( const vec4D& v ) const;

        void makeFrustum( double left, double right, double bottom, double top, double zNear, double zFar )
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

        void makeLookat( const vec3D& eye,
                         const vec3D& center,
                         const vec3D& up )
        {
            vec3D F = center - eye;
            vec3D UP = up;
            F.normalize();
            UP.normalize();

            vec3D s = F ^ UP;
            s.normalize();
            vec3D u = s ^ F;
            u.normalize();

            set( s[0], u[0], -F[0], 0,
                 s[1], u[1], -F[1], 0,
                 s[2], u[2], -F[2], 0,
                 0, 0, 0, 1
            );
            preMult(matrix4D::translate(-eye ));
        }

        void makeTranslate( double x, double y, double z )
        {
                SET_ROW(0,    1, 0, 0, 0 )
                SET_ROW(1,    0, 1, 0, 0 )
                SET_ROW(2,    0, 0, 1, 0 )
                SET_ROW(3,    x, y, z, 1 )
        }


        void makeRotate( double angle, double x, double y, double z )
        {
            double inversenorm  = 1.0/sqrt( x*x + y*y + z*z );
            double coshalfangle = cos( 0.5*angle );
            double sinhalfangle = sin( 0.5*angle );

            double fv[4];
            fv[0] = x * sinhalfangle * inversenorm;
            fv[1] = y * sinhalfangle * inversenorm;
            fv[2] = z * sinhalfangle * inversenorm;
            fv[3] = coshalfangle;

            makeRotate( fv );
        }

        // Special case for quaternions
        void makeRotate( const double fv[4] )
        {
            // Source: Gamasutra, Rotating Objects Using Quaternions
            //
            //http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
#define QX  fv[0]
#define QY  fv[1]
#define QZ  fv[2]
#define QW  fv[3]

            double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

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

        void makeScale( double x, double y, double z )
        {
            SET_ROW(0,    x, 0, 0, 0 )
            SET_ROW(1,    0, y, 0, 0 )
            SET_ROW(2,    0, 0, z, 0 )
            SET_ROW(3,    0, 0, 0, 1 )
        }

        void mult( const matrix4D &lhs, const matrix4D &rhs )
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



        inline matrix4D operator * ( const matrix4D &m ) const
        {
            matrix4D r;
            r.mult(*this,m);
            return  r;
        }

        inline void operator *= ( const matrix4D& other )
        {
            if( this == &other )
            {
                matrix4D temp(other);
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

        bool invert( const matrix4D& mat )
        {
            if (&mat==this)
            {
               matrix4D tm(mat);
               return invert(tm);
            }

            unsigned int indxc[4], indxr[4], ipiv[4];
            unsigned int i,j,k,l,ll;
            unsigned int icol = 0;
            unsigned int irow = 0;
            double temp, pivinv, dum, big;

            // copy in place this may be unnecessary
            *this = mat;

            for (j=0; j<4; j++) ipiv[j]=0;

            for(i=0;i<4;i++)
            {
               big=(double)0.0;
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

        void transpose( matrix4D& mat )
        {
            if (&mat==this)
            {
               matrix4D tm(mat);
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

        inline void decompose( iiMath::vec3D& translation, iiMath::vec4D& rotation,
                               iiMath::vec3D& scale, iiMath::vec4D& so );

        inline static matrix4D identity();
        inline static matrix4D translate( const vec3D &);
        inline static matrix4D translate( double x, double y, double z );
        inline static matrix4D rotate( double angle, double x, double y, double z);
        inline static matrix4D rotate(double angle, const vec3D& axis );
        inline static matrix4D scale( double sx, double sy, double sz);
        inline static matrix4D scale( double s );

        inline double& operator()(int row, int col) { return _mat[row][col]; }
        inline double operator()(int row, int col) const { return _mat[row][col]; }

        int numcomponents() { return 16; }
        size_t typesize() { return sizeof(double); }

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

        inline double SGL_ABS(double a)
        {
           return (a >= 0.0f ? a : -a);
        }
};



inline matrix4D matrix4D::translate(const vec3D& v )
{
    return translate(v.x(), v.y(), v.z() );
}



inline matrix4D matrix4D::translate( double tx, double ty, double tz)
{
    matrix4D m;
    m.makeTranslate(tx,ty,tz);
    return m;
}


inline matrix4D matrix4D::identity()
{
    matrix4D m;
    return m;
}


inline matrix4D matrix4D::rotate(double angle, double x, double y, double z )
{
    matrix4D m;
    m.makeRotate(angle,x,y,z);
    return m;
}


inline matrix4D matrix4D::rotate(double angle, const vec3D& axis )
{
    return rotate(angle,axis.x(),axis.y(),axis.z());
}


inline matrix4D matrix4D::scale(double sx, double sy, double sz)
{
    matrix4D m;
    m.makeScale(sx,sy,sz);
    return m;
}


inline matrix4D matrix4D::scale(double s )
{
    matrix4D m;
    m.makeScale(s,s,s);
    return m;
}


inline vec3D operator* (const vec3D& v, const matrix4D& m )
{
    return m.preMult(v);
}


inline vec4D operator* (const vec4D& v, const matrix4D& m )
{
    return m.preMult(v);
}


inline vec3D matrix4D::preMult( const vec3D& v ) const
{
    double d = 1.0f/(_mat[0][3]*v.x()+_mat[1][3]*v.y()+_mat[2][3]*v.z()+_mat[3][3]) ;

    return vec3D( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0])*d,
                     (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1])*d,
                     (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2])*d);
}


inline vec4D matrix4D::preMult( const vec4D& v ) const
{
    return vec4D( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0]*v.w()),
                     (_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1]*v.w()),
                     (_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2]*v.w()),
                     (_mat[0][3]*v.x() + _mat[1][3]*v.y() + _mat[2][3]*v.z() + _mat[3][3]*v.w())
            );
}

#include <Chaskii/Math/matrixDecomposeD.h>

//typedef matrix4D<short>             matrix4s;
//typedef matrix4D<unsigned short>    matrix4us;
//typedef matrix4D<int>               matrix4i;
//typedef matrix4D<unsigned int>      matrix4ui;
//typedef matrix4D<float>             matrix4f;
//typedef matrix4D<double>            matrix4d;
//
//typedef matrix4d matrix4;
typedef matrix4D matrix4;


}

#undef SET_ROW
#undef INNER_PRODUCT

#endif

