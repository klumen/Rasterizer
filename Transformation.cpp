#include "Transformation.hpp"

Matrix scale(const float& f)
{
    return Matrix({
        {f, 0.0f, 0.0f, 0.0f},
        {0.0f, f, 0.0f, 0.0f},
        {0.0f, 0.0f, f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f} });
}

Matrix scale(const Vector3f& v)
{
    return Matrix({
        {v.x, 0.0f, 0.0f, 0.0f},
        {0.0f, v.y, 0.0f, 0.0f},
        {0.0f, 0.0f, v.z, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f} });
}

Matrix scale_in_any_axis(const Vector3f& n, const float& k)
{
    return Matrix(3, 3).identity() + (k - 1) * n * n;
}

Matrix translate(const Vector3f& v)
{
    return Matrix({ 
        { 1.0f, 0.0f, 0.0f, v.x },
        { 0.0f, 1.0f, 0.0f, v.y },
        { 0.0f, 0.0f, 1.0f, v.z },
        { 0.0f, 0.0f, 0.0f, 1.0f } });
}

Matrix rotate_around_fixed_axis(const axis& a, float angle)
{
    Matrix R(4, 4);
    angle = angle / 180.0f * PI;

    if (a == axis::z)
    {
        R = { { std::cos(angle), -std::sin(angle), 0.0f, 0.0f},
              { std::sin(angle), std::cos(angle), 0.0f, 0.0f },
              { 0.0f, 0.0f, 1.0f, 0.0f },
              { 0.0f, 0.0f, 0.0f, 1.0f } };
    }
    else if (a == axis::x)
    {
        R = { { 1.0f, 0.0f, 0.0f, 0.0f},
              { 0.0f, std::cos(angle), -std::sin(angle), 0.0f },
              { 0.0f, std::sin(angle), std::cos(angle), 0.0f },
              { 0.0f, 0.0f, 0.0f, 1.0f } };
    }
    else if (a == axis::y)
    {
        R = { { std::cos(angle), 0.0f, std::sin(angle), 0.0f},
              { 0.0f, 1.0f, 0.0f, 0.0f },
              { -std::sin(angle), 0.0f, std::cos(angle), 0.0f },
              { 0.0f, 0.0f, 0.0f, 1.0f } };
    }

    return R;
}

Matrix rotate_around_any_axis(const Vector3f& n, float angle)
{
    Matrix R(3, 3);
    Matrix N(3, 3);
    N = {
        { 0.0f, -n.z, n.y },
        { n.z, 0.0f, -n.x },
        { -n.y, n.x, 0.0f } };
    angle = angle / 180.0f * PI;

    R = (std::cos(angle) * Matrix(3, 3).identity() +
        (1 - std::cos(angle)) * n * n +
        std::sin(angle) * N);

    auto data = R.get_data();

    return Matrix({
        { data[0][0], data[0][1], data[0][2], 0.0f },
        { data[1][0], data[1][1], data[1][2], 0.0f },
        { data[2][0], data[2][1], data[2][2], 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f } });
}

Matrix view_transformation(const Camera& c)
{
    auto pos = c.get_position();
    auto gaze = c.get_gaze_direction();
    auto up = c.get_up_direction();
    Matrix T = translate(Vector3f(-pos.x, -pos.y, -pos.z)), R(3, 3);
    R = {
        { gaze.cross(up).x, gaze.cross(up).y, gaze.cross(up).z },
        { up.x, up.y, up.z },
        { -gaze.x, -gaze.y, -gaze.z } };

    auto data = R.get_data();
    
    Matrix R_t({
        { data[0][0], data[0][1], data[0][2], 0.0f },
        { data[1][0], data[1][1], data[1][2], 0.0f },
        { data[2][0], data[2][1], data[2][2], 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f } });

    return R_t * T;
}

Matrix othographic_projection(const float& right, const float& left, const float& top, const float& bottom, const float& near, const float& far)
{
    return scale(Vector3f(2.0f / (right - left), 2.0f / (top - bottom), 2.0f / (near - far))) * translate(Vector3f(-(right + left) / 2.0f, -(top + bottom) / 2.0f, -(near + far) / 2.0f));
}

Matrix perspective_projection(const Camera& c)
{
    auto FOV = c.get_FOVy();
    auto ratio = c.get_aspect_ratio();
    auto near = c.get_near_plane();
    auto far = c.get_far_plane();

    Matrix M_persToOtho(4, 4);
    M_persToOtho = {
        {near, 0.0f, 0.0f, 0.0f},
        {0.0f, near, 0.0f, 0.0f},
        {0.0f, 0.0f, near + far, -near * far},
        {0.0f, 0.0f, 1.0f, 0.0f} };

    float angle = FOV / 2.0f / 180.0f * PI;
    float top = tan(angle) * (-near);
    float bottom = -top;
    float right = top * ratio;
    float left = -right;
    
    return othographic_projection(right, left, top, bottom, near, far) * M_persToOtho;
}

Matrix viewport_transformation(const float& width, const float& height)
{
    return translate(Vector3f(width / 2.0f, height / 2.0f, 0.0f)) * scale(Vector3f(width / 2.0f, height / 2.0f, 1.0f));
}