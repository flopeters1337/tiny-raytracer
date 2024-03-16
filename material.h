#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"

struct SMaterial
{
    SMaterial(const Vec3f& Color) : DiffuseColor(Color) {}
    SMaterial() : DiffuseColor() {}
    Vec3f DiffuseColor;
};

#endif // __MATERIAL_H__