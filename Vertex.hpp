#pragma once
#include "Vector.hpp"
#include <vector>

class Vertex
{

public:
	Vertex();
	Vertex(const Vertex& initver);
	~Vertex();

	Vertex& operator=(const Vertex& v);
    std::vector<Vector3f> position;
	std::vector<Vector3f> normal;
	std::vector<Vector2f> UVCoordinate;
    std::vector<Vector3f> color;

private:

};