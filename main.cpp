
#include <fstream>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "light.h"
#include "sphere.h"
#include "rendering.h"

int main()
{
    constexpr size_t Width = 1024;
    constexpr size_t Height = 768;
    constexpr float FOV = 70.f;

    const SMaterial GreenMaterial{SVec3f{0.f, 0.6f, 0.2f}};
    const SMaterial RedMaterial{SVec3f{0.7f, 0.1f, 0.2f}};

    std::vector<SSphere> Spheres;
    Spheres.emplace_back(SSphere{SVec3f{-3.f, 0.f, -16.f}, 2.f, GreenMaterial});
    Spheres.emplace_back(SSphere{SVec3f{-1.f, -1.5f, -12.f}, 2.f, RedMaterial});
    Spheres.emplace_back(SSphere{SVec3f{1.5f, -0.5f, -18.f}, 3.f, RedMaterial});
    Spheres.emplace_back(SSphere{SVec3f{7.f, 5.f, -18.f}, 4.f, GreenMaterial});

    std::vector<SLight> Lights;
    Lights.emplace_back(SLight{SVec3f{-20.f, 20.f, 20.f}, 1.5f});

    RenderSpheres(Spheres, Lights, Width, Height, FOV);

    return 0;
}