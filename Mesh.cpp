#include "Mesh.hpp"

Mesh::Mesh() : objName("None") {}

Mesh::Mesh(const Mesh& initmesh) : vertex(initmesh.vertex), face(initmesh.face), objName(initmesh.objName), mtllib(initmesh.mtllib){}

Mesh::~Mesh() {}

Mesh& Mesh::operator=(const Mesh& m)
{
	if (this == &m)
		return *this;
	objName = m.objName;
    mtllib = m.mtllib;
	vertex = m.vertex;
	face = m.face;
	return *this;
}

void split_string(const std::string& s, const char& c, std::vector<std::string>& v)
{
	std::istringstream sin(s);
	std::string single = "";

	while (std::getline(sin, single, c))
		v.push_back(single);
}

void Mesh::load_mesh(const std::string& root, const std::string& obj)
{
    std::string objFile = root + obj;
	std::ifstream ofin(objFile, std::ios::in);
	std::string line = "";

	if (ofin.is_open())
	{
        std::map<std::string, size_t> mtllibId;
        size_t id = 0;
		while (std::getline(ofin, line))
		{
			if (line[0] == 'o')
			{
				objName = line.erase(0, 2);
			}
            else if (line.substr(0, 6) == "mtllib")
            {
                std::string mtllibName = line.erase(0, 7);
                std::string mtllibFile = root + mtllibName;
                std::ifstream mfin(mtllibFile, std::ios::in);
                std::string line1 = "";

                if (mfin.is_open())
                {
                    size_t cnt = 0;
                    while (std::getline(mfin, line1))
                    {
                        if (line1.substr(0, 6) == "newmtl")
                        {
                            Material mtl;
                            mtl.name = line1.erase(0, 7);
                            mtllib.push_back(mtl);
                            mtllibId[mtl.name] = cnt++;
                        }
                        else if (line1[0] == 'N' && line1[1] == 's')
                        {
                            mtllib.back().Ns = std::stof(line1.erase(0, 3));
                        }
                        else if (line1[0] == 'N' && line1[1] == 'i')
                        {
                            mtllib.back().Ni = std::stof(line1.erase(0, 3));
                        }
                        else if (line1[0] == 'd' && line1[1] == ' ')
                        {
                            mtllib.back().d = std::stof(line1.erase(0, 2));
                        }
                        else if (line1.substr(0, 5) == "illum")
                        {
                            mtllib.back().illum = std::stoi(line1.erase(0, 6));
                        }
                        else if (line1[0] == 'K' && line1[1] == 'a')
                        {
                            std::vector<std::string> v;

                            split_string(line1.erase(0, 3), ' ', v);
                            mtllib.back().Ka = Vector3f(std::stof(v[0]), std::stof(v[1]), std::stof(v[2]));
                        }
                        else if (line1[0] == 'K' && line1[1] == 'd')
                        {
                            std::vector<std::string> v;

                            split_string(line1.erase(0, 3), ' ', v);
                            mtllib.back().Kd = Vector3f(std::stof(v[0]), std::stof(v[1]), std::stof(v[2]));
                        }
                        else if (line1[0] == 'K' && line1[1] == 's')
                        {
                            std::vector<std::string> v;

                            split_string(line1.erase(0, 3), ' ', v);
                            mtllib.back().Ks = Vector3f(std::stof(v[0]), std::stof(v[1]), std::stof(v[2]));
                        }
                        else if (line1.substr(0, 6) == "map_Kd")
                        {
                            std::string mapName = line1.erase(0, 7);
                            mtllib.back().mapKd.load_texture(root + mapName);
                        }
                        else if (line1.substr(0, 8) == "map_Bump")
                        {
                            std::string mapName = line1.erase(0, 9);
                            Texture tex;
                            mtllib.back().mapBump.load_texture(root + mapName);
                        }
                    }
                    mfin.close();
                }
                else
                {
                    std::cerr << "Not find mtl file";
                    exit(1);
                }
            }
			else if (line[0] == 'v' && line[1] == ' ')
			{
				std::vector<std::string> v;

				split_string(line.erase(0, 2), ' ', v);
				vertex.position.push_back(Vector3f(std::stof(v[0]), std::stof(v[1]), std::stof(v[2])));
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				std::vector<std::string> v;

				split_string(line.erase(0, 3), ' ', v);
				vertex.normal.push_back(Vector3f(std::stof(v[0]), std::stof(v[1]), std::stof(v[2])));
			}
			else if (line[0] == 'v' && line[1] == 't')
			{
				std::vector<std::string> v;

				split_string(line.erase(0, 3), ' ', v);
				vertex.UVCoordinate.push_back(Vector2f(std::stof(v[0]), std::stof(v[1])));
			}
            else if (line.substr(0, 6) == "usemtl")
            {
                id = mtllibId[line.erase(0, 7)];
            }
			else if (line[0] == 'f' && line[1] == ' ')
			{
				std::vector<std::string> v;

				split_string(line.erase(0, 2), ' ', v);

                std::vector<Vector3i> ind;
				for (auto& sind : v)
				{
					std::vector<std::string> i;

					split_string(sind, '/', i);
					ind.push_back(Vector3i(std::stoi(i[0]) - 1, std::stoi(i[1]) - 1, std::stoi(i[2]) - 1));
				}

                face.materialIndex.push_back(id);
                face.index.push_back(ind);
                ind.clear();
			}
		}
        ofin.close();
	}
	else
	{
		std::cerr << "Not find obj file" << std::endl;
        std::exit(1);
	}
}

void Mesh::show_information() const
{
	std::cout << objName << std::endl;
    std::cout << "Vertex: " << vertex.position.size() << std::endl;
	std::cout << "Vertex Position:" << std::endl;
	for (auto &pos : vertex.position)
		std::cout << pos << std::endl;
	std::cout << "Normal:" << std::endl;
	for (auto &n : vertex.normal)
		std::cout << n << std::endl;
	std::cout << "Texture Coordinate:" << std::endl;
	for (auto &cor : vertex.UVCoordinate)
		std::cout << cor << std::endl;
	std::cout << "Face: " << face.index.size() << std::endl;
    for (auto& mtl_i : face.materialIndex)
        std::cout << mtl_i << " ";
    std::cout << std::endl;
    for (auto& f : face.index)
    {
        std::cout << "f: ";
        for (auto& ind : f)
            std::cout << ind << " ";
        std::cout << std::endl;
    }
}