#ifndef CHASKII_POS_DEF
#define CHASKII_POS_DEF 1

#include "vec3.h"

namespace iiMath {

template <class T>
class posT : public vec3T<T>
{
    public:
        posT():vec3T<T>() {}
        posT( const vec3T<T> &vc ):
            vec3T<T>(vc)  {}

        posT( T x, T y, T z): vec3T<T>(x,y,z) {}
        T latitude()  { return vec3T<T>::x(); }
        T longitude() { return vec3T<T>::y(); }
        T altitude()  { return vec3T<T>::z(); }
        T latitude()  const { return vec3T<T>::x(); }
        T longitude() const { return vec3T<T>::y(); }
        T altitude()  const { return vec3T<T>::z(); }
};

typedef posT<float> posf;
typedef posT<double> posd;
typedef posd pos;

}

#endif
