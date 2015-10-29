#ifndef CHASKII_MATH_QUATD_DEF
#define CHASKII_MATH_QUATD_DEF 1

#include "vec3d.h"
#include "vec4d.h"
#include "matrix.h"

namespace iiMath {

/** A quaternion class. It can be used to represent an orientation in 3D space.*/

class quatD
{

    public:
        double  v[4];    // a four-vector

        inline quatD() { v[0]=0.0; v[1]=0.0; v[2]=0.0; v[3]=1.0; }

        inline quatD( double x, double y, double z, double w )
        {
            v[0]=x;
            v[1]=y;
            v[2]=z;
            v[3]=w;
        }

        inline quatD( const quatD& q )
        {
            v[0]=q.x();
            v[1]=q.y();
            v[2]=q.z();
            v[3]=q.w();
        }

        inline quatD( const vec4D& vv )
        {
            v[0]=vv.x();
            v[1]=vv.y();
            v[2]=vv.z();
            v[3]=vv.w();
        }


        inline quatD( double angle, const vec3D& axis)
        {
            makeRotate(angle,axis);
        }

        inline quatD( double angle1, const vec3D& axis1,
                      double angle2, const vec3D& axis2,
                      double angle3, const vec3D& axis3)
        {
            makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
        }

        inline quatD& operator = (const quatD& q) { v[0]=q.v[0];  v[1]=q.v[1]; v[2]=q.v[2]; v[3]=q.v[3]; return *this; }

        inline bool operator == (const quatD& v) const { return v[0]==v.v[0] && v[1]==v.v[1] && v[2]==v.v[2] && v[3]==v.v[3]; }

        inline bool operator != (const quatD& v) const { return v[0]!=v.v[0] || v[1]!=v.v[1] || v[2]!=v.v[2] || v[3]!=v.v[3]; }

        inline bool operator <  (const quatD& v) const
        {
            if (v[0]<v.v[0]) return true;
            else if (v[0]>v.v[0]) return false;
            else if (v[1]<v.v[1]) return true;
            else if (v[1]>v.v[1]) return false;
            else if (v[2]<v.v[2]) return true;
            else if (v[2]>v.v[2]) return false;
            else return (v[3]<v.v[3]);
        }

        /* ----------------------------------
           Methods to access data members
        ---------------------------------- */

        inline vec4D asVec4() const
        {
            return vec4D(v[0], v[1], v[2], v[3]);
        }

        inline vec3D asVec3() const
        {
            return vec3D(v[0], v[1], v[2]);
        }

        inline void set(double x, double y, double z, double w)
        {
            v[0]=x;
            v[1]=y;
            v[2]=z;
            v[3]=w;
        }

        inline void set(const vec4D& vv)
        {
            v[0]=vv.x();
            v[1]=vv.y();
            v[2]=vv.z();
            v[3]=vv.w();
        }

        void set(const matrix4& matrix)
        {
            *this = getRotate( matrix );
        }

//        void set(matrix4& matrix)
//        {
//            *this = getRotate( matrix );
//        }

        void get(matrix4& matrix) const
        {
        	//matrix.makeRotate(*this);
        	matrix.makeRotate(v);
        }

        inline double & operator [] (int i) { return v[i]; }
        inline double   operator [] (int i) const { return v[i]; }

        inline double & x() { return v[0]; }
        inline double & y() { return v[1]; }
        inline double & z() { return v[2]; }
        inline double & w() { return v[3]; }

        inline double x() const { return v[0]; }
        inline double y() const { return v[1]; }
        inline double z() const { return v[2]; }
        inline double w() const { return v[3]; }

        /** return true if the Quat represents a zero rotation, and therefore can be ignored in computations.*/
        inline bool zeroRotation() const { return v[0]==0.0 && v[1]==0.0 && v[2]==0.0 && v[3]==1.0; }


         /* -------------------------------------------------------------
                   BASIC ARITHMETIC METHODS
        Implemented in terms of Vec4s.  Some Vec4 operators, e.g.
        operator* are not appropriate for quaternions (as
        mathematical objects) so they are implemented differently.
        Also define methods for conjugate and the multiplicative inverse.
        ------------------------------------------------------------- */
        /// Multiply by scalar
        inline const quatD operator * (double rhs) const
        {
            return quatD(v[0]*rhs, v[1]*rhs, v[2]*rhs, v[3]*rhs);
        }

