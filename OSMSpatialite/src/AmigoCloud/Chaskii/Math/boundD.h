#ifndef CHASKII_MATH_BOUNDD_DEF
#define CHASKII_MATH_BOUNDD_DEF 1

#include "vec3d.h"
#include <stdio.h>

namespace iiMath {


class boundD
{
    public:
        boundD( ):
            _center(0,0,0),
            _radius(0),
            _lbn(0,0,0),
            _rtf(0,0,0),
            _isset(false) {}

        boundD( const boundD &b ):
            _center(b._center),
            _radius(b._radius),
            _lbn(b._lbn),
            _rtf(b._rtf),
            _isset(b._isset) {}

        boundD( const vec3 &center, double radius ):
            _center(center),
            _radius(radius)
        {
            _lbn.set( _center[0] - _radius,
                      _center[1] - _radius,
                      _center[2] - _radius );

            _rtf.set( _center[0] + _radius,
                      _center[1] + _radius,
                      _center[2] + _radius );
            _isset = true;
        }

        boundD( const vec3 &lbn, const vec3 &rtf ):
            _lbn(lbn),
            _rtf(rtf)
        {
            _center = (_lbn + _rtf) * 0.5;
            _radius = (_rtf - _center).length();
        }

        void set( const vec3 &lbn, const vec3 &rtf )
        {
            _lbn = lbn;
            _rtf = rtf;

            _center = (_lbn + _rtf) * 0.5;
            _radius = (_rtf - _center).length();
        }

        void set( const vec3 &center, double radius )
        {
            _center = center;
            _radius = radius;
            _lbn.set( _center[0] - _radius,
                      _center[1] - _radius,
                      _center[2] - _radius );

            _rtf.set( _center[0] + _radius,
                      _center[1] + _radius,
                      _center[2] + _radius );
            _isset = true;
        }

        void set( const boundD &b )
        {
            set( b.lbn(), b.rtf() );
        }

        double l() const { return _lbn[0]; }
        double r() const { return _rtf[0]; }
        double b() const { return _lbn[1]; }
        double t() const { return _rtf[1]; }
        double n() const { return _lbn[2]; }
        double f() const { return _rtf[2]; }

        const vec3 &lbn() const { return _lbn; }
        vec3 lbf() const { return vec3(l(),b(),f()); }
        vec3 ltn() const { return vec3(l(),t(),n()); }
        vec3 ltf() const { return vec3(l(),t(),f()); }

        vec3 rbn() const { return vec3(r(),b(),n()); }
        vec3 rbf() const { return vec3(r(),b(),f()); }
        vec3 rtn() const { return vec3(r(),t(),n()); }
        const vec3 &rtf() const { return _rtf; }

        const vec3 &center() const { return _center; }
        double radius() const { return _radius; }

        void reset()
        {
            _lbn.set(0,0,0);
            _rtf.set(0,0,0);
            _center.set(0,0,0);
            _radius = 0;
            _isset = false;
        }


        void expandBy( const vec3 &p )
        {
            if( _isset == false )
            {
                _lbn = _rtf = p;
                _center = p;
                _radius = 0;
                _isset = true;
            }
            else
            {
                if( _lbn[0] > p[0] ) _lbn[0] = p[0];
                if( _lbn[1] > p[1] ) _lbn[1] = p[1];
                if( _lbn[2] > p[2] ) _lbn[2] = p[2];

                if( _rtf[0] < p[0] ) _rtf[0] = p[0];
                if( _rtf[1] < p[1] ) _rtf[1] = p[1];
                if( _rtf[2] < p[2] ) _rtf[2] = p[2];

                _center = (_lbn + _rtf) * 0.5;
                _radius = (_rtf - _center).length();
            }
        }


        void expandBy( const boundD &b )
        {
            expandBy( b.center(), b.radius() );
        }

        void expandBy( const vec3 &center, double radius )
        {
            if(!_isset )
            {
                _center = center;
                _radius = radius;
                _isset = true;
            }
            else
            {
                vec3 v = center - _center;
                double v_len = v.length();

                if( v_len == 0 && radius > _radius )
                    _radius = radius;
                else if( (v_len + radius) > _radius )
                {
                    v.normalize();
                    vec3 c1 = center + (v * radius);
                    vec3 c0 = _center - (v * _radius);
                    _center = (c0 + c1) * 0.5;
                    _radius = (c1 - c0).length() * 0.5;
                }

                double s = sqrt( sqr<double>(_radius)/3 );
                _lbn.set( _center[0] - s,
                          _center[1] - s,
                          _center[2] - s );

                _rtf.set( _center[0] + s,
                          _center[1] + s,
                          _center[2] + s );
            }
        }

        bool within(const vec3 &p) const 
        {
            if(p.y() < l() || p.y() > r())
            {
                return false;
            }
            
            if(p.x() < b() || p.x() > t())
            {
                return false;
            }
        
            return true;
        }
    
    private:
        vec3   _center;
        double _radius;
        vec3 _lbn;
        vec3 _rtf;
        bool _isset;
};

//typedef boundD<float> boundf;
//typedef boundD<double> boundd;
typedef boundD bound;

}

#endif
