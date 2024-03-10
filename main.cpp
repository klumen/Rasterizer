#include "Global.hpp"
#include "Rasterizer.hpp"
#include "Scene.hpp"
#include "Vector.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transformation.hpp"
#include <iostream>
#include <string>

int main()
{
	std::cout << "Hello Rasterizer" << std::endl;
    std::string root = "D:\\Assets\\Shader\\wuqie\\";
    std::string objname = "wuqie.obj";
    Rasterizer r;
    Camera camera;
    camera.set_position(Vector3f(0.f, 2.f, 10.f));
    Light light;
    light.position = Vector3f(10.f, 10.f, 10.f);
    light.intensity = Vector3f(500.f, 500.f, 500.f);
    Mesh obj1, obj2;
    obj1.load_mesh(root, objname);
    root = "D:\\Assets\\Shader\\";
    objname = "plane.obj";
    obj2.load_mesh(root, objname);
    Scene scene;
    scene.load_light(light);
    scene.load_mesh(obj1);
    scene.load_mesh(obj2);
    scene.load_camera(camera);
    ShaderType model = ShaderType::phong;
    if(model == ShaderType::flat)
        flat_shader(scene);
    else if(model == ShaderType::gouraud)
        gouraud_shader(scene);
    r.draw(scene, Primitive::triangle, model);
    std::string resPath = "D:\\Assets\\Shader\\scene.ppm";
    r.generate_PPM(resPath);
    return 0;
}