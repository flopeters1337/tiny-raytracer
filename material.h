#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"

struct SMaterial
{
    SMaterial(const float& Albedo, const SVec3f& Color, const float& SpecularExponent) : Albedo{Albedo}, DiffuseColor{Color}, SpecularExponent{SpecularExponent} {}
    SMaterial() : DiffuseColor() {}
    float Albedo;
    SVec3f DiffuseColor;
    float SpecularExponent = 0.f;
};

#endif // __MATERIAL_H__