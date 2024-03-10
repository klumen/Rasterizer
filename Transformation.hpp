#pragma once
#include "Global.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Camera.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

enum class axis
{
    x,
    y,
    z
};

Matrix scale(const float& f);

Matrix scale(const Vector3f& v);

Matrix scale_in_any_axis(const Vector3f& n, const float& k);

Matrix translate(const Vector3f& v);

Matrix rotate_around_fixed_axis(const axis& a, float angle);

// Rodrigues’ Rotation Formula
// 浮点数精度丢失问题较大
Matrix rotate_around_any_axis(const Vector3f& n, float angle);

Matrix view_transformation(const Camera& c);

Matrix othographic_projection(const float& right, const float& left, const float& top, const float& bottom, const float& near, const float& far);

Matrix perspective_projection(const Camera& c);

Matrix viewport_transformation(const float& width, const float& height);