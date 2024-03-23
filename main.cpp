
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
    constexpr size_t MaxRecursion = 4;

    const SMaterial OliveSkin{0.8f, 0.1f, 0.f, 1.f, SVec3f{0.f, 0.6f, 0.4f}, 50.f};
    const SMaterial Glass{0.9f, 0.1f, 0.8f, 1.5f, SVec3f{0.6f, 0.7f, 0.8f}, 125.f};
    const SMaterial RedRubber{0.4f, 0.f, 0.f, 1.f, SVec3f{0.4f, 0.f, 0.f}, 10.f};
    const SMaterial Mirror{0.5f, 0.6f, 0.f, 1.f, SVec3f{0.f, 0.f, 0.f}, 1425.f};

    std::vector<SSphere> Spheres;
    Spheres.emplace_back(SSphere{SVec3f{-3.f, 0.f, -16.f}, 2.f, OliveSkin});
    Spheres.emplace_back(SSphere{SVec3f{-1.f, -1.5f, -12.f}, 2.f, Glass});
    Spheres.emplace_back(SSphere{SVec3f{1.5f, -0.5f, -18.f}, 3.f, RedRubber});
    Spheres.emplace_back(SSphere{SVec3f{7.f, 5.f, -18.f}, 4.f, Mirror});

    std::vector<SLight> Lights;
    Lights.emplace_back(SLight{SVec3f{-20.f, 20.f, 20.f}, 1.5f});
    Lights.emplace_back(SLight{SVec3f{30.f, 50.f, -25.f}, 1.8f});
    Lights.emplace_back(SLight{SVec3f{30.f, 20.f, 30.f}, 1.7f});

    RenderSpheres(Spheres, Lights, Width, Height, FOV, MaxRecursion);

    return 0;
}