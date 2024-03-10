#pragma once
#include "Vector.hpp"
#include "Texture.hpp"
#include <string>

class Material
{
public:
    Material();
    ~Material();

    std::string name;
    Vector3f Ka, Kd, Ks;
    float Ns, Ni, d;
    size_t illum;
    Texture mapKd, mapBump;

private:

};