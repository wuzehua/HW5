//
// Created by RainZhong on 2019/3/12.
//

#include "../Include/usefulfuctions.h"


float min3(float x, float y, float z)
{
    float result = x < y ? x : y;
    result = result < z ? result : z;
    return result;
}

float max3(float x1,float x2,float x3)
{
    float result = x1 > x2 ? x1 : x2;
    result = result > x3 ? result : x3;
    return result;
}