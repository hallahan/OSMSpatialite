#ifndef CHASKII_MATH_PLANED_DEF
#define CHASKII_MATH_PLANED_DEF 1

#include "vec3d.h"

namespace iiMath {


class planeD
{
    public:
        double a, b, c, d, l;

        planeD():
            a(0),
            b(0),
            c(1),
            d(0)
        {}

        planeD(const planeD &p ):
            a(p.a),
            b(p.b),
            c(p.c),
            d(p.d),
            l(p.l) {}

        planeD( vec3 pt, vec3 normal )
        {
            set( pt, normal );
        }

        void set( double _a, double _b, double _c, double _d )
        {
            a = _a;
            b = _b;
            c = _c;
            d = _d;
        }

        void set( vec3 pt, vec3 normal )
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

        double distanceFrom( const vec3 &pt ) const
        {
            return ((a*pt.x())+(b*pt.y())+(c*pt.z())+d)/l;
        }

        double operator[](unsigned int index)
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
        size_t typesize() { return sizeof(double); }
};

//typedef planeD<float> planef;
//typedef planeD planed;

typedef planeD plane;

}

#endif
