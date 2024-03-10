#pragma once
#include "Scene.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Transformation.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

enum class ShaderType
{
    flat,
    gouraud,
    phong
};

Vector3f bilinear_lerp(const Vector2f& UVind, const std::vector<Vector3f>& data, const size_t& width, const size_t& height);

Vector3f trilinear_lerp(const Vector2f& UVind, const Texture& tex, const float& depth);

void flat_shader(Scene& scene);

void gouraud_shader(Scene& scene);

Vector3f phong_shader(const Triangle& tri, const Vector3f& pos, const Vector3f& normal, const Vector2f UVInd, const Material& mtl, const float& depth, const Vector3f& view_position, const std::vector<Light>& lights);