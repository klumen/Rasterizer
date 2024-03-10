#pragma once
#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Transformation.hpp"
#include "Shader.hpp"
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream>

enum class Primitive
{
    line,
    triangle
};

class Rasterizer
{
public:
    Rasterizer();
    Rasterizer(const size_t& w, const size_t& h);
    ~Rasterizer();

    Matrix viewportTrans;

    void draw_line(const Vector3f& begin, const Vector3f& end, const Vector3f& color);
    bool inside_triangle(const Vector3f& p, const std::vector<Vector3f>& tri);
    void rasterize_triangle(const Scene& scene, const Mesh& mesh, const size_t& i, const std::vector<Vector4f>& viewSpacePos, const std::vector<Vector3f>& pos, const ShaderType& model);
    void draw(const Scene& scene, const Primitive& type, const ShaderType& model);
    void generate_PPM(const std::string& path) const;

private:
    std::tuple<float, float, float> get_barycentric_coordinates(const Vector3f& p, const std::vector<Vector3f>& pos);
    float get_length_in_texture(std::map<size_t, Vector2f>& UVBuf, const size_t& x, const size_t& y, const size_t& k) const;
    void set_pixel(const size_t& x, const size_t& y, const Vector3f& color);
    void clear_buff();
    size_t width, height;
    std::vector<Vector3f> frameBuff, mtiSampleBuff;
    std::vector<float> depthBuff;

};