        /// Unary multiply by scalar
        inline quatD& operator *= (double rhs)
        {
            v[0]*=rhs;
            v[1]*=rhs;
            v[2]*=rhs;
            v[3]*=rhs;
            return *this;        // enable nesting
        }

        /// Binary multiply
        inline const quatD operator*(const quatD& rhs) const
        {
            return quatD( rhs.v[3]*v[0] + rhs.v[0]*v[3] + rhs.v[1]*v[2] - rhs.v[2]*v[1],
                 rhs.v[3]*v[1] - rhs.v[0]*v[2] + rhs.v[1]*v[3] + rhs.v[2]*v[0],
                 rhs.v[3]*v[2] + rhs.v[0]*v[1] - rhs.v[1]*v[0] + rhs.v[2]*v[3],
                 rhs.v[3]*v[3] - rhs.v[0]*v[0] - rhs.v[1]*v[1] - rhs.v[2]*v[2] );
        }

        /// Unary multiply
        inline quatD& operator*=(const quatD& rhs)
        {
            double x = rhs.v[3]*v[0] + rhs.v[0]*v[3] + rhs.v[1]*v[2] - rhs.v[2]*v[1];
            double y = rhs.v[3]*v[1] - rhs.v[0]*v[2] + rhs.v[1]*v[3] + rhs.v[2]*v[0];
            double z = rhs.v[3]*v[2] + rhs.v[0]*v[1] - rhs.v[1]*v[0] + rhs.v[2]*v[3];
            v[3]   = rhs.v[3]*v[3] - rhs.v[0]*v[0] - rhs.v[1]*v[1] - rhs.v[2]*v[2];

            v[2] = z;
            v[1] = y;
            v[0] = x;

            return (*this);            // enable nesting
        }

        /// Divide by scalar
        inline quatD operator / (double rhs) const
        {
            double div = 1.0/rhs;
            return quatD(v[0]*div, v[1]*div, v[2]*div, v[3]*div);
        }

        /// Unary divide by scalar
        inline quatD& operator /= (double rhs)
        {
            double div = 1.0/rhs;
            v[0]*=div;
            v[1]*=div;
            v[2]*=div;
            v[3]*=div;
            return *this;
        }

        /// Binary divide
        inline const quatD operator/(const quatD& denom) const
        {
            return ( (*this) * denom.inverse() );
        }

        /// Unary divide
        inline quatD& operator/=(const quatD& denom)
        {
            (*this) = (*this) * denom.inverse();
            return (*this);            // enable nesting
        }

        /// Binary addition
        inline const quatD operator + (const quatD& rhs) const
        {
            return quatD(v[0]+rhs.v[0], v[1]+rhs.v[1],
                v[2]+rhs.v[2], v[3]+rhs.v[3]);
        }

        /// Unary addition
        inline quatD& operator += (const quatD& rhs)
        {
            v[0] += rhs.v[0];
            v[1] += rhs.v[1];
            v[2] += rhs.v[2];
            v[3] += rhs.v[3];
            return *this;            // enable nesting
        }

        /// Binary subtraction
        inline const quatD operator - (const quatD& rhs) const
        {
            return quatD(v[0]-rhs.v[0], v[1]-rhs.v[1],
                v[2]-rhs.v[2], v[3]-rhs.v[3] );
        }

        /// Unary subtraction
        inline quatD& operator -= (const quatD& rhs)
        {
            v[0]-=rhs.v[0];
            v[1]-=rhs.v[1];
            v[2]-=rhs.v[2];
            v[3]-=rhs.v[3];
            return *this;            // enable nesting
        }

        /** Negation operator - returns the negative of the quaternion.
        Basically just calls operator - () on the Vec4 */
        inline const quatD operator - () const
        {
            return quatD (-v[0], -v[1], -v[2], -v[3]);
        }

        /// Length of the quaternion = sqrt( vec . vec )
        inline double length() const
        {
            return sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
        }

        /// Length of the quaternion = vec . vec
        inline double length2() const
        {
            return v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3];
        }

        /// Conjugate
        inline quatD conj () const
        {
             return quatD( -v[0], -v[1], -v[2], v[3] );
        }

        /// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
        inline const quatD inverse () const
        {
             return conj() / length2();
         }

