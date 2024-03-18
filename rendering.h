#ifndef __RENDERING_H__
#define __RENDERING_H__

#include <vector>
#include "geometry.h"
#include "light.h"

struct SSphere;
struct SMaterial;

void RenderGradient(const size_t& Width, const size_t& Height);

void RenderSphere(const SSphere& Sphere, const std::vector<SLight>& LightSources, const size_t& Width, const size_t& Height, const float& FOV);

void RenderSpheres(const std::vector<SSphere>& Spheres, const std::vector<SLight>& LightSources, const size_t& Width, const size_t& Height, const float& FOV, const size_t& MaxRecursion = 0);

SVec3f Raycast(const SVec3f& Origin, const SVec3f& Direction, const std::vector<SSphere>& Spheres, const std::vector<SLight>& LightSources, const size_t& MaxRecursion = 0);

bool SceneIntersect(const SVec3f& Origin, const SVec3f& Direction, const std::vector<SSphere>& Spheres, SVec3f& Hit, SVec3f& Normal, SMaterial& Material);

SVec3f Reflect(const SVec3f& Ray, const SVec3f& Normal);

void WriteBufferToFile(const std::vector<SVec3f>& Framebuffer, const size_t& Width, const size_t& Height);

#endif // __RENDERING_H__
