
#include <fstream>
#include <iostream>
#include <vector>
#include "geometry.h"

void Render(const size_t Width, const size_t Height)
{
    std::vector<Vec3f> Framebuffer{Width * Height};

    for (size_t RowIndex = 0; RowIndex < Height; ++RowIndex)
    {
        for (size_t ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        {
            Framebuffer[ColumnIndex + RowIndex * Width] = Vec3f{RowIndex / float(Height), ColumnIndex / float(Width), 0.f};
        }
    }

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

int main()
{
    constexpr size_t Width = 1024;
    constexpr size_t Height = 768;

    Render(Width, Height);

    return 0;
}