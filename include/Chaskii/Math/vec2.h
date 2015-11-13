#ifndef CHASKII_MATH_VEC2_DEF
#define CHASKII_MATH_VEC2_DEF 1

#include <Chaskii/Math/iiMath.h>

#define VEC2_DEBUG(x)
//#define VEC2_DEBUG(x) {x}

namespace iiMath {

template <class T>
class vec2T {
    public:
        T v[2];

        vec2T()
        { v[0] = 0; v[1] = 0; }

        vec2T( const vec2T &vc )
        { v[0] = vc.v[0]; v[1] = vc.v[1];  }

        vec2T( const T *r )
        { v[0] = r[0]; v[1] = r[1];  }

        vec2T( T x, T y )
        { v[0] = x; v[1] = y; }

        void set( T x, T y )
        { v[0] = x; v[1] = y; }

        T x() const { return v[0]; }
        T y() const { return v[1]; }

        T s() { return v[0]; }
        T t() { return v[1]; }

        void clear()
        { v[0] = v[1] = 0.0; }

        bool normalize()
        {
            T d = sqrt(sqr<T>(v[0])+sqr<T>(v[1]));
            VEC2_DEBUG ( if( d == T(0) ) return false; )
            v[0] /= d;
            v[1] /= d;
            return true;
        }

        vec2T normalized() const
        {
            vec2T _v(*this);
            _v.normalize();
            return _v;
        }

        T length()
        {
            return sqrt(sqr<T>(v[0])+sqr<T>(v[1]));
        }

        T &operator []( unsigned int index )
        {
            VEC2_DEBUG( if( index > 1 ) throw 1; )
            return v[index];
        }

        T operator []( unsigned int index ) const
        {
            VEC2_DEBUG( if( index > 1 ) throw 1; )
            return v[index];
        }

        bool operator == ( const vec2T &rhs )
        { return v[0] == rhs.v[0] && v[1] == rhs.v[1]; }

        bool operator != ( const vec2T &rhs )
        { return v[0] != rhs.v[0] || v[1] != rhs.v[1]; }

        vec2T operator + ( const vec2T &rhs )
        {    return vec2T(v[0]+rhs.v[0], v[1]+rhs.v[1] ); }

        void operator += (const vec2T &rhs )
        {   v[0] += rhs.v[0]; v[1] += rhs.v[1]; }

        vec2T operator - ( const vec2T &rhs ) 
        {    return vec2T(v[0]-rhs.v[0], v[1]-rhs.v[1]  ); }

        vec2T operator - ( const vec2T &rhs ) const
        {    return vec2T(v[0]-rhs.v[0], v[1]-rhs.v[1]  ); }

        void operator -= (const vec2T &rhs )
        {   v[0] -= rhs.v[0]; v[1] -= rhs.v[1];  }

        vec2T operator * ( T scalar ) 
        { return vec2T( v[0]*scalar, v[1]*scalar ); }

        void operator *= ( T scalar ) 
        { v[0]*=scalar; v[1]*=scalar; }

        int numcomponents() { return 2; }
        size_t typesize() { return sizeof(T); }


};

typedef vec2T<char> vec2c;
typedef vec2T<unsigned char> vec2uc;
typedef vec2T<short> vec2s;
typedef vec2T<unsigned short> vec2us;
typedef vec2T<int> vec2i;
typedef vec2T<unsigned int> vec2ui;
typedef vec2T<float> vec2f;
typedef vec2T<double> vec2d;

typedef vec2d vec2;


}

#endif

