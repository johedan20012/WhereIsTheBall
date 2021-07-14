#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include <gba_types.h>

namespace Math{

    ///Calculates the logarithm base 2 of the number, if number is equal to 0 the function returns 0 instead of throwing an error
    u32 logBase2(u32 number);

    u32 pow(u32 base,u32 exp);
};

#endif // MATHFUNCTIONS_H
