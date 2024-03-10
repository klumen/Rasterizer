#include "Vertex.hpp"

Vertex::Vertex() {}

Vertex::Vertex(const Vertex& initver) : position(initver.position), normal(initver.normal), UVCoordinate(initver.UVCoordinate), color(initver.color) {}

Vertex::~Vertex() {}

Vertex& Vertex::operator=(const Vertex& v)
{
	if (this == &v)
		return *this;
	normal = v.normal;
	position = v.position;
	UVCoordinate = v.UVCoordinate;
    color = v.color;
	return *this;
}