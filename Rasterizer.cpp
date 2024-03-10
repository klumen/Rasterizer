#include "Rasterizer.hpp"

Rasterizer::Rasterizer() : width(1920), height(1080)
{
    frameBuff.resize(width * height);
    mtiSampleBuff.resize(4 * width * height);
    depthBuff.resize(4 * width * height);
    viewportTrans = viewport_transformation(width, height);
}

Rasterizer::Rasterizer(const size_t& w, const size_t& h) : width(w), height(h) 
{
    frameBuff.resize(width * height);
    mtiSampleBuff.resize(4 * width * height);
    depthBuff.resize(4 * width * height);
}

Rasterizer::~Rasterizer() {}

void Rasterizer::set_pixel(const size_t& x, const size_t& y, const Vector3f& color)
{
    size_t ind = y * width + x;
    frameBuff[ind] = color;
}

//Bresenham's line drawing algorithm
void Rasterizer::draw_line(const Vector3f& begin, const Vector3f& end, const Vector3f& color)
{
    int x0 = std::floor(begin.x), x1 = std::floor(end.x);
    int y0 = std::floor(begin.y), y1 = std::floor(end.y);
    if (abs(y1 - y0) <= abs(x1 - x0))
    {
        if (x1 < x0)
        {
            std::swap(x1, x0);
            std::swap(y1, y0);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        if (dy < 0)
        {
            int D = 2 * dy + dx;
            int y = y0;
            for (int x = x0; x <= x1; x++)
            {
                set_pixel(x, y, color);
                if (D < 0)
                {
                    y--;
                    D += 2 * (dy + dx);
                }
                else
                {
                    D += 2 * dy;
                }
            }
        }
        else
        {
            int D = 2 * dy - dx;
            int y = y0;
            for (int x = x0; x <= x1; x++)
            {

                set_pixel(x, y, color);
                if (D > 0)
                {
                    y++;
                    D += 2 * (dy - dx);
                }
                else
                {
                    D += 2 * dy;
                }
            }
        }
    }
    else
    {
        std::swap(x1, y1);
        std::swap(x0, y0);
        if (x1 < x0)
        {
            std::swap(x1, x0);
            std::swap(y1, y0);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        if (dy < 0)
        {
            int D = 2 * dy + dx;
            int y = y0;
            for (int x = x0; x <= x1; x++)
            {
                set_pixel(y, x, color);
                if (D < 0)
                {
                    y--;
                    D += 2 * (dy + dx);
                }
                else
                {
                    D += 2 * dy;
                }
            }
        }
        else
        {
            int D = 2 * dy - dx;
            int y = y0;
            for (int x = x0; x <= x1; x++)
            {
                set_pixel(y, x, color);
                if (D > 0)
                {
                    y++;
                    D += 2 * (dy - dx);
                }
                else
                {
                    D += 2 * dy;
                }
            }
        }
    }
}

bool Rasterizer::inside_triangle(const Vector3f& p, const std::vector<Vector3f>& pos)
{
    Vector3f AP = p - pos[0], BP = p - pos[1], CP = p - pos[2];
    Vector3f AB = pos[1] - pos[0], BC = pos[2] - pos[1], CA = pos[0] - pos[2];
    if ((AP.cross(AB).z >= 0 && BP.cross(BC).z >= 0 && CP.cross(CA).z >= 0) ||
        (AP.cross(AB).z <= 0 && BP.cross(BC).z <= 0 && CP.cross(CA).z <= 0))
    {
        return true;
    }
    return false;
}

std::tuple<float, float, float> Rasterizer::get_barycentric_coordinates(const Vector3f& p, const std::vector<Vector3f>& pos)
{
    float alpha = ((-(p.x - pos[1].x) * (pos[2].y - pos[1].y) + (p.y - pos[1].y) * (pos[2].x - pos[1].x))
        / (-(pos[0].x - pos[1].x) * (pos[2].y - pos[1].y) + (pos[0].y - pos[1].y) * (pos[2].x - pos[1].x)));
    float beta = ((-(p.x - pos[2].x) * (pos[0].y - pos[2].y) + (p.y - pos[2].y) * (pos[0].x - pos[2].x))
        / (-(pos[1].x - pos[2].x) * (pos[0].y - pos[2].y) + (pos[1].y - pos[2].y) * (pos[0].x - pos[2].x)));
    float gamma = 1.f - alpha - beta;

    return { alpha, beta, gamma };
}

float Rasterizer::get_length_in_texture(std::map<size_t, Vector2f>& UVBuf, const size_t& x, const size_t& y, const size_t& k) const
{
    float length = 1.f;
    if (x == 0 || y == 0)
        return length;
    size_t ind = k * width * height + y * width + x;

    if (k == 3)
    {
        size_t ind1 = 2 * width * height + y * width + x;
        size_t ind2 = 1 * width * height + y * width + x;
        if (UVBuf.find(ind1) != UVBuf.end() && UVBuf.find(ind2) != UVBuf.end())
        {
            length = std::max((UVBuf[ind1] - UVBuf[ind]).norm(), (UVBuf[ind2] - UVBuf[ind]).norm());
        }
    }
    else if (k == 2)
    {
        size_t ind1 = 3 * width * height + y * width + x - 1;
        size_t ind2 = y * width + x;
        if (UVBuf.find(ind1) != UVBuf.end() && UVBuf.find(ind2) != UVBuf.end())
        {
            length = std::max((UVBuf[ind1] - UVBuf[ind]).norm(), (UVBuf[ind2] - UVBuf[ind]).norm());
        }
    }
    else if (k == 1)
    {
        size_t ind1 = 3 * width * height + (y - 1) * width + x;
        size_t ind2 = y * width + x;
        if (UVBuf.find(ind1) != UVBuf.end() && UVBuf.find(ind2) != UVBuf.end())
        {
            length = std::max((UVBuf[ind1] - UVBuf[ind]).norm(), (UVBuf[ind2] - UVBuf[ind]).norm());
        }
    }
    else if (k == 0)
    {
        size_t ind1 = width * height + y * width + x - 1;
        size_t ind2 = 2 * width * height + (y - 1) * width + x;
        if (UVBuf.find(ind1) != UVBuf.end() && UVBuf.find(ind2) != UVBuf.end())
        {
            length = std::max((UVBuf[ind1] - UVBuf[ind]).norm(), (UVBuf[ind2] - UVBuf[ind]).norm());
        }
    }

    return length;
}

void Rasterizer::rasterize_triangle(const Scene& scene, const Mesh& mesh, const size_t& i, const std::vector<Vector4f>& viewSpacePos, const std::vector<Vector3f>& pos, const ShaderType& model)
{
    float x_min = std::min(pos[0].x, std::min(pos[1].x, pos[2].x));
    float x_max = std::max(pos[0].x, std::max(pos[1].x, pos[2].x));
    float y_min = std::min(pos[0].y, std::min(pos[1].y, pos[2].y));
    float y_max = std::max(pos[0].y, std::max(pos[1].y, pos[2].y));
    float z_A = viewSpacePos[mesh.face.index[i][0].x].z;
    float z_B = viewSpacePos[mesh.face.index[i][1].x].z;
    float z_C = viewSpacePos[mesh.face.index[i][2].x].z;
    Triangle tri;
    tri.vPos.emplace_back(mesh.vertex.position[mesh.face.index[i][0].x]);
    tri.vPos.emplace_back(mesh.vertex.position[mesh.face.index[i][1].x]);
    tri.vPos.emplace_back(mesh.vertex.position[mesh.face.index[i][2].x]);
    tri.vTexCor.emplace_back(mesh.vertex.UVCoordinate[mesh.face.index[i][0].y]);
    tri.vTexCor.emplace_back(mesh.vertex.UVCoordinate[mesh.face.index[i][1].y]);
    tri.vTexCor.emplace_back(mesh.vertex.UVCoordinate[mesh.face.index[i][2].y]);
    std::map<size_t, Vector2f> UVBuf;

    for (size_t y = std::max(0.f, std::floor(y_min)); y < std::min(std::ceil(y_max), 1.f * height); y++)
        for (size_t x = std::max(0.f, std::floor(x_min)); x < std::min(std::ceil(x_max), 1.f * width); x++)
        {
            float dx[4] = { 0.25f, 0.75f, 0.25f, 0.75f };
            float dy[4] = { 0.25f, 0.25f, 0.75f, 0.75f };
            for (size_t k = 0; k < 4; k++)
            {
                Vector3f p = Vector3f(x * 1.0f + dx[k], y * 1.0f + dy[k], 0.0f);
                if (!inside_triangle(p, pos))
                    continue;
                auto [alpha, beta, gamma] = get_barycentric_coordinates(p, pos);
                float dep = 1.f / (alpha / z_A + beta / z_B + gamma / z_C);
                size_t ind = k * width * height + y * width + x;
                if (depthBuff[ind] < dep)
                {
                    Vector3f color;
                    depthBuff[ind] = dep;
                    if (model == ShaderType::flat)
                    {
                        color = mesh.face.color[i] / 4.f;
                    }
                    else if (model == ShaderType::gouraud)
                    {
                        color = (alpha * mesh.vertex.color[mesh.face.index[i][0].x] + 
                            beta * mesh.vertex.color[mesh.face.index[i][1].x] +
                            gamma * mesh.vertex.color[mesh.face.index[i][2].x]) / 4.f;
                    }
                    else if (model == ShaderType::phong)
                    {
                        Vector3f point = (alpha * mesh.vertex.position[mesh.face.index[i][0].x] / z_A +
                            beta * mesh.vertex.position[mesh.face.index[i][1].x] / z_B +
                            gamma * mesh.vertex.position[mesh.face.index[i][2].x] / z_C) * dep;
                        Vector2f UVInd = (alpha * mesh.vertex.UVCoordinate[mesh.face.index[i][0].y] / z_A +
                            beta * mesh.vertex.UVCoordinate[mesh.face.index[i][1].y] / z_B +
                            gamma * mesh.vertex.UVCoordinate[mesh.face.index[i][2].y] / z_C) * dep;
                        UVBuf[ind] = 512.f * UVInd;
                        float depth = std::max(-1.f, std::log2f(get_length_in_texture(UVBuf, x, y, k)));
                        Vector3f normal = (alpha * mesh.vertex.normal[mesh.face.index[i][0].z] / z_A +
                            beta * mesh.vertex.normal[mesh.face.index[i][1].z] / z_B +
                            gamma * mesh.vertex.normal[mesh.face.index[i][2].z] / z_C) * dep;
                        color = phong_shader(tri, point, normal, UVInd, mesh.mtllib[mesh.face.materialIndex[i]], depth, scene.camera.get_position(), scene.light) / 4.f;
                    }
                    mtiSampleBuff[ind] = color;
                }
            }
        }
    for (size_t x = std::max(0.f, std::floor(x_min)); x < std::min(std::ceil(x_max), 1.f * width); x++)
        for (size_t y = std::max(0.f, std::floor(y_min)); y < std::min(std::ceil(y_max), 1.f * height); y++)
        {
            Vector3f color;
            float dx[4] = { 0.25f, 0.25f, 0.75f, 0.75f };
            float dy[4] = { 0.75f, 0.25f, 0.25f, 0.75f };
            for (size_t k = 0; k < 4; k++)
            {
                size_t ind = k * width * height + y * width + x;
                color = color + mtiSampleBuff[ind];
            }
            set_pixel(x, y, color);
        }
}

void Rasterizer::draw(const Scene& scene, const Primitive& type, const ShaderType& model)
{
    clear_buff();

    Matrix viewTrans = view_transformation(scene.camera);
    Matrix PerProj = perspective_projection(scene.camera);

    for (auto& mesh_i : scene.mesh)
    {
        std::vector<Vector4f> position(mesh_i.vertex.position.size());
        std::vector<Vector4f> viewSpacePos(mesh_i.vertex.position.size());

        for (size_t i = 0; i < mesh_i.vertex.position.size(); i++)
        {
            viewSpacePos[i] = viewTrans * mesh_i.vertex.position[i].to_Vector4f(1.0f);
            position[i] = PerProj * viewSpacePos[i];
        }

        for (auto& pos : position)
        {
            pos = pos / pos.w;
        }

        for (auto& pos : position)
        {
            pos = viewportTrans * pos;
        }

        if (type == Primitive::line)
        {
            for (auto& ind : mesh_i.face.index)
            {
                draw_line(position[ind[0].x].to_Vector3f(), position[ind[1].x].to_Vector3f(), Vector3f(1, 0, 0));
                draw_line(position[ind[1].x].to_Vector3f(), position[ind[2].x].to_Vector3f(), Vector3f(1, 0, 0));
                draw_line(position[ind[2].x].to_Vector3f(), position[ind[0].x].to_Vector3f(), Vector3f(1, 0, 0));
            }
        }

        if (type == Primitive::triangle)
        {
            for (size_t i = 0; i < mesh_i.face.index.size(); i++)
            {
                std::vector<Vector3f> pos(3);
                pos[0] = position[mesh_i.face.index[i][0].x].to_Vector3f();
                pos[1] = position[mesh_i.face.index[i][1].x].to_Vector3f();
                pos[2] = position[mesh_i.face.index[i][2].x].to_Vector3f();
                rasterize_triangle(scene, mesh_i, i, viewSpacePos, pos, model);
            }
        }
    }
}

void Rasterizer::clear_buff()
{
    std::fill(frameBuff.begin(), frameBuff.end(), Vector3f(0.f));
    std::fill(depthBuff.begin(), depthBuff.end(), -std::numeric_limits<float>::infinity());
}

void Rasterizer::generate_PPM(const std::string& path) const
{
    std::ofstream ppm;
    ppm.open(path);

    ppm << "P3\n" << width << " " << height << "\n" << "255\n";
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
        {
            size_t ind = (height - i - 1) * width + j;
            Color color;
            color = { (unsigned char)(255 * std::clamp(frameBuff[ind].x, 0.f, 1.f)), (unsigned char)(255 * std::clamp(frameBuff[ind].y, 0.f, 1.f)), (unsigned char)(255 * std::clamp(frameBuff[ind].z, 0.f, 1.f)) };
            ppm << color.r() << " " << color.g() << " " << color.b() << "\n";
        }
        
    ppm.close();
}