      /* --------------------------------------------------------
               METHODS RELATED TO ROTATIONS
        Set a quaternion which will perform a rotation of an
        angle around the axis given by the vector (x,y,z).
        Should be written to also accept an angle and a Vec3?

        Define Spherical Linear interpolation method also

        Not inlined - see the Quat.cpp file for implementation
        -------------------------------------------------------- */
        inline void makeRotate( double  angle,
                          double  x, double  y, double  z )
        {
            const double epsilon = 0.0000001;

            double length = sqrt( x*x + y*y + z*z );
            if (length < epsilon)
            {
                // ~zero length axis, so reset rotation to zero.
                *this = quatD();
                return;
            }

            double inversenorm  = 1.0/length;
            double coshalfangle = cos( 0.5*angle );
            double sinhalfangle = sin( 0.5*angle );

            v[0] = x * sinhalfangle * inversenorm;
            v[1] = y * sinhalfangle * inversenorm;
            v[2] = z * sinhalfangle * inversenorm;
            v[3] = coshalfangle;
        }

        void makeRotate ( double  angle, const vec3D& vec )
        {
        	makeRotate( angle, vec[0], vec[1], vec[2] );
        }

        void makeRotate ( double  angle1, const vec3D& axis1,
                          double  angle2, const vec3D& axis2,
                          double  angle3, const vec3D& axis3)
        {
            makeRotate(angle1, vec3D(axis1),
                       angle2, vec3D(axis2),
                       angle3, vec3D(axis3));

        }

        /** Make a rotation Quat which will rotate vec1 to vec2.
            Generally take a dot product to get the angle between these
            and then use a cross product to get the rotation axis
            Watch out for the two special cases when the vectors
            are co-incident or opposite in direction.*/
        void makeRotate( const vec3D& from, const vec3D& to )
        {

            // This routine takes any vector as argument but normalized
            // vectors are necessary, if only for computing the dot product.
            // Too bad the API is that generic, it leads to performance loss.
            // Even in the case the 2 vectors are not normalized but same length,
            // the sqrt could be shared, but we have no way to know beforehand
            // at this point, while the caller may know.
            // So, we have to test... in the hope of saving at least a sqrt
        	vec3D sourceVector = from;
        	vec3D targetVector = to;

            double fromLen2 = from.length2();
            double fromLen;
            // normalize only when necessary, epsilon test
            if ((fromLen2 < 1.0-1e-7) || (fromLen2 > 1.0+1e-7))
            {
                fromLen = sqrt(fromLen2);
                sourceVector /= fromLen;
            } else fromLen = 1.0;

            double toLen2 = to.length2();
            // normalize only when necessary, epsilon test
            if ((toLen2 < 1.0-1e-7) || (toLen2 > 1.0+1e-7)) {
                double toLen;
                // re-use fromLen for case of mapping 2 vectors of the same length
                if ((toLen2 > fromLen2-1e-7) && (toLen2 < fromLen2+1e-7)) {
                    toLen = fromLen;
                }
                else toLen = sqrt(toLen2);
                targetVector /= toLen;
            }


            // Now let's get into the real stuff
            // Use "dot product plus one" as test as it can be re-used later on
            double dotProdPlus1 = 1.0 + sourceVector * targetVector;

            // Check for degenerate case of full u-turn. Use epsilon for detection
            if (dotProdPlus1 < 1e-7)
            {

                // Get an orthogonal vector of the given vector
                // in a plane with maximum vector coordinates.
                // Then use it as quaternion axis with pi angle
                // Trick is to realize one value at least is >0.6 for a normalized vector.
                if (fabs(sourceVector.x()) < 0.6)
                {
                    const double norm = sqrt(1.0 - sourceVector.x() * sourceVector.x());
                    v[0] = 0.0;
                    v[1] = sourceVector.z() / norm;
                    v[2] = -sourceVector.y() / norm;
                    v[3] = 0.0;
                } else if (fabs(sourceVector.y()) < 0.6)
                {
                    const double norm = sqrt(1.0 - sourceVector.y() * sourceVector.y());
                    v[0] = -sourceVector.z() / norm;
                    v[1] = 0.0;
                    v[2] = sourceVector.x() / norm;
                    v[3] = 0.0;
                } else
                {
                    const double norm = sqrt(1.0 - sourceVector.z() * sourceVector.z());
                    v[0] = sourceVector.y() / norm;
                    v[1] = -sourceVector.x() / norm;
                    v[2] = 0.0;
                    v[3] = 0.0;
                }
            }

            else {
                // Find the shortest angle quaternion that transforms normalized vectors
                // into one other. Formula is still valid when vectors are colinear
                const double s = sqrt(0.5 * dotProdPlus1);
                const vec3D tmp = sourceVector ^ targetVector / (2.0*s);
                v[0] = tmp.x();
                v[1] = tmp.y();
                v[2] = tmp.z();
                v[3] = s;
            }
        }

        void makeRotate(matrix4 &m)
        {
#define QXD  q.v[0]
#define QYD  q.v[1]
#define QZD  q.v[2]
#define QWD  q.v[3]

        	//Quat q_in;
            quatD q( *this );
            double length2 = q.length2();
            if (length2!=1.0 && length2!=0)
            {
                // normalize quat if required.
                q /= sqrt(length2);
            }

            // Source: Gamasutra, Rotating Objects Using Quaternions
            //
            //http://www.gamasutra.com/features/19980703/quaternions_01.htm

            double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

            // calculate coefficients
            x2 = QXD + QXD;
            y2 = QYD + QYD;
            z2 = QZD + QZD;

            xx = QXD * x2;
            xy = QXD * y2;
            xz = QXD * z2;

            yy = QYD * y2;
            yz = QYD * z2;
            zz = QZD * z2;

            wx = QWD * x2;
            wy = QWD * y2;
            wz = QWD * z2;

            // Note.  Gamasutra gets the matrix assignments inverted, resulting
            // in left-handed rotations, which is contrary to OpenGL and OSG's
            // methodology.  The matrix assignment has been altered in the next
            // few lines of code to do the right thing.
            // Don Burns - Oct 13, 2001
            m._mat[0][0] = 1.0 - (yy + zz);
            m._mat[1][0] = xy - wz;
            m._mat[2][0] = xz + wy;


            m._mat[0][1] = xy + wz;
            m._mat[1][1] = 1.0 - (xx + zz);
            m._mat[2][1] = yz - wx;

            m._mat[0][2] = xz - wy;
            m._mat[1][2] = yz + wx;
            m._mat[2][2] = 1.0 - (xx + yy);

        }

        /** Make a rotation Quat which will rotate vec1 to vec2.
            Generally take a dot product to get the angle between these
            and then use a cross product to get the rotation axis
            Watch out for the two special cases of when the vectors
            are co-incident or opposite in direction.*/

        void makeRotate_original( vec3D& from, vec3D& to )
        {
            const double epsilon = 0.0000001;

            double length1  = from.length();
            double length2  = to.length();

            // dot product vec1*vec2
            double cosangle = from*to/(length1*length2);

            if ( fabs(cosangle - 1) < epsilon )
            {
                // cosangle is close to 1, so the vectors are close to being coincident
                // Need to generate an angle of zero with any vector we like
                // We'll choose (1,0,0)
                makeRotate( 0.0, 0.0, 0.0, 1.0 );
            }
            else
            if ( fabs(cosangle + 1.0) < epsilon )
            {
                // vectors are close to being opposite, so will need to find a
                // vector orthongonal to from to rotate about.
                vec3D tmp;
                if (fabs(from.x())<fabs(from.y()))
                    if (fabs(from.x())<fabs(from.z())) tmp.set(1.0,0.0,0.0); // use x axis.
                    else tmp.set(0.0,0.0,1.0);
                else if (fabs(from.y())<fabs(from.z())) tmp.set(0.0,1.0,0.0);
                else tmp.set(0.0,0.0,1.0);

                vec3D fromd(from.x(),from.y(),from.z());

                // find orthogonal axis.
                vec3D axis(fromd^tmp);
                axis.normalize();

                v[0] = axis[0]; // sin of half angle of PI is 1.0.
                v[1] = axis[1]; // sin of half angle of PI is 1.0.
                v[2] = axis[2]; // sin of half angle of PI is 1.0.
                v[3] = 0; // cos of half angle of PI is zero.

            }
            else
            {
                // This is the usual situation - take a cross-product of vec1 and vec2
                // and that is the axis around which to rotate.
            	vec3D axis(from^to);
                double angle = acos( cosangle );
                makeRotate( angle, axis );
            }
        }

        /** Return the angle and vector components represented by the quaternion.*/
        void getRotate ( double & angle, double & x, double & y, double & z ) const
        {
            double sinhalfangle = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );

            angle = 2.0 * atan2( sinhalfangle, v[3] );
            if(sinhalfangle)
            {
                x = v[0] / sinhalfangle;
                y = v[1] / sinhalfangle;
                z = v[2] / sinhalfangle;
            }
            else
            {
                x = 0.0;
                y = 0.0;
                z = 1.0;
            }
        }

        /** Return the angle and vector represented by the quaternion.*/
        void getRotate ( double & angle, vec3D& vec ) const
        {
            double x,y,z;
            getRotate(angle,x,y,z);
            vec[0]=x;
            vec[1]=y;
            vec[2]=z;
        }

        quatD getRotate(const matrix4& m)
        {
            quatD q;

#define QXD  q.v[0]
#define QYD  q.v[1]
#define QZD  q.v[2]
#define QWD  q.v[3]

            double s;
            double tq[4];
            int    i, j;

            // Use tq to store the largest trace
            tq[0] = 1 + m._mat[0][0]+m._mat[1][1]+m._mat[2][2];
            tq[1] = 1 + m._mat[0][0]-m._mat[1][1]-m._mat[2][2];
            tq[2] = 1 - m._mat[0][0]+m._mat[1][1]-m._mat[2][2];
            tq[3] = 1 - m._mat[0][0]-m._mat[1][1]+m._mat[2][2];

            // Find the maximum (could also use stacked if's later)
            j = 0;
            for(i=1;i<4;i++) j = (tq[i]>tq[j])? i : j;

            // check the diagonal
            if (j==0)
            {
                /* perform instant calculation */
                QWD = tq[0];
                QXD = m._mat[1][2]-m._mat[2][1];
                QYD = m._mat[2][0]-m._mat[0][2];
                QZD = m._mat[0][1]-m._mat[1][0];
            }
            else if (j==1)
            {
                QWD = m._mat[1][2]-m._mat[2][1];
                QXD = tq[1];
                QYD = m._mat[0][1]+m._mat[1][0];
                QZD = m._mat[2][0]+m._mat[0][2];
            }
            else if (j==2)
            {
                QWD = m._mat[2][0]-m._mat[0][2];
                QXD = m._mat[0][1]+m._mat[1][0];
                QYD = tq[2];
                QZD = m._mat[1][2]+m._mat[2][1];
            }
            else /* if (j==3) */
            {
                QWD = m._mat[0][1]-m._mat[1][0];
                QXD = m._mat[2][0]+m._mat[0][2];
                QYD = m._mat[1][2]+m._mat[2][1];
                QZD = tq[3];
            }

            s = sqrt(0.25/tq[j]);
            QWD *= s;
            QXD *= s;
            QYD *= s;
            QZD *= s;

            return q;

        }
        /** Spherical Linear Interpolation.
        As t goes from 0 to 1, the Quat object goes from "from" to "to". */
        void slerp   ( double  t, const quatD& from, const quatD& to)
        {
            const double epsilon = 0.00001;
            double omega, cosomega, sinomega, scale_from, scale_to ;

            quatD quatTo(to);
            // this is a dot product

            cosomega = from.asVec4() * to.asVec4();

            if ( cosomega <0.0 )
            {
                cosomega = -cosomega;
                quatTo = -to;
            }

            if( (1.0 - cosomega) > epsilon )
            {
                omega= acos(cosomega) ;  // 0 <= omega <= Pi (see man acos)
                sinomega = sin(omega) ;  // this sinomega should always be +ve so
                // could try sinomega=sqrt(1-cosomega*cosomega) to avoid a sin()?
                scale_from = sin((1.0-t)*omega)/sinomega ;
                scale_to = sin(t*omega)/sinomega ;
            }
            else
            {
                /* --------------------------------------------------
                   The ends of the vectors are very close
                   we can use simple linear interpolation - no need
                   to worry about the "spherical" interpolation
                   -------------------------------------------------- */
                scale_from = 1.0 - t ;
                scale_to = t ;
            }

            *this = (from*scale_from) + (quatTo*scale_to);
        }

        /** Rotate a vector by this quaternion.*/
        vec3D operator* (const vec3D& v) const
        {
            // nVidia SDK implementation
        	vec3D uv, uuv;
        	vec3D qvec(v[0], v[1], v[2]);
            uv = qvec ^ v;
            uuv = qvec ^ uv;
            uv *= ( 2.0f * v[3] );
            uuv *= 2.0f;
            return v + uv + uuv;
        }

    protected:

};    // end of class prototype

//typedef quatD<float>  quatf;
typedef quatD quatd;
typedef quatD quat;

}    // end of namespace

#endif
