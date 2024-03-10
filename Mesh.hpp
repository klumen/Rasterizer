#pragma once
#include "Face.hpp"
#include "Vertex.hpp"
#include "Material.hpp"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

class Mesh
{

public:
	Mesh();
	Mesh(const Mesh& initmesh);
	~Mesh();
    
	Mesh& operator=(const Mesh& m);

	void load_mesh(const std::string& root, const std::string& objname);
	void show_information() const;
    std::string objName;
    std::vector<Material> mtllib;
	Vertex vertex;
    Face face;

private:

};

void split_string(const std::string& s, const char& c, std::vector<std::string>& v);
