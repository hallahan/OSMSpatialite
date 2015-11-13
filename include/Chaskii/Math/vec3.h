#ifndef CHASKII_MATH_VEC3_DEF
#define CHASKII_MATH_VEC3_DEF 1

#include <Chaskii/Math/vec3d.h>

#if 1

#include <Chaskii/Math/iiMath.h>

#define VEC3_DEBUG(x)
//#define VEC3_DEBUG(x) {x}

namespace iiMath {

template <class T>
class vec3T
{
    public:
        T v[3];

        vec3T()
        { v[0] = 0; v[1] = 0; v[2] = 0; }

        vec3T( const vec3T &vc )
        { v[0] = vc.v[0]; v[1] = vc.v[1]; v[2] = vc.v[2]; }

        vec3T( const T *r )
        { v[0] = r[0]; v[1] = r[1]; v[2] = r[2]; }

        vec3T( T x, T y, T z )
        { v[0] = x; v[1] = y; v[2] = z; }

        void set( T x, T y, T z )
        { v[0] = x; v[1] = y; v[2] = z; }

        T x() { return v[0]; }
        T y() { return v[1]; }
        T z() { return v[2]; }

        T x() const { return v[0]; }
        T y() const { return v[1]; }
        T z() const { return v[2]; }

        T r() { return v[0]; }
        T g() { return v[1]; }
        T b() { return v[2]; }

        T r() const { return v[0]; }
        T g() const { return v[1]; }
        T b() const { return v[2]; }

        void clear()
        { v[0] = v[1] = v[2] = 0.0; }

        bool normalize()
        {
            T d = sqrt(sqr(v[0])+sqr(v[1])+sqr(v[2]));
            VEC3_DEBUG ( if( d == T(0) ) return false; )
            v[0] /= d;
            v[1] /= d;
            v[2] /= d;
            return true;
        }

        vec3T normalized() const
        {
            vec3T _v(*this);
            _v.normalize();
            return _v;
        }

        const T length() const
        {
            return sqrt(sqr<T>(v[0])+sqr<T>(v[1])+sqr<T>(v[2]));
        }

        const T length2() const
        {
            return sqr<T>(v[0])+sqr<T>(v[1])+sqr<T>(v[2]);
        }

        T operator []( unsigned int index ) const
        {
            VEC3_DEBUG( if( index > 2 ) throw 1; )
            return v[index];
        }

        T &operator []( unsigned int index )
        {
            VEC3_DEBUG( if( index > 2 ) throw 1; )
            return v[index];
        }

        bool operator < ( const vec3T<T> &rhs ) const 
        { return v[0] < rhs.v[0] && v[1] < rhs.v[1] && v[2] < rhs.v[2]; }

        bool operator == ( const vec3T &rhs )
        { return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2]; }

        bool operator != ( const vec3T &rhs )
        { return v[0] != rhs.v[0] || v[1] != rhs.v[1] || v[2] != rhs.v[2]; }

        vec3T operator + ( const vec3T &rhs ) const
        {    return vec3T(v[0]+rhs.v[0], v[1]+rhs.v[1], v[2]+rhs.v[2] ); }

        void operator += (const vec3T &rhs )
        {   v[0] += rhs.v[0]; v[1] += rhs.v[1]; v[2] += rhs.v[2]; }

        vec3T operator - ( const vec3T &rhs ) const
        {    return vec3T(v[0]-rhs.v[0], v[1]-rhs.v[1], v[2]-rhs.v[2] ); }

        void operator -= (const vec3T &rhs )
        {   v[0] -= rhs.v[0]; v[1] -= rhs.v[1]; v[2] -= rhs.v[2]; }

        // Dot product
        T operator * ( const vec3T &rhs )
        {   return ((v[0]*rhs.v[0]) +
                    (v[1]*rhs.v[1]) +
                    (v[2]*rhs.v[2]) );
        }

        // Cross product
        vec3T operator ^ ( const vec3T &rhs )
        {   return vec3T(
                (v[1]*rhs.v[2]) - (v[2]*rhs.v[1]),
                -(v[0]*rhs.v[2]) + (v[2]*rhs.v[0]),
                (v[0]*rhs.v[1]) - (v[1]*rhs.v[0]) );

        }

        // Angle between two vec3s
        double operator % ( const vec3T &rhs )
        {
            vec3T vn0 = this->normalized();
            vec3T vn1 = rhs.normalized();
            return asin( (vn0^vn1).length() );
        }

        vec3T operator * ( T scalar )
        { return vec3T( v[0]*scalar, v[1]*scalar, v[2]*scalar ); }

        void operator *= ( T scalar )
        { v[0]*=scalar; v[1]*=scalar; v[2]*=scalar; }

        vec3T operator / ( T scalar )
        { return vec3T( v[0]/scalar, v[1]/scalar, v[2]/scalar ); }

        void operator /= ( T scalar )
        { v[0] /=scalar; v[1] /=scalar; v[2]/=scalar ; }

        const vec3T operator - () const
        {
            return vec3T (-v[0], -v[1], -v[2]);
        }

        int numcomponents() { return 3; }
        size_t typesize() { return sizeof(T); }


};

typedef vec3T<char> vec3c;
typedef vec3T<unsigned char> vec3uc;
typedef vec3T<short> vec3s;
typedef vec3T<unsigned short> vec3us;
typedef vec3T<int> vec3i;
typedef vec3T<unsigned int> vec3ui;
typedef vec3T<float> vec3f;
typedef vec3T<double> vec3d;

//typedef vec3D vec3;

}
#endif

#endif
