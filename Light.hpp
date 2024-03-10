#pragma once
#include "Vector.hpp"
#include "Matrix.hpp"

class Light
{
public:
	Light();
	~Light();

    Vector3f position;
    Vector3f intensity;
    Matrix M_projection;
    Matrix M_viewport;
    std::vector<float> shadowMap;
    size_t width, height;

private:

};