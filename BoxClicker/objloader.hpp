#pragma once
#include <string>
#include <vector>
//#include <tol/objload.h>
#include <iostream>
#include <glm/glm.hpp>       


class extObj {

	

public:
	//tinyobj::attrib_t attrib;
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;

	bool loadObj(std::string filename);

	std::vector<glm::vec3> verticesList;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};