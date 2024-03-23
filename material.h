#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"

struct SMaterial
{
    SMaterial(const float& Albedo, const float& ReflectionAlbedo, const float& RefractionAlbedo, const float& RefractiveIndex, const SVec3f& DiffuseColor, const float& SpecularExponent) 
    :   Albedo{Albedo}, 
        ReflectionAlbedo{ReflectionAlbedo},
        RefractionAlbedo{RefractionAlbedo},
        RefractiveIndex{RefractiveIndex},
        DiffuseColor{DiffuseColor},
        SpecularExponent{SpecularExponent} {}
    SMaterial() : DiffuseColor() {}
    
    float Albedo;
    float ReflectionAlbedo;
    float RefractionAlbedo;
    float RefractiveIndex;
    SVec3f DiffuseColor;
    float SpecularExponent = 0.f;
};

#endif // __MATERIAL_H__