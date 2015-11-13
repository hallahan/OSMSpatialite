#ifndef CHASKII_MATH_VEC4D_DEF
#define CHASKII_MATH_VEC4D_DEF 1

#include <Chaskii/Math/iiMath.h>

#define VEC4_DEBUG(x)
//#define VEC4_DEBUG(x) {x}

namespace iiMath {

class vec4D
{
    public:
        double v[4];

        vec4D()
        { v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 0; }

        vec4D( const vec4D &vc )
        { v[0] = vc.v[0]; v[1] = vc.v[1]; v[2] = vc.v[2]; v[3] = vc.v[3]; }

        vec4D( const double *r )
        { v[0] = r[0]; v[1] = r[1]; v[2] = r[2]; v[3] = r[3]; }

        vec4D( double x, double y, double z, double w )
        { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

        void set( double x, double y, double z, double w )
        { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

        double x() { return v[0]; }
        double y() { return v[1]; }
        double z() { return v[2]; }
        double w() { return v[3]; }

        double x() const { return v[0]; }
        double y() const { return v[1]; }
        double z() const { return v[2]; }
        double w() const { return v[3]; }

        double r() { return v[0]; }
        double g() { return v[1]; }
        double b() { return v[2]; }
        double a() { return v[3]; }

        double r() const { return v[0]; }
        double g() const { return v[1]; }
        double b() const { return v[2]; }
        double a() const { return v[3]; }

        void clear()
        { v[0] = v[1] = v[2] = v[3] = 0;  }

        bool normalize()
        {
            double d = sqrt(sqr<double>(v[0])+sqr<double>(v[1])+sqr<double>(v[2])+sqr<double>(v[3]));
            VEC4_DEBUG ( if( d == double(0) ) return false; )
            v[0] /= d;
            v[1] /= d;
            v[2] /= d;
            v[3] /= d;
            return true;
        }

        vec4D normalized() const
        {
            vec4D _v(*this);
            _v.normalize();
            return _v;
        }

        const double length() const
        {
            return sqrt(sqr<double>(v[0])+sqr<double>(v[1])+sqr<double>(v[2])+sqr<double>(v[3]));
        }

        const double length2() const
        {
            return sqr<double>(v[0])+sqr<double>(v[1])+sqr<double>(v[2]);
        }

        double operator []( unsigned int index ) const
        {
            VEC4_DEBUG( if( index > 3 ) throw 1; )
            return v[index];
        }

        double &operator []( unsigned int index )
        {
            VEC4_DEBUG( if( index > 3 ) throw 1; )
            return v[index];
        }

        bool operator == ( const vec4D &rhs )
        { return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2] && v[3] == rhs.v[3]; }

        bool operator != ( const vec4D &rhs )
        { return v[0] != rhs.v[0] || v[1] != rhs.v[1] || v[2] != rhs.v[2] || v[3] != rhs.v[3]; }

        vec4D operator + ( const vec4D &rhs ) const
        {    return vec4D(v[0]+rhs.v[0], v[1]+rhs.v[1], v[2]+rhs.v[2], v[3]+rhs.v[3]); }

        void operator += (const vec4D &rhs )
        {   v[0] += rhs.v[0]; v[1] += rhs.v[1]; v[2] += rhs.v[2]; v[3] += rhs.v[3]; }

        vec4D operator - ( const vec4D &rhs ) const
        {    return vec4D(v[0]-rhs.v[0], v[1]-rhs.v[1], v[2]-rhs.v[2], v[3]-rhs.v[3]); }

        void operator -= (const vec4D &rhs )
        {   v[0] -= rhs.v[0]; v[1] -= rhs.v[1]; v[2] -= rhs.v[2]; v[3] -= rhs.v[3]; }

        vec4D operator * ( double scalar )
        { return vec4D( v[0]*scalar, v[1]*scalar, v[2]*scalar, v[3]*scalar); }

        void operator *= ( double scalar )
        { v[0]*=scalar; v[1]*=scalar; v[2]*=scalar; v[3] *= scalar; }

        vec4D operator / ( double scalar )
        { return vec4D( v[0]/scalar, v[1]/scalar, v[2]/scalar,  v[3] / scalar); }

        void operator /= ( double scalar )
        { v[0] /=scalar; v[1] /=scalar; v[2]/=scalar ; v[3] /= scalar; }

        const vec4D operator - () const
        { return vec4D(-v[0], -v[1], -v[2], -v[3]);  }

        int numcomponents() { return 4; }
        size_t typesize() { return sizeof(double); }

        // Dot product
        double operator * ( const vec4D &rhs )
        {   return ((v[0]*rhs.v[0]) +
                    (v[1]*rhs.v[1]) +
                    (v[2]*rhs.v[2]) +
                    (v[3]*rhs.v[3]) );
        }
};

//typedef vec4D<char> vec4c;
//typedef vec4D<unsigned char> vec4uc;
//typedef vec4D<short> vec4s;
//typedef vec4D<unsigned short> vec4us;
//typedef vec4D<int> vec4i;
//typedef vec4D<unsigned int> vec4ui;
//typedef vec4D<float> vec4f;
//typedef vec4D<double> vec4d;

typedef vec4D vec4;

}

#endif
