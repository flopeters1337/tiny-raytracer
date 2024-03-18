#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "geometry.h"

struct SLight
{
    SLight(const SVec3f& Position, const float& Intensity) : Position{Position}, Intensity{Intensity} {}
    SVec3f Position;
    float Intensity;
};

#endif // __LIGHT_H__