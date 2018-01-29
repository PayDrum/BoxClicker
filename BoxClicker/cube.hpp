#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <vector>
#include <algorithm>
#include <random>
#include <tuple>

#include "lightSource.hpp"
#include "objloader.hpp"
#include "shaderprogram.hpp"


class cube {

private:
	//matrices of the model
	glm::mat4 modelMat;
	glm::mat4 translateMat = glm::mat4(1.0f);
	glm::mat4 scaleMat = glm::mat4(1.0f);
	glm::mat4 rotationMat = glm::mat4(1.0f);
	
	//everything related to buffers and shaders
	GLuint  vboPos, vboNorms, vboInstanceMats;
	GLuint vaoCube;
	ShaderProgram shaderProgram;
	const std::string vertexShaderName = "shaders/cube.vs";
	const std::string fragmentShaderName = "shaders/cube.fs";
	ShaderProgram shaderProgramMin;
	const std::string minimalisticVertexShaderName = "shaders/cubeminimalistic.vs";
	const std::string minimalisticFragmentShaderName = "shaders/cubeminimalistic.fs";


	//cube positions on the screen
	const int rows = 10;
	const int columns = 10;
	std::vector<std::tuple<int, int>> instancePos;
	std::vector<std::tuple<int, int>> instancePosActive;
	void shuffleInstancePositions();
	std::vector<glm::mat4> instanceTranslationMats;

	//our exteranlly loaded cube
	extObj obj;

	void loadGeometry();
	void buildBuffers();
	void buildInstanceBuffer();
	


public:

	cube();
	~cube();


	void load();
	void draw(glm::mat4* , glm::mat4*,lightSource* light);
	void drawMinimalistic(glm::mat4*, glm::mat4*);

	void addInstance();
	void deleteInstance(GLuint index);


};