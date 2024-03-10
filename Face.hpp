#pragma once
#include "Vertex.hpp"
#include <vector>

struct Triangle
{
    std::vector<Vector3f> vPos;
    std::vector<Vector2f> vTexCor;
};

class Face
{

public:
    Face();
    Face(const Face& initf);
	~Face();

    Face& operator=(const Face& f);
    std::vector<Vector3f> color;
    std::vector<size_t> materialIndex;
    std::vector<std::vector<Vector3i>> index;

private:

};