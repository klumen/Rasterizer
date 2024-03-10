#include "Face.hpp"

Face::Face() {}

Face::Face(const Face& initf) : index(initf.index), color(initf.color), materialIndex(initf.materialIndex){}

Face::~Face() {}

Face& Face::operator=(const Face& f)
{
	if (this == &f)
		return *this;
	index = f.index;
    color = f.color;
	return *this;
}