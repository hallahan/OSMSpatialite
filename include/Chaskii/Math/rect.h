#ifndef CHASKII_MATH_RECT_DEF
#define CHASKII_MATH_RECT_DEF 1

#include <Chaskii/Math/iiMath.h>

namespace iiMath {

template <class T, class U>
class rectT 
{
    public:
        rectT():
            _x(0),_y(0),_width(0),_height(0) 
        {
        }

        rectT(T x, T y,  U width, U height ):
            _x(x),
            _y(y),
            _width(width),
            _height(height)
        {
        }

        rectT( const rectT &r ):
            _x(r._x),
            _y(r._y),
            _width(r._width),
            _height(r._height)
        {
        }

        virtual ~rectT() {}

        void zero( void )
        {
            _x = 0;
            _y = 0;
            _width = 0;
            _height = 0;
        }

        void set( T xx, T yy, U w, U h )
        { 
            _x = xx; _y = yy; _width = w; _height = h; 
        }

        void set( const rectT &r )
        {
            _x = r._x;
            _y = r._y;
            _width = r._width;
            _height = r._height;
        }
        
        void x(T xx) { _x = xx; } 
        void y(T yy) { _y = yy; } 
        void width( U w ) { _width = w; }
        void height( U h ) { _height = h; }

        T x() const { return _x; }
        T y() const { return _y; }
        U width() const { return _width; }
        U height() const { return _height; }

        bool isEmpty( void ) const
        {
            return ( _width == 0 || _height == 0 );
        }

        void intersect( const rectT &r )
        {
            _width = iiMath::Math::_min_<T>(_x + _width, r._x + r._width);
            _x = iiMath::Math::_max_<T>(_x, r._x);
            _width = _width - _x;
            if ( _width < 0 )
            {
                zero();
                return;
            }
            _height = iiMath::Math::_min_<T>(_y + _height, r._y + r._height);
            _y = iiMath::Math::_max_<T>(_y, r._y);
            _height = _height - _y;
            if ( _height < 0 )
            {
                zero();
                return;
            }
        }

        bool containsPoint(T x, T y) const
        {
            return (x >= _x && x < _x + (int)_width && y >= _y && y < _y + (int)_height);
        }

        virtual bool within( T x, T y ) const
        {
            if( x >= 0 && x < (T)_width &&
                y >= 0 && y < (T)_height )
            {
                return true;
            }
            return false;
        }

        void expandBy(const rectT& r)
        {
            if( _x < r._x ) _x = r._x;
            if( _y < r._y ) _y = r._y;
            if( (_x + _width) < (r._x + r._width) )
                _width = (r._x + r._width) - _x;
            if( (_y + _height) < (r._y + r._height ) )
                _height = (r._y + r._height) - _x;
        }

        bool sameSizeAs( const rectT &r )
        {
            return _width == r._width && _height == r._height; 
        }

    protected:

        T _x, _y;
        U _width, _height;
};

typedef rectT<char, unsigned char> rectc;
typedef rectT<short, unsigned short> rects;
typedef rectT<int, unsigned int> recti;
typedef rectT<float, float> rectf;
typedef rectT<double, double> rectd;

typedef recti rect;


}

#endif
