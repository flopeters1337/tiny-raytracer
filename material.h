#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"

struct SMaterial
{
    SMaterial(const SVec3f& Color) : DiffuseColor(Color) {}
    SMaterial() : DiffuseColor() {}
    SVec3f DiffuseColor;
};

#endif // __MATERIAL_H__