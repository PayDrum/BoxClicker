#include "scene.hpp"

scene::scene()
{
}


bool scene::init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	cube.load();

	return true;
}



//normal render of the scene
void scene::render() {

	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//we define a light source for the scene, could pass an array for multiple light sources
	lightSource light;
	light.color = glm::vec3(0.3f, 0.6f, 0.9f);
	light.position = glm::vec3(0, 0, 0.3f);

	cube.draw(&viewMat, &projectionMat, &light);
}

//colorpicking render of the scene
void scene::renderColorPicker()
{
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//culling settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	cube.drawMinimalistic(&viewMat, &projectionMat);
}


void scene::resize(GLsizei width, GLsizei height)
{

	// Update OpenGL viewport to match window system's window size
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);

	//set field of view and create projection matrix
	float fov = 45.0f;
	projectionMat = glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

	//set viewMat and calculate camera normal for zoomin/zoomout
	viewMat = glm::lookAt(cameraPosition, cameraCenter, cameraUp);
	cameraNormal = glm::normalize(cameraCenter - cameraPosition);
}


void scene::update()
{
}

bool scene::handleEvent(const SDL_Event &e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			//render the scene for colorpicking
			this->renderColorPicker();

			//object picker
			glm::vec3 colors;
			glReadPixels(e.button.x, this->height - 1 - e.button.y, 1, 1, GL_RGB, GL_FLOAT, &colors);
			if (colors == glm::vec3(0, 0, 0))
				cube.addInstance();
			else if (colors != glm::vec3(1, 1, 1))
				cube.deleteInstance((int)(colors.y * 255.0f));
		}
	}

	if (e.type == SDL_MOUSEWHEEL)
	{
		//zoomin zoomout
		if (e.wheel.y > 0)
			//cameraPosition += zoomScale *glm::vec3(1.0f, 0.0, 0.0);
			cameraPosition += zoomScale* cameraNormal;
		if (e.wheel.y < 0)
			cameraPosition -= zoomScale* cameraNormal;
		//cameraPosition -= zoomScale *glm::vec3(1.0f, 0.0, 0.0);

		viewMat = glm::lookAt(cameraPosition, cameraCenter, cameraUp);
	}

	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case (SDLK_RIGHT):cameraPosition.x += zoomScale; cameraCenter.x += zoomScale;
			break;
		case (SDLK_LEFT):cameraPosition.x -= zoomScale; cameraCenter.x -= zoomScale;
			break;
		case (SDLK_UP):cameraPosition.y += zoomScale; cameraCenter.y += zoomScale;
			break;
		case (SDLK_DOWN):cameraPosition.y -= zoomScale; cameraCenter.y -= zoomScale;
			break;
		}
		viewMat = glm::lookAt(cameraPosition, cameraCenter, cameraUp);

	}
	return true;

}

scene::~scene()
{

	// Clean up everything, buffers and arrays will be cleaned in their respective classes
	glUseProgram(0); // Shader state
}
