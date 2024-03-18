
#include <fstream>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "sphere.h"

#include "rendering.h"

constexpr float ZClip = 1000.f;
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

void RenderSpheres(const std::vector<SSphere>& Spheres, const std::vector<SLight>& LightSources, const size_t& Width, const size_t& Height, const float& FOV)
{
    std::vector<SVec3f> Framebuffer{Width * Height};
    const SVec3f CameraOrigin = SVec3f{0.f, 0.f, 0.f};
    for (size_t RowIndex = 0; RowIndex < Height; ++RowIndex)
    {
        for (size_t ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        {
            float x = (2 * (ColumnIndex + 0.5) / (float) Width  - 1) * tan(FOV / 2.0) * Width / (float) Height;
            float y = -(2 * (RowIndex + 0.5) / (float) Height - 1) * tan(FOV / 2.0);
            SVec3f RayDirection = SVec3f{x, y, -1}.Normalize();
            Framebuffer[ColumnIndex + RowIndex * Width] = Raycast(CameraOrigin, RayDirection, Spheres, LightSources);
        }
    }
    WriteBufferToFile(Framebuffer, Width, Height);
}

SVec3f Raycast(const SVec3f& Origin, const SVec3f& Direction, const std::vector<SSphere>& Spheres, const std::vector<SLight>& LightSources)
{
    const SVec3f BackgroundColor = SVec3f{0.2f, 0.7f, 0.8f};
    SVec3f Hit;
    SVec3f Normal;
    SMaterial Material;

    if (!SceneIntersect(Origin, Direction, Spheres, Hit, Normal, Material))
    {
        return BackgroundColor;
    }

    float DiffuseLightIntensity = 0.f;
    float SpecularLightIntensity = 0.f;
    for (const SLight& LightSource : LightSources)
    {
        const SVec3f LightDirection = (LightSource.Position - Hit).Normalize();
        const float DotProductLightNormal = Dot(LightDirection, Normal);
        DiffuseLightIntensity += LightSource.Intensity * std::max(0.f, DotProductLightNormal);

        const SVec3f ReflectedLightDirection = Reflect(LightDirection, Normal);
        SpecularLightIntensity += LightSource.Intensity * powf(std::max(0.f, Dot(ReflectedLightDirection, Direction)), Material.SpecularExponent);
    }

    const SVec3f SpecularLightColor = SVec3f{1.f, 1.f, 1.f};
    return Material.DiffuseColor * DiffuseLightIntensity * (1 - Material.Albedo) + SpecularLightColor * SpecularLightIntensity * Material.Albedo;
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
    return ClosestSphereDistance < ZClip;
}

SVec3f Reflect(const SVec3f& Ray, const SVec3f& Normal)
{
    return Ray - 2 * Normal * Dot(Ray, Normal);
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