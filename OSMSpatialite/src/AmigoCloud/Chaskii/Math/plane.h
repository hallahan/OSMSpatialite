#ifndef CHASKII_MATH_PLANE_DEF
#define CHASKII_MATH_PLANE_DEF 1

#include "planed.h"
#include "vec3.h"

namespace iiMath {

template <class T>
class planeT
{
    public:
        T a, b, c, d, l;

        planeT():
            a(0),
            b(0),
            c(1),
            d(0)
        {}

        planeT(const planeT &p ):
            a(p.a),
            b(p.b),
            c(p.c),
            d(p.d),
            l(p.l) {}

        planeT( vec3T<T> pt, vec3T<T> normal )
        {
            set( pt, normal );
        }

        void set( T _a, T _b, T _c, T _d )
        {
            a = _a;
            b = _b;
            c = _c;
            d = _d;
        }

        void set( vec3T<T> pt, vec3T<T> normal )
        {
            a = normal[0];
            b = normal[1];
            c = normal[2];
            d = -((a * pt[0]) +
                (b * pt[1]) +
                (c * pt[2])) ;
            l = normal.length();
        }

        void getEquation( double &_a, double &_b, double &_c, double &_d ) const
        {
            _a = a;
            _b = b;
            _c = c;
            _d = d;
        }

        double distanceFrom( const vec3T<T> &pt ) const
        {
            return ((a*pt.x())+(b*pt.y())+(c*pt.z())+d)/l;
        }

        T operator[](unsigned int index)
        {
            switch( index )
            {
                case 0: return a; break;
                case 1: return b; break;
                case 2: return c; break;
                case 4: return d; break;
                //case 5: return l; break;
                default: return 0;
            }
        }

        int numcomponents() { return 4; }
        size_t typesize() { return sizeof(T); }
};

typedef planeT<float> planef;
typedef planeT<double> planed;

typedef planeD plane;

}

#endif
