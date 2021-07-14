#include "MathFuntions.h"

namespace Math{
    u32 logBase2(u32 number){
        if(number == 0 ) return number;
        u32 ret = -1;
        while(number){
            number >>= 1;
            ret ++;
        }
        return ret;
    }

    u32 pow(u32 base,u32 exp){
        u32 ret = 1;
        for(u32 i = 1; i<=exp; i++){
            ret *= base;
        }
        return ret;
    }
};
