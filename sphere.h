#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <cmath>
#include "geometry.h"
#include "material.h"

struct SSphere
{
    SSphere(const SVec3f &Center, const float &Radius, const SMaterial& Material) : Center{Center}, Radius{Radius}, Material{Material} {}

    bool RayIntersect(const SVec3f& RayOrigin, const SVec3f& RayDirection, float& Distance) const
    {
        const SVec3f RayToCenter = Center - RayOrigin;

        // Sphere behind the ray's origin.
        if (Dot(RayToCenter, RayDirection) < 0)
        {
            const float RayToCenterNorm = RayToCenter.Norm();
            if (RayToCenterNorm > Radius)
            {
                return false;
            }
            else if (RayToCenterNorm == Radius)
            {
                Distance = 0.f;
                return true;
            }
            else
            {
                const SVec3f Projection = Project(Center, RayOrigin, RayDirection);
                const float ProjectionToCenterDistanceSquared = Projection.DistanceSquared(Center);
                const float ProjectionToIntersectionDistance = std::sqrt(Radius * Radius - ProjectionToCenterDistanceSquared);
                Distance = ProjectionToIntersectionDistance - (Projection - RayOrigin).Norm();
                return true;
            }
        }
        // Sphere in front of the ray's origin.
        else
        {
            const SVec3f Projection = Project(Center, RayOrigin, RayDirection);
            if (Projection.DistanceSquared(Center) > Radius * Radius)
            {
                return false;
            }
            else
            {
                const float ProjectionToCenterDistanceSquared = Projection.DistanceSquared(Center);
                const float ProjectionToIntersectionDistance = std::sqrt(Radius * Radius - ProjectionToCenterDistanceSquared);
                const float RayToCenterNorm = RayToCenter.Norm();
                Distance = RayToCenterNorm > Radius ? (Projection - RayOrigin).Norm() - ProjectionToIntersectionDistance : (Projection - RayOrigin).Norm() + ProjectionToIntersectionDistance;
                return true;
            }
        }
    }

    SVec3f Center;
    float Radius;
    SMaterial Material;
};

#endif //__SPHERE_H__