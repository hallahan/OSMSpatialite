#ifndef CHASKII_MATH_QUAT_DEF
#define CHASKII_MATH_QUAT_DEF 1

#include "vec3.h"
#include "vec4.h"
#include "matrix.h"
#include "quatd.h"

namespace iiMath {

/** A quaternion class. It can be used to represent an orientation in 3D space.*/
template <class T>
class quatT
{

    public:
        T  v[4];    // a four-vector

        inline quatT() { v[0]=0.0; v[1]=0.0; v[2]=0.0; v[3]=1.0; }

        inline quatT( T x, T y, T z, T w )
        {
            v[0]=x;
            v[1]=y;
            v[2]=z;
            v[3]=w;
        }

        inline quatT( const quatT<T>& q )
        {
            v[0]=q.x();
            v[1]=q.y();
            v[2]=q.z();
            v[3]=q.w();
        }

        inline quatT( const vec4T<T>& vv )
        {
            v[0]=vv.x();
            v[1]=vv.y();
            v[2]=vv.z();
            v[3]=vv.w();
        }


        inline quatT( T angle, const vec3T<T>& axis)
        {
            makeRotate(angle,axis);
        }

        inline quatT( T angle1, const vec3T<T>& axis1,
                      T angle2, const vec3T<T>& axis2,
                      T angle3, const vec3T<T>& axis3)
        {
            makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
        }

        inline quatT& operator = (const quatT& q) { v[0]=q.v[0];  v[1]=q.v[1]; v[2]=q.v[2]; v[3]=q.v[3]; return *this; }

        inline bool operator == (const quatT& v) const { return v[0]==v.v[0] && v[1]==v.v[1] && v[2]==v.v[2] && v[3]==v.v[3]; }

        inline bool operator != (const quatT& v) const { return v[0]!=v.v[0] || v[1]!=v.v[1] || v[2]!=v.v[2] || v[3]!=v.v[3]; }

        inline bool operator <  (const quatT& v) const
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

        inline vec4T<T> asVec4() const
        {
            return vec4T<T>(v[0], v[1], v[2], v[3]);
        }

        inline vec3T<T> asVec3() const
        {
            return vec3T<T>(v[0], v[1], v[2]);
        }

        inline void set(T x, T y, T z, T w)
        {
            v[0]=x;
            v[1]=y;
            v[2]=z;
            v[3]=w;
        }

        inline void set(const vec4T<T>& v)
        {
            v[0]=v.x();
            v[1]=v.y();
            v[2]=v.z();
            v[3]=v.w();
        }

        void set(const matrix4T<T>& matrix)
        {
            *this = getRotate( matrix );
        }

//        void set(matrix4T<T>& matrix)
//        {
//            *this = getRotate( matrix );
//        }

        void get(matrix4T<T>& matrix) const
        {
        	//matrix.makeRotate(*this);
        	matrix.makeRotate(v);
        }

        inline T & operator [] (int i) { return v[i]; }
        inline T   operator [] (int i) const { return v[i]; }

        inline T & x() { return v[0]; }
        inline T & y() { return v[1]; }
        inline T & z() { return v[2]; }
        inline T & w() { return v[3]; }

        inline T x() const { return v[0]; }
        inline T y() const { return v[1]; }
        inline T z() const { return v[2]; }
        inline T w() const { return v[3]; }

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
        inline const quatT operator * (T rhs) const
        {
            return quatT(v[0]*rhs, v[1]*rhs, v[2]*rhs, v[3]*rhs);
        }

        /// Unary multiply by scalar
        inline quatT& operator *= (T rhs)
        {
            v[0]*=rhs;
            v[1]*=rhs;
            v[2]*=rhs;
            v[3]*=rhs;
            return *this;        // enable nesting
        }

        /// Binary multiply
        inline const quatT operator*(const quatT& rhs) const
        {
            return quatT( rhs.v[3]*v[0] + rhs.v[0]*v[3] + rhs.v[1]*v[2] - rhs.v[2]*v[1],
                 rhs.v[3]*v[1] - rhs.v[0]*v[2] + rhs.v[1]*v[3] + rhs.v[2]*v[0],
                 rhs.v[3]*v[2] + rhs.v[0]*v[1] - rhs.v[1]*v[0] + rhs.v[2]*v[3],
                 rhs.v[3]*v[3] - rhs.v[0]*v[0] - rhs.v[1]*v[1] - rhs.v[2]*v[2] );
        }

