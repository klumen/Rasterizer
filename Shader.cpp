#include "Shader.hpp"

Vector3f bilinear_lerp(const Vector2f& UVind, const std::vector<Vector3f>& data, const size_t& width, const size_t& height)
{
    Vector2f UV(UVind.x * width - 0.5f, UVind.y * height - 0.5f);
    float s = UV.x - std::floor(UV.x), t = UV.y - std::floor(UV.y);

    Vector3f color_00, color_10, color_01, color_11;
    size_t ind_00 = std::floor(UV.y) * width + std::floor(UV.x);
    ind_00 = std::clamp(ind_00, (size_t)0, width * height - 1);
    color_00 = data[ind_00];
    size_t ind_01 = std::ceil(UV.y) * width + std::floor(UV.x);
    ind_01 = std::clamp(ind_01, (size_t)0, width * height - 1);
    color_01 = data[ind_01];
    size_t ind_10 = std::floor(UV.y) * width + std::ceil(UV.x);
    ind_10 = std::clamp(ind_10, (size_t)0, width * height - 1);
    color_10 = data[ind_10];
    size_t ind_11 = std::ceil(UV.y) * width + std::ceil(UV.x);
    ind_11 = std::clamp(ind_11, (size_t)0, width * height - 1);
    color_11 = data[ind_11];

    Vector3f color_0, color_1;
    color_0 = color_00 + s * (color_10 - color_00);
    color_1 = color_01 + s * (color_11 - color_01);

    Vector3f color;
    color = color_0 + t * (color_1 - color_0);

    return color;
}

Vector3f trilinear_lerp(const Vector2f& UVind, const Texture& tex, const float& depth)
{
    if (std::ceil(depth) >= tex.mipmap.size())
        return tex.mipmap.back()[0];

    Vector3f color_0, color_1;
    color_0 = bilinear_lerp(UVind, tex.mipmap[std::floor(depth)], 
        tex.width / std::powf(2, std::floor(depth)), tex.height / std::powf(2, std::floor(depth)));
    color_1 = bilinear_lerp(UVind, tex.mipmap[std::ceil(depth)], 
        tex.width / std::powf(2, std::ceil(depth)), tex.height / std::powf(2, std::ceil(depth)));

    Vector3f color;
    color = color_0 + (depth - std::floor(depth)) * (color_1 - color_0);

    return color;
}

void flat_shader(Scene& scene)
{
    for (auto& mesh_i : scene.mesh)
    {
        mesh_i.face.color.resize(mesh_i.face.index.size());
        for (size_t i = 0; i < mesh_i.face.index.size(); i++)
        {
            Vector3f normal, point;
            Vector2f UVInd;
            for (auto& ind : mesh_i.face.index[i])
            {
                point = point + mesh_i.vertex.position[ind.x] / 3.0f;
                normal = normal + mesh_i.vertex.normal[ind.z] / 3.0f;
                if (mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.width != 0 && mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.height != 0)
                {
                    UVInd = UVInd + mesh_i.vertex.UVCoordinate[ind.y] / 3.f;
                }
            }
            Vector3f n = normal.normalized();
            Vector3f v = (scene.camera.get_position() - point).normalized();
            Vector3f ambient_light_intensity(10.f);
            Vector3f k_a = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Ka, k_d = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Kd, k_s = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Ks;
            float p = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Ns;
            if (mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.width != 0 && mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.height != 0)
            {
                k_d = bilinear_lerp(UVInd, mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.data, mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.width, mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.height).cwise(k_d);
            }
            Vector3f resColor;

            for (auto& light : scene.light)
            {
                Vector3f l = (light.position - point).normalized();
                float r = (light.position - point).norm();
                Vector3f h = (l + v).normalized();

                Vector3f l_a = k_a.cwise(ambient_light_intensity);
                Vector3f l_d = k_d.cwise(light.intensity / (r * r)) * std::max(0.f, l.dot(n));
                Vector3f l_s = k_s.cwise(light.intensity / (r * r)) * std::pow(std::max(0.f, h.dot(n)), p);

                resColor = resColor + l_d + l_a + l_s;
            }

            mesh_i.face.color[i] = resColor;
        }
    }

}

void gouraud_shader(Scene& scene)
{
    for (auto& mesh_i : scene.mesh)
    {
        mesh_i.vertex.color.resize(mesh_i.vertex.position.size());
        for (size_t i = 0; i < mesh_i.face.index.size(); i++)
        {
            for (auto& ind : mesh_i.face.index[i])
            {
                if (mesh_i.vertex.color[ind.x].x != 0.f || mesh_i.vertex.color[ind.x].y != 0.f || mesh_i.vertex.color[ind.x].z != 0.f)
                    continue;
                Vector3f point = mesh_i.vertex.position[ind.x];
                Vector2f UVInd = mesh_i.vertex.UVCoordinate[ind.y];
                Vector3f normal = mesh_i.vertex.normal[ind.z];
                Vector3f n = normal.normalized();
                Vector3f v = (scene.camera.get_position() - point).normalized();
                Vector3f ambient_light_intensity(10.f);
                Vector3f k_a = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Ka, k_d = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Kd, k_s = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Ks;
                float p = mesh_i.mtllib[mesh_i.face.materialIndex[i]].Ns;
                if (mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.width != 0 && mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.height != 0)
                {
                    k_d = bilinear_lerp(UVInd, mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.data, mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.width, mesh_i.mtllib[mesh_i.face.materialIndex[i]].mapKd.height).cwise(k_d);
                }
                Vector3f resColor;

                for (auto& light : scene.light)
                {
                    Vector3f l = (light.position - point).normalized();
                    float r = (light.position - point).norm();
                    Vector3f h = (l + v).normalized();

                    Vector3f l_a = k_a.cwise(ambient_light_intensity);
                    Vector3f l_d = k_d.cwise(light.intensity / (r * r)) * std::max(0.f, l.dot(n));
                    Vector3f l_s = k_s.cwise(light.intensity / (r * r)) * std::pow(std::max(0.f, h.dot(n)), p);

                    resColor = resColor + l_d + l_a + l_s;
                }

                mesh_i.vertex.color[ind.x] = resColor;
            }
        }
    }
}

Vector3f phong_shader(const Triangle& tri, const Vector3f& pos, const Vector3f& normal, const Vector2f UVInd, const Material& mtl, const float& depth, const Vector3f& view_position, const std::vector<Light>& lights)
{
    Vector3f n = normal.normalized();
    Vector3f v = (view_position - pos).normalized();
    Vector3f ambient_light_intensity(10.f);
    Vector3f k_a = mtl.Ka, k_d = mtl.Kd, k_s = mtl.Ks;
    float p = mtl.Ns;
    if (mtl.mapKd.width != 0 && mtl.mapKd.height != 0)
    {
        if (depth <= 0.f)
            k_d = bilinear_lerp(UVInd, mtl.mapKd.data, mtl.mapKd.width, mtl.mapKd.height).cwise(k_d);
        else
            k_d = trilinear_lerp(UVInd, mtl.mapKd, depth).cwise(k_d);
    }
    if (mtl.mapBump.width != 0 && mtl.mapBump.height != 0)
    {
        float du = 0.0f, dv = 0.0f;
        if (depth <= 0.f)
        {
            du = (bilinear_lerp(UVInd + Vector2f(1.0f / mtl.mapBump.width, 0.0f), mtl.mapBump.data, mtl.mapBump.width, mtl.mapBump.height).norm()
                - bilinear_lerp(UVInd, mtl.mapBump.data, mtl.mapBump.width, mtl.mapBump.height).norm());
            dv = (bilinear_lerp(UVInd + Vector2f(0.0f, 1.0f / mtl.mapBump.height), mtl.mapBump.data, mtl.mapBump.width, mtl.mapBump.height).norm()
                - bilinear_lerp(UVInd, mtl.mapBump.data, mtl.mapBump.width, mtl.mapBump.height).norm());
        }
        else
        {
            du = (trilinear_lerp(UVInd + Vector2f(1.0f / mtl.mapBump.width, 0.0f), mtl.mapBump, depth).norm()
                - trilinear_lerp(UVInd, mtl.mapBump, depth).norm());
            dv = (trilinear_lerp(UVInd + Vector2f(0.0f, 1.0f / mtl.mapBump.height), mtl.mapBump, depth).norm()
                - trilinear_lerp(UVInd, mtl.mapBump, depth).norm());
        }
        float c1 = 0.5f, c2 = 0.5f;
        Vector3f newNormal(-c1 * du, -c2 * dv, 1.0f);

        Matrix TBN(4, 4);

        Vector3f e_1 = tri.vPos[1] - tri.vPos[0], e_2 = tri.vPos[2] - tri.vPos[0];
        Vector2f UV_1 = tri.vTexCor[1] - tri.vTexCor[0], UV_2 = tri.vTexCor[2] - tri.vTexCor[0];
        float k = 1.0f / (UV_1.x * UV_2.y - UV_2.x * UV_1.y);
        Vector3f T = k * (UV_2.y - UV_2.x) * e_1;
        Vector3f B = k * (UV_1.x - UV_1.y) * e_2;
        TBN = {
            { T.x, B.x, n.x, 0.0f},
            { T.y, B.y, n.y, 0.0f},
            { T.z, B.z, n.z, 0.0f},
            { 0.0f, 0.0f, 0.0f, 1.0f} };

        n = (TBN * newNormal.to_Vector4f(0.0f)).to_Vector3f().normalized();
    }
    Vector3f resColor;

    for (auto& light : lights)
    {
        Vector3f l = (light.position - pos).normalized();
        float r = (light.position - pos).norm();
        Vector3f h = (l + v).normalized();

        Vector3f l_a = k_a.cwise(ambient_light_intensity);
        Vector3f l_d = k_d.cwise(light.intensity / (r * r)) * std::max(0.f, l.dot(n));
        Vector3f l_s = k_s.cwise(light.intensity / (r * r)) * std::pow(std::max(0.f, h.dot(n)), p);
        
        resColor = resColor + l_d + l_a + l_s;
    }

    return resColor;
}
