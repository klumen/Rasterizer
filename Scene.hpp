#pragma once
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include <vector>

class Scene
{

public:
	Scene();
	~Scene();

    void load_light(const Light& l);
    void load_mesh(const Mesh& m);
    void load_camera(const Camera& c);
    std::vector<Mesh> mesh;
    std::vector<Light> light;
    Camera camera;

private:

};