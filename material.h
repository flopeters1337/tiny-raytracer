#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"

struct SMaterial
{
    SMaterial(const float& Albedo, const float& ReflectiveCoefficient, const SVec3f& Color, const float& SpecularExponent) : Albedo{Albedo}, ReflectiveCoefficient{ReflectiveCoefficient}, DiffuseColor{Color}, SpecularExponent{SpecularExponent} {}
    SMaterial() : DiffuseColor() {}
    float Albedo;
    float ReflectiveCoefficient;
    SVec3f DiffuseColor;
    float SpecularExponent = 0.f;
};

#endif // __MATERIAL_H__