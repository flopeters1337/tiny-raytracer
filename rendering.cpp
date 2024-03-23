
#include <fstream>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "sphere.h"

#include "rendering.h"

constexpr float SmallValue = 1e-3f;
constexpr float ZClip = 1000.f;
constexpr float CheckerboardTileSize = 1000.f;
constexpr size_t MaxCellValue = 255;

void RenderGradient(const size_t& Width, const size_t& Height)
{
    std::vector<SVec3f> Framebuffer{Width * Height};
    for (size_t RowIndex = 0; RowIndex < Height; ++RowIndex)
    {
        for (size_t ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        {
            Framebuffer[ColumnIndex + RowIndex * Width] = SVec3f{RowIndex / float(Height), ColumnIndex / float(Width), 0.f};
        }
    }
    WriteBufferToFile(Framebuffer, Width, Height);
}

void RenderSphere(const SSphere& Sphere, const std::vector<SLight>& LightSources, const size_t& Width, const size_t& Height, const float& FOV)
{
    std::vector<SSphere> Spheres = std::vector<SSphere>{Sphere};
    RenderSpheres(Spheres, LightSources, Width, Height, FOV);
}

void RenderSpheres(const std::vector<SSphere>& Spheres, const std::vector<SLight>& LightSources, const size_t& Width, const size_t& Height, const float& FOV, const size_t& MaxRecursion)
{
    std::vector<SVec3f> Framebuffer{Width * Height};
    const SVec3f CameraOrigin = SVec3f{0.f, 0.f, 0.f};
    for (size_t RowIndex = 0; RowIndex < Height; ++RowIndex)
    {
        for (size_t ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        {
            float x = (2.f * (ColumnIndex + 0.5f) / (float) Width  - 1.f) * tan(FOV / 2.f) * Width / (float) Height;
            float y = -(2.f * (RowIndex + 0.5f) / (float) Height - 1.f) * tan(FOV / 2.f);
            SVec3f RayDirection = SVec3f{x, y, -1.f}.Normalize();
            Framebuffer[ColumnIndex + RowIndex * Width] = Raycast(CameraOrigin, RayDirection, Spheres, LightSources, MaxRecursion);
        }
    }
    WriteBufferToFile(Framebuffer, Width, Height);
}

SVec3f Raycast(const SVec3f& Origin, const SVec3f& Direction, const std::vector<SSphere>& Spheres, const std::vector<SLight>& LightSources, const int& MaxRecursion)
{
    const SVec3f BackgroundColor = SVec3f{0.2f, 0.7f, 0.8f};
    SVec3f RaycastColor;
    SVec3f Hit;
    SVec3f Normal;
    SMaterial Material;

    if (MaxRecursion < 0 || !SceneIntersect(Origin, Direction, Spheres, Hit, Normal, Material))
    {
        return BackgroundColor;
    }

    const SVec3f ReflectionDirection = Reflect(Direction, Normal).Normalize();
    const SVec3f ReflectionOrigin = Dot(ReflectionDirection, Normal) < 0.f ? Hit - Normal * SmallValue : Hit + Normal * SmallValue;
    const SVec3f ReflectionColor = Raycast(ReflectionOrigin, ReflectionDirection, Spheres, LightSources, MaxRecursion - 1);
    RaycastColor = RaycastColor + ReflectionColor * Material.ReflectionAlbedo;

    const SVec3f RefractionDirection = Refract(Direction, Normal, Material.RefractiveIndex).Normalize();
    const SVec3f RefractionOrigin = Dot(RefractionDirection, Normal) < 0.f ? Hit - Normal * SmallValue : Hit + Normal * SmallValue;
    const SVec3f RefractionColor = Raycast(RefractionOrigin, RefractionDirection, Spheres, LightSources, MaxRecursion - 1);
    RaycastColor = RaycastColor + RefractionColor * Material.RefractionAlbedo;

    float DiffuseLightIntensity = 0.f;
    float SpecularLightIntensity = 0.f;
    for (const SLight& LightSource : LightSources)
    {
        const SVec3f LightDirection = (LightSource.Position - Hit).Normalize();
        const float LightDistance = (LightSource.Position - Hit).Norm();
        SVec3f ShadowOrigin = Dot(LightDirection, Normal) < 0 ? Hit - Normal * SmallValue : Hit + Normal * SmallValue;
        SVec3f ShadowHit;
        SVec3f ShadowNormal;
        SMaterial ShadowMaterial;
        if(SceneIntersect(ShadowOrigin, LightDirection, Spheres, ShadowHit, ShadowNormal, ShadowMaterial) && ShadowOrigin.DistanceSquared(ShadowHit) < LightDistance * LightDistance)
        {
            continue;
        }

        const float DotProductLightNormal = Dot(LightDirection, Normal);
        DiffuseLightIntensity += LightSource.Intensity * std::max(0.f, DotProductLightNormal);

        const SVec3f ReflectedLightDirection = Reflect(LightDirection, Normal).Normalize();
        SpecularLightIntensity += LightSource.Intensity * powf(std::max(0.f, Dot(ReflectedLightDirection, Direction)), Material.SpecularExponent);
    }

    const SVec3f SpecularLightColor = SVec3f{1.f, 1.f, 1.f};
    RaycastColor = RaycastColor + Material.DiffuseColor * DiffuseLightIntensity * (1.f - Material.Albedo) + SpecularLightColor * SpecularLightIntensity * Material.Albedo;
    return RaycastColor;
}

bool SceneIntersect(const SVec3f& Origin, const SVec3f& Direction, const std::vector<SSphere>& Spheres, SVec3f& Hit, SVec3f& Normal, SMaterial& Material)
{
    float ClosestSphereDistance = std::numeric_limits<float>::max();
    for (const SSphere& Sphere : Spheres)
    {
        float CurrentSphereDistance;
        const bool bRayIntersectsSphere = Sphere.RayIntersect(Origin, Direction, CurrentSphereDistance);
        if (!bRayIntersectsSphere || CurrentSphereDistance >= ClosestSphereDistance)
        {
            continue;
        }
        ClosestSphereDistance = CurrentSphereDistance;
        Hit = Origin + Direction * ClosestSphereDistance;
        Normal = (Hit - Sphere.Center).Normalize();
        Material = Sphere.Material;
    }
    
    const SVec3f LightTileColor = SVec3f{1.f, 1.f, 1.f};
    const SVec3f DarkTileColor = SVec3f{0.54f, 0.168f, 0.886f};
    float CheckerboardDistance = std::numeric_limits<float>::max();
    if (fabs(Direction.y) > SmallValue)
    {
        const float Distance = -(Origin.y + 4) / Direction.y;
        const SVec3f Point = Origin + Direction * Distance;
        if (Distance > 0.f && fabs(Point.x) < 10.f && Point.z < -10.f && Point.z > -30.f && Distance < ClosestSphereDistance)
        {
            CheckerboardDistance = Distance;
            Hit = Point;
            Normal = SVec3f{0.f, 1.f, 0.f};
            const bool bHitLightTile = (int(Hit.x + CheckerboardTileSize) + int(Hit.z)) & 1;
            Material.DiffuseColor = bHitLightTile ? LightTileColor : DarkTileColor;
            Material.DiffuseColor = Material.DiffuseColor * 0.3f;
            Material.Albedo = 0.f;
            Material.RefractionAlbedo = 0.f;
        }
    }

    return ClosestSphereDistance < CheckerboardDistance ? ClosestSphereDistance < ZClip : CheckerboardDistance < ZClip;
}

SVec3f Reflect(const SVec3f& Ray, const SVec3f& Normal)
{
    return Ray - 2.f * Normal * Dot(Ray, Normal);
}

SVec3f Refract(const SVec3f& Ray, const SVec3f& Normal, const float& RefractiveIndexMaterial)
{
    constexpr float RefractiveIndexAir = 1.f;
    SVec3f IncidenceNormal = Normal;
    float CosIncidenceAngle = - std::max(-1.f, std::min(1.f, Dot(IncidenceNormal, Ray)));
    float RefractiveIndexRatio = RefractiveIndexAir / RefractiveIndexMaterial;
    if (CosIncidenceAngle < 0.f)
    {
        IncidenceNormal = - Normal;
        CosIncidenceAngle = - CosIncidenceAngle;
        RefractiveIndexRatio = RefractiveIndexMaterial / RefractiveIndexAir;
    }

    const float CosRefractionAngleSquared = 1.f - RefractiveIndexRatio * RefractiveIndexRatio * (1.f - CosIncidenceAngle * CosIncidenceAngle);
    if (CosRefractionAngleSquared < 0.f)
    {
        return SVec3f{0.f, 0.f, 0.f};
    }
    const SVec3f RefractedRay = RefractiveIndexRatio * Ray + (RefractiveIndexRatio * CosIncidenceAngle - std::sqrt(CosRefractionAngleSquared)) * IncidenceNormal;
    return RefractedRay;
}

void WriteBufferToFile(const std::vector<SVec3f>& Framebuffer, const size_t& Width, const size_t& Height)
{
    std::ofstream OutputFileStream;
    OutputFileStream.open("./out.ppm", std::ofstream::out | std::ofstream::binary);
    OutputFileStream << "P6\n" << Width << " " << Height << "\n255\n";
    for (size_t CellIndex = 0; CellIndex < Width * Height; ++CellIndex)
    {
        for (size_t ColorIndex = 0; ColorIndex < 3; ++ColorIndex)
        {
            OutputFileStream << (char) (MaxCellValue * std::max(0.f, std::min(1.f, Framebuffer[CellIndex][ColorIndex])));
        }
    }
    OutputFileStream.close();
}