#pragma once

#include <vector>
#include <glm/glm.hpp>                  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>         

#include "sdlwrapper.hpp"
#include "shaderprogram.hpp"              
#include "cube.hpp"
#include "lightSource.hpp"


class scene 
{
private:
	cube cube;

	glm::mat4 projectionMat;
	glm::mat4 viewMat;

	GLsizei width, height;

	//camera is simplistic
	glm::vec3 cameraPosition= glm::vec3(-0.5f, -0.5f, 1.0f);
	glm::vec3 cameraCenter= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp= glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraNormal;
	GLfloat zoomScale = 0.05f;




public:
	void update();
	void render();
	void renderColorPicker();
	bool handleEvent(const SDL_Event &e);

	scene();
	~scene();
	bool init(void);
	void resize(GLsizei width, GLsizei height);

};
