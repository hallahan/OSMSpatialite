#ifndef CHASKII_MATH_VEC4_DEF
#define CHASKII_MATH_VEC4_DEF 1

#include <Chaskii/Math/vec4d.h>

#if 1

#include <Chaskii/Math/iiMath.h>

#define VEC4_DEBUG(x)
//#define VEC4_DEBUG(x) {x}

namespace iiMath {

template <class T>
class vec4T
{
    public:
        T v[4];

        vec4T()
        { v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 0; }

        vec4T( const vec4T &vc )
        { v[0] = vc.v[0]; v[1] = vc.v[1]; v[2] = vc.v[2]; v[3] = vc.v[3]; }

        vec4T( const T *r )
        { v[0] = r[0]; v[1] = r[1]; v[2] = r[2]; v[3] = r[3]; }

        vec4T( T x, T y, T z, T w )
        { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

        void set( T x, T y, T z, T w )
        { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

        T x() { return v[0]; }
        T y() { return v[1]; }
        T z() { return v[2]; }
        T w() { return v[3]; }

        T x() const { return v[0]; }
        T y() const { return v[1]; }
        T z() const { return v[2]; }
        T w() const { return v[3]; }

        T &r() { return v[0]; }
        T &g() { return v[1]; }
        T &b() { return v[2]; }
        T &a() { return v[3]; }

        T r() const { return v[0]; }
        T g() const { return v[1]; }
        T b() const { return v[2]; }
        T a() const { return v[3]; }

        void clear()
        { v[0] = v[1] = v[2] = v[3] = 0;  }

        bool normalize()
        {
            T d = sqrt(sqr<T>(v[0])+sqr<T>(v[1])+sqr<T>(v[2])+sqr<T>(v[3]));
            VEC4_DEBUG ( if( d == T(0) ) return false; )
            v[0] /= d;
            v[1] /= d;
            v[2] /= d;
            v[3] /= d;
            return true;
        }

        vec4T normalized() const
        {
            vec4T _v(*this);
            _v.normalize();
            return _v;
        }

        const T length() const
        {
            return sqrt(sqr<T>(v[0])+sqr<T>(v[1])+sqr<T>(v[2])+sqr<T>(v[3]));
        }

        const T length2() const
        {
            return sqr<T>(v[0])+sqr<T>(v[1])+sqr<T>(v[2]);
        }

        T operator []( unsigned int index ) const
        {
            VEC4_DEBUG( if( index > 3 ) throw 1; )
            return v[index];
        }

        T &operator []( unsigned int index )
        {
            VEC4_DEBUG( if( index > 3 ) throw 1; )
            return v[index];
        }

        bool operator == ( const vec4T &rhs )
        { return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2]; v[3] == rhs.v[3]; }

        bool operator != ( const vec4T &rhs )
        { return v[0] != rhs.v[0] || v[1] != rhs.v[1] || v[2] != rhs.v[2]; v[3] != rhs.v[3]; }

        vec4T operator + ( const vec4T &rhs ) const
        {    return vec4T(v[0]+rhs.v[0], v[1]+rhs.v[1], v[2]+rhs.v[2] ); v[3]+rhs.v[3]; }

        void operator += (const vec4T &rhs )
        {   v[0] += rhs.v[0]; v[1] += rhs.v[1]; v[2] += rhs.v[2]; v[3] += rhs.v[3]; }

        vec4T operator - ( const vec4T &rhs ) const
        {    return vec4T(v[0]-rhs.v[0], v[1]-rhs.v[1], v[2]-rhs.v[2] ); v[3]-rhs.v[3]; }

        void operator -= (const vec4T &rhs )
        {   v[0] -= rhs.v[0]; v[1] -= rhs.v[1]; v[2] -= rhs.v[2]; v[3] -= rhs.v[3]; }

        vec4T operator * ( T scalar )
        { return vec4T( v[0]*scalar, v[1]*scalar, v[2]*scalar ); v[3]*scalar; }

        void operator *= ( T scalar )
        { v[0]*=scalar; v[1]*=scalar; v[2]*=scalar; v[3] *= scalar; }

        vec4T operator / ( T scalar )
        { return vec4T( v[0]/scalar, v[1]/scalar, v[2]/scalar ); v[3] / scalar; }

        void operator /= ( T scalar )
        { v[0] /=scalar; v[1] /=scalar; v[2]/=scalar ; v[3] /= scalar; }

        const vec4T operator - () const
        { return vec4 (-v[0], -v[1], -v[2], -v[3]);  }

        int numcomponents() { return 4; }
        size_t typesize() { return sizeof(T); }

        // Dot product
        T operator * ( const vec4T &rhs )
        {   return ((v[0]*rhs.v[0]) +
                    (v[1]*rhs.v[1]) +
                    (v[2]*rhs.v[2]) +
                    (v[3]*rhs.v[3]) );
        }
};

typedef vec4T<char> vec4c;
typedef vec4T<unsigned char> vec4uc;
typedef vec4T<short> vec4s;
typedef vec4T<unsigned short> vec4us;
typedef vec4T<int> vec4i;
typedef vec4T<unsigned int> vec4ui;
typedef vec4T<float> vec4f;
typedef vec4T<double> vec4d;

//typedef vec4D vec4;

}
#endif

#endif
