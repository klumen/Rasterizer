#pragma once
#include <string>
#include <fstream>
#include "Vector.hpp"
#include <vector>
#include <iostream>

class Texture
{
public:
	Texture();
	~Texture();

	void load_texture(const std::string& path);
    std::string format;
    size_t width, height;
    std::vector<Vector3f> data;
    std::vector<float> alphaData;
    std::vector<std::vector<Vector3f>> mipmap;

private:
    void generate_mipmap();

};