        /// Unary multiply
        inline quatT& operator*=(const quatT& rhs)
        {
            T x = rhs.v[3]*v[0] + rhs.v[0]*v[3] + rhs.v[1]*v[2] - rhs.v[2]*v[1];
            T y = rhs.v[3]*v[1] - rhs.v[0]*v[2] + rhs.v[1]*v[3] + rhs.v[2]*v[0];
            T z = rhs.v[3]*v[2] + rhs.v[0]*v[1] - rhs.v[1]*v[0] + rhs.v[2]*v[3];
            v[3]   = rhs.v[3]*v[3] - rhs.v[0]*v[0] - rhs.v[1]*v[1] - rhs.v[2]*v[2];

            v[2] = z;
            v[1] = y;
            v[0] = x;

            return (*this);            // enable nesting
        }

        /// Divide by scalar
        inline quatT operator / (T rhs) const
        {
            T div = 1.0/rhs;
            return quatT(v[0]*div, v[1]*div, v[2]*div, v[3]*div);
        }

        /// Unary divide by scalar
        inline quatT& operator /= (T rhs)
        {
            T div = 1.0/rhs;
            v[0]*=div;
            v[1]*=div;
            v[2]*=div;
            v[3]*=div;
            return *this;
        }

        /// Binary divide
        inline const quatT operator/(const quatT& denom) const
        {
            return ( (*this) * denom.inverse() );
        }

        /// Unary divide
        inline quatT& operator/=(const quatT& denom)
        {
            (*this) = (*this) * denom.inverse();
            return (*this);            // enable nesting
        }

        /// Binary addition
        inline const quatT operator + (const quatT& rhs) const
        {
            return quatT(v[0]+rhs.v[0], v[1]+rhs.v[1],
                v[2]+rhs.v[2], v[3]+rhs.v[3]);
        }

        /// Unary addition
        inline quatT& operator += (const quatT& rhs)
        {
            v[0] += rhs.v[0];
            v[1] += rhs.v[1];
            v[2] += rhs.v[2];
            v[3] += rhs.v[3];
            return *this;            // enable nesting
        }

        /// Binary subtraction
        inline const quatT operator - (const quatT& rhs) const
        {
            return quatT(v[0]-rhs.v[0], v[1]-rhs.v[1],
                v[2]-rhs.v[2], v[3]-rhs.v[3] );
        }

        /// Unary subtraction
        inline quatT& operator -= (const quatT& rhs)
        {
            v[0]-=rhs.v[0];
            v[1]-=rhs.v[1];
            v[2]-=rhs.v[2];
            v[3]-=rhs.v[3];
            return *this;            // enable nesting
        }

        /** Negation operator - returns the negative of the quaternion.
        Basically just calls operator - () on the Vec4 */
        inline const quatT operator - () const
        {
            return quatT (-v[0], -v[1], -v[2], -v[3]);
        }

        /// Length of the quaternion = sqrt( vec . vec )
        inline T length() const
        {
            return sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
        }

        /// Length of the quaternion = vec . vec
        inline T length2() const
        {
            return v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3];
        }

        /// Conjugate
        inline quatT conj () const
        {
             return quatT( -v[0], -v[1], -v[2], v[3] );
        }

        /// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
        inline const quatT inverse () const
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
        inline void makeRotate( T  angle,
                          T  x, T  y, T  z )
        {
            const T epsilon = 0.0000001;

            T length = sqrt( x*x + y*y + z*z );
            if (length < epsilon)
            {
                // ~zero length axis, so reset rotation to zero.
                *this = quatT<T>();
                return;
            }

            T inversenorm  = 1.0/length;
            T coshalfangle = cos( 0.5*angle );
            T sinhalfangle = sin( 0.5*angle );

            v[0] = x * sinhalfangle * inversenorm;
            v[1] = y * sinhalfangle * inversenorm;
            v[2] = z * sinhalfangle * inversenorm;
            v[3] = coshalfangle;
        }

        void makeRotate ( T  angle, const vec3T<T>& vec )
        {
        	makeRotate( angle, vec[0], vec[1], vec[2] );
        }

        void makeRotate ( T  angle1, const vec3T<T>& axis1,
                          T  angle2, const vec3T<T>& axis2,
                          T  angle3, const vec3T<T>& axis3)
        {
            makeRotate(angle1, vec3T<T>(axis1),
                       angle2, vec3T<T>(axis2),
                       angle3, vec3T<T>(axis3));

        }

        /** Make a rotation Quat which will rotate vec1 to vec2.
            Generally take a dot product to get the angle between these
            and then use a cross product to get the rotation axis
            Watch out for the two special cases when the vectors
            are co-incident or opposite in direction.*/
        void makeRotate( const vec3T<T>& from, const vec3T<T>& to )
        {

            // This routine takes any vector as argument but normalized
            // vectors are necessary, if only for computing the dot product.
            // Too bad the API is that generic, it leads to performance loss.
            // Even in the case the 2 vectors are not normalized but same length,
            // the sqrt could be shared, but we have no way to know beforehand
            // at this point, while the caller may know.
            // So, we have to test... in the hope of saving at least a sqrt
        	vec3T<T> sourceVector = from;
        	vec3T<T> targetVector = to;

            T fromLen2 = from.length2();
            T fromLen;
            // normalize only when necessary, epsilon test
            if ((fromLen2 < 1.0-1e-7) || (fromLen2 > 1.0+1e-7))
            {
                fromLen = sqrt(fromLen2);
                sourceVector /= fromLen;
            } else fromLen = 1.0;

            T toLen2 = to.length2();
            // normalize only when necessary, epsilon test
            if ((toLen2 < 1.0-1e-7) || (toLen2 > 1.0+1e-7)) {
                T toLen;
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
                const vec3T<T> tmp = sourceVector ^ targetVector / (2.0*s);
                v[0] = tmp.x();
                v[1] = tmp.y();
                v[2] = tmp.z();
                v[3] = s;
            }
        }

        void makeRotate(matrix4T<T> &m)
        {
#define QX  q.v[0]
#define QY  q.v[1]
#define QZ  q.v[2]
#define QW  q.v[3]

        	//Quat q_in;
            quatT<T> q( *this );
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

        void makeRotate_original( const vec3T<T>& from, const vec3T<T>& to )
        {
            const T epsilon = 0.0000001;

            T length1  = from.length();
            T length2  = to.length();

            // dot product vec1*vec2
            T cosangle = from*to/(length1*length2);

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
                vec3T<T> tmp;
                if (fabs(from.x())<fabs(from.y()))
                    if (fabs(from.x())<fabs(from.z())) tmp.set(1.0,0.0,0.0); // use x axis.
                    else tmp.set(0.0,0.0,1.0);
                else if (fabs(from.y())<fabs(from.z())) tmp.set(0.0,1.0,0.0);
                else tmp.set(0.0,0.0,1.0);

                vec3T<T> fromd(from.x(),from.y(),from.z());

                // find orthogonal axis.
                vec3T<T> axis(fromd^tmp);
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
            	vec3T<T> axis(from^to);
                T angle = acos( cosangle );
                makeRotate( angle, axis );
            }
        }

        /** Return the angle and vector components represented by the quaternion.*/
        void getRotate ( T & angle, T & x, T & y, T & z ) const
        {
            T sinhalfangle = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );

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
        void getRotate ( T & angle, vec3T<T>& vec ) const
        {
            T x,y,z;
            getRotate(angle,x,y,z);
            vec[0]=x;
            vec[1]=y;
            vec[2]=z;
        }

        quatT<T> getRotate(const matrix4T<T>& m)
        {
            quatT<T> q;

#define QX  q.v[0]
#define QY  q.v[1]
#define QZ  q.v[2]
#define QW  q.v[3]

            T s;
            T tq[4];
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
                QW = tq[0];
                QX = m._mat[1][2]-m._mat[2][1];
                QY = m._mat[2][0]-m._mat[0][2];
                QZ = m._mat[0][1]-m._mat[1][0];
            }
            else if (j==1)
            {
                QW = m._mat[1][2]-m._mat[2][1];
                QX = tq[1];
                QY = m._mat[0][1]+m._mat[1][0];
                QZ = m._mat[2][0]+m._mat[0][2];
            }
            else if (j==2)
            {
                QW = m._mat[2][0]-m._mat[0][2];
                QX = m._mat[0][1]+m._mat[1][0];
                QY = tq[2];
                QZ = m._mat[1][2]+m._mat[2][1];
            }
            else /* if (j==3) */
            {
                QW = m._mat[0][1]-m._mat[1][0];
                QX = m._mat[2][0]+m._mat[0][2];
                QY = m._mat[1][2]+m._mat[2][1];
                QZ = tq[3];
            }

            s = sqrt(0.25/tq[j]);
            QW *= s;
            QX *= s;
            QY *= s;
            QZ *= s;

            return q;

        }
        /** Spherical Linear Interpolation.
        As t goes from 0 to 1, the Quat object goes from "from" to "to". */
        void slerp   ( T  t, const quatT& from, const quatT& to)
        {
            const double epsilon = 0.00001;
            double omega, cosomega, sinomega, scale_from, scale_to ;

            quatT<T> quatTo(to);
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
        vec3T<T> operator* (const vec3T<T>& v) const
        {
            // nVidia SDK implementation
        	vec3T<T> uv, uuv;
        	vec3T<T> qvec(v[0], v[1], v[2]);
            uv = qvec ^ v;
            uuv = qvec ^ uv;
            uv *= ( 2.0f * v[3] );
            uuv *= 2.0f;
            return v + uv + uuv;
        }

    protected:

};    // end of class prototype

typedef quatT<float>  quatf;
//typedef quatT<double> quatd;
//typedef quatd quat;

}    // end of namespace

#endif
