#ifndef CHASKII_MATH_VEC3D_DEF
#define CHASKII_MATH_VEC3D_DEF 1

#include "iiMath.h"

#define VEC3_DEBUG(x)
//#define VEC3_DEBUG(x) {x}

namespace iiMath {


class vec3D
{
    public:
        double v[3];

        vec3D()
        { v[0] = 0; v[1] = 0; v[2] = 0; }

        vec3D( const vec3D &vc )
        { v[0] = vc.v[0]; v[1] = vc.v[1]; v[2] = vc.v[2]; }

        vec3D( const double *r )
        { v[0] = r[0]; v[1] = r[1]; v[2] = r[2]; }

        vec3D( double x, double y, double z )
        { v[0] = x; v[1] = y; v[2] = z; }

        void set( double x, double y, double z )
        { v[0] = x; v[1] = y; v[2] = z; }

        double x() { return v[0]; }
        double y() { return v[1]; }
        double z() { return v[2]; }

        double x() const { return v[0]; }
        double y() const { return v[1]; }
        double z() const { return v[2]; }

        double r() { return v[0]; }
        double g() { return v[1]; }
        double b() { return v[2]; }

        double r() const { return v[0]; }
        double g() const { return v[1]; }
        double b() const { return v[2]; }

        void clear()
        { v[0] = v[1] = v[2] = 0.0; }

        bool normalize()
        {
            double d = sqrt(sqr(v[0])+sqr(v[1])+sqr(v[2]));
            VEC3_DEBUG ( if( d == double(0) ) return false; )
            v[0] /= d;
            v[1] /= d;
            v[2] /= d;
            return true;
        }

        vec3D normalized() const
        {
            vec3D _v(*this);
            _v.normalize();
            return _v;
        }

        const double length() const
        {
            return sqrt(sqr<double>(v[0])+sqr<double>(v[1])+sqr<double>(v[2]));
        }

        const double length2() const
        {
            return sqr<double>(v[0])+sqr<double>(v[1])+sqr<double>(v[2]);
        }

        double operator []( unsigned int index ) const
        {
            VEC3_DEBUG( if( index > 2 ) throw 1; )
            return v[index];
        }

        double &operator []( unsigned int index )
        {
            VEC3_DEBUG( if( index > 2 ) throw 1; )
            return v[index];
        }

        bool operator < ( const vec3D &rhs ) const 
        { return v[0] < rhs.v[0] && v[1] < rhs.v[1] && v[2] < rhs.v[2]; }


        bool operator == ( const vec3D &rhs )
        { return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2]; }

        bool operator != ( const vec3D &rhs )
        { return v[0] != rhs.v[0] || v[1] != rhs.v[1] || v[2] != rhs.v[2]; }

        vec3D operator + ( const vec3D &rhs ) const
        {    return vec3D(v[0]+rhs.v[0], v[1]+rhs.v[1], v[2]+rhs.v[2] ); }

        void operator += (const vec3D &rhs )
        {   v[0] += rhs.v[0]; v[1] += rhs.v[1]; v[2] += rhs.v[2]; }

        vec3D operator - ( const vec3D &rhs ) const
        {    return vec3D(v[0]-rhs.v[0], v[1]-rhs.v[1], v[2]-rhs.v[2] ); }

        void operator -= (const vec3D &rhs )
        {   v[0] -= rhs.v[0]; v[1] -= rhs.v[1]; v[2] -= rhs.v[2]; }

        // Dot product
        double operator * ( const vec3D &rhs )
        {   return ((v[0]*rhs.v[0]) +
                    (v[1]*rhs.v[1]) +
                    (v[2]*rhs.v[2]) );
        }

        // Cross product
        vec3D operator ^ ( const vec3D &rhs )
        {   return vec3D(
                (v[1]*rhs.v[2]) - (v[2]*rhs.v[1]),
                -(v[0]*rhs.v[2]) + (v[2]*rhs.v[0]),
                (v[0]*rhs.v[1]) - (v[1]*rhs.v[0]) );

        }

        // Angle between two vec3s
        double operator % ( const vec3D &rhs )
        {
            vec3D vn0 = this->normalized();
            vec3D vn1 = rhs.normalized();
            return asin( (vn0^vn1).length() );
        }

        vec3D operator * ( double scalar )
        { return vec3D( v[0]*scalar, v[1]*scalar, v[2]*scalar ); }

        void operator *= ( double scalar )
        { v[0]*=scalar; v[1]*=scalar; v[2]*=scalar; }

        vec3D operator / ( double scalar )
        { return vec3D( v[0]/scalar, v[1]/scalar, v[2]/scalar ); }

        void operator /= ( double scalar )
        { v[0] /=scalar; v[1] /=scalar; v[2]/=scalar ; }

        const vec3D operator - () const
        {
            return vec3D (-v[0], -v[1], -v[2]);
        }

        int numcomponents() { return 3; }
        size_t typesize() { return sizeof(double); }


};

//typedef vec3D<char> vec3c;
//typedef vec3D<unsigned char> vec3uc;
//typedef vec3D<short> vec3s;
//typedef vec3D<unsigned short> vec3us;
//typedef vec3D<int> vec3i;
//typedef vec3D<unsigned int> vec3ui;
//typedef vec3D<float> vec3f;
//typedef vec3D<double> vec3d;
//
typedef vec3D vec3;

}

#endif
