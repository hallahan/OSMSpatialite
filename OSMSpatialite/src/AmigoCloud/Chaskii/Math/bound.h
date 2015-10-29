#ifndef CHASKII_MATH_BOUND_DEF
#define CHASKII_MATH_BOUND_DEF 1

#include "vec3.h"
#include "boundD.h"

namespace iiMath {

template <class T>
class boundT
{
    public:
        boundT( ):
            _center(0,0,0),
            _radius(0),
            _lbn(0,0,0),
            _rtf(0,0,0),
            _isset(false) {}

        boundT( const boundT<T> &b):
            _center(b._center),
            _radius(b._radius),
            _isset(b._isset),
            _lbn(b._lbn),
            _rtf(b._rtf){}
    
        boundT( const vec3T<T> &center, T radius ):
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

        boundT( const vec3T<T> &lbn, const vec3T<T> &rtf ):
            _lbn(lbn),
            _rtf(rtf)
        {
            _center = (_lbn + _rtf) * 0.5;
            _radius = (_rtf - _center).length();
        }

        void set( const vec3T<T> &lbn, const vec3T<T> &rtf )
        {
            _lbn = lbn;
            _rtf = rtf;

            _center = (_lbn + _rtf) * 0.5;
            _radius = (_rtf - _center).length();
        }

        void set( const vec3T<T> &center, T radius )
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

        void set( const boundT<T> &b )
        {
            set( b.lbn(), b.rtf() );
        }

        T l() const { return _lbn[0]; }
        T r() const { return _rtf[0]; }
        T b() const { return _lbn[1]; }
        T t() const { return _rtf[1]; }
        T n() const { return _lbn[2]; }
        T f() const { return _rtf[2]; }

        const vec3T<T> &lbn() const { return _lbn; }
        vec3T<T> lbf() const { return vec3T<T>(l(),b(),f()); }
        vec3T<T> ltn() const { return vec3T<T>(l(),t(),n()); }
        vec3T<T> ltf() const { return vec3T<T>(l(),t(),f()); }

        vec3T<T> rbn() const { return vec3T<T>(r(),b(),n()); }
        vec3T<T> rbf() const { return vec3T<T>(r(),b(),f()); }
        vec3T<T> rtn() const { return vec3T<T>(r(),t(),n()); }
        const vec3T<T> &rtf() const { return _rtf; }

        const vec3T<T> &center() const { return _center; }
        T radius() const { return _radius; }

        void reset()
        {
            _lbn.set(0,0,0);
            _rtf.set(0,0,0);
            _center.set(0,0,0);
            _radius = 0;
            _isset = false;
        }


        void expandBy( const vec3T<T> &p )
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


        void expandBy( const boundT<T> &b )
        {
            expandBy( b.center(), b.radius() );
        }

        void expandBy( const vec3T<T> &center, T radius )
        {
            if(!_isset )
            {
                _center = center;
                _radius = radius;
                _isset = true;
            }
            else
            {
                vec3T<T> v = center - _center;
                T v_len = v.length();

                if( v_len == 0 && radius > _radius )
                    _radius = radius;
                else if( (v_len + radius) > _radius )
                {
                    v.normalize();
                    vec3T<T> c1 = center + (v * radius);
                    vec3T<T> c0 = _center - (v * _radius);
                    _center = (c0 + c1) * 0.5;
                    _radius = (c1 - c0).length() * 0.5;
                }

                T s = sqrt( sqr<T>(_radius)/3 );
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
        vec3T<T>   _center;
        T _radius;
        vec3T<T> _lbn;
        vec3T<T> _rtf;
        bool _isset;
};

typedef boundT<float> boundf;
typedef boundT<double> boundd;
//typedef boundd bound;

}

#endif
