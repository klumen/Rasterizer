#include "Scene.hpp"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::load_light(const Light& l) { light.emplace_back(l); }

void Scene::load_mesh(const Mesh& m) { mesh.emplace_back(m); }

void Scene::load_camera(const Camera& c) { camera = c; }