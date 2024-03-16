
#include <fstream>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "sphere.h"

constexpr float Z_CLIP = 1000.f;

void WriteBufferToFile(const std::vector<Vec3f>& Framebuffer, const size_t& Width, const size_t& Height)
{
    std::ofstream OutputFileStream;
    OutputFileStream.open("./out.ppm", std::ofstream::out | std::ofstream::binary);
    OutputFileStream << "P6\n" << Width << " " << Height << "\n255\n";
    for (size_t CellIndex = 0; CellIndex < Width * Height; ++CellIndex)
    {
        for (size_t ColorIndex = 0; ColorIndex < 3; ++ColorIndex)
        {
            OutputFileStream << (char) (255 * std::max(0.f, std::min(1.f, Framebuffer[CellIndex][ColorIndex])));
        }
    }
    OutputFileStream.close();
}

void RenderGradient(const size_t& Width, const size_t& Height)
{
    std::vector<Vec3f> Framebuffer{Width * Height};
    for (size_t RowIndex = 0; RowIndex < Height; ++RowIndex)
    {
        for (size_t ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        {
            Framebuffer[ColumnIndex + RowIndex * Width] = Vec3f{RowIndex / float(Height), ColumnIndex / float(Width), 0.f};
        }
    }
    WriteBufferToFile(Framebuffer, Width, Height);
}

bool SceneIntersect(const Vec3f& Origin, const Vec3f& Direction, const std::vector<SSphere>& Spheres, Vec3f& Hit, Vec3f& Normal, SMaterial& Material)
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
    return ClosestSphereDistance < Z_CLIP;
}

Vec3f Raycast(const Vec3f& Origin, const Vec3f& Direction, const std::vector<SSphere>& Spheres)
{
    const Vec3f BackgroundColor = Vec3f{0.2f, 0.7f, 0.8f};
    Vec3f Hit;
    Vec3f Normal;
    SMaterial Material;

    if (!SceneIntersect(Origin, Direction, Spheres, Hit, Normal, Material))
    {
        return BackgroundColor;
    }

    return Material.DiffuseColor;
}

void RenderSpheres(const std::vector<SSphere>& Spheres, const size_t& Width, const size_t& Height, const float& FOV)
{
    std::vector<Vec3f> Framebuffer{Width * Height};
    const Vec3f CameraOrigin = Vec3f{0.f, 0.f, 0.f};
    for (size_t RowIndex = 0; RowIndex < Height; ++RowIndex)
    {
        for (size_t ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        {
            float x = (2 * (ColumnIndex + 0.5) / (float) Width  - 1) * tan(FOV / 2.0) * Width / (float) Height;
            float y = -(2 * (RowIndex + 0.5) / (float) Height - 1) * tan(FOV / 2.0);
            Vec3f RayDirection = Vec3f{x, y, -1}.Normalize();
            Framebuffer[ColumnIndex + RowIndex * Width] = Raycast(CameraOrigin, RayDirection, Spheres);
        }
    }
    WriteBufferToFile(Framebuffer, Width, Height);
}

void RenderSphere(const SSphere& Sphere, const size_t& Width, const size_t& Height, const float& FOV)
{
    std::vector<SSphere> Spheres = std::vector<SSphere>{Sphere};
    RenderSpheres(Spheres, Width, Height, FOV);
}

int main()
{
    constexpr size_t Width = 1024;
    constexpr size_t Height = 768;
    constexpr float FOV = 70.f;

    const SMaterial GreenMaterial{Vec3f{0.f, 0.6f, 0.2f}};
    const SMaterial RedMaterial{Vec3f{0.7f, 0.1f, 0.2f}};

    std::vector<SSphere> Spheres;
    Spheres.emplace_back(SSphere{Vec3f{-3.f, 0.f, -16.f}, 2.f, GreenMaterial});
    Spheres.emplace_back(SSphere{Vec3f{-1.f, -1.5f, -12.f}, 2.f, RedMaterial});
    Spheres.emplace_back(SSphere{Vec3f{1.5f, -0.5f, -18.f}, 3.f, RedMaterial});
    Spheres.emplace_back(SSphere{Vec3f{7.f, 5.f, -18.f}, 4.f, GreenMaterial});

    RenderSpheres(Spheres, Width, Height, FOV);

    return 0;
}