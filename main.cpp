
#include <fstream>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "sphere.h"

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

Vec3f Raycast(const Vec3f& Origin, const Vec3f& Direction, const Sphere& Sphere)
{
    const Vec3f BackgroundColor = Vec3f{0.2f, 0.7f, 0.8f};
    const Vec3f SphereColor = Vec3f{0.4f, 0.4f, 0.3f};
    float SphereDistance = std::numeric_limits<float>::max();
    if (!Sphere.RayIntersect(Origin, Direction, SphereDistance))
    {
        return BackgroundColor;
    }
    return SphereColor;
}

void RenderSphere(const Sphere& Sphere, const size_t& Width, const size_t& Height, const float& FOV)
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
            Framebuffer[ColumnIndex + RowIndex * Width] = Raycast(CameraOrigin, RayDirection, Sphere);
        }
    }
    WriteBufferToFile(Framebuffer, Width, Height);
}

int main()
{
    constexpr size_t Width = 1024;
    constexpr size_t Height = 768;;
    RenderSphere(Sphere{Vec3f{-3.f, 0.f, -16.f}, 2.f}, Width, Height, 70.f);

    return 0;
}