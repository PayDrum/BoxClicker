//Program entry point

#include <thread>
#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "sdlwrapper.hpp"
#include "glewwrapper.hpp"
#include "scene.hpp"




bool checkOpenGLErrors()
{
	GLenum e;
	bool allOk = true;
	while ((e = glGetError()) != GL_NO_ERROR)
	{
		allOk = false;

		switch (e)
		{
#ifdef GL_INVALID_ENUM
		case GL_INVALID_ENUM:
			std::cerr << "OpenGL error GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument." << std::endl;
			break;
#endif
#ifdef GL_INVALID_VALUE
		case GL_INVALID_VALUE:
			std::cerr << "OpenGL error GL_INVALID_VALUE: A numeric argument is out of range." << std::endl;
			break;
#endif
#ifdef GL_INVALID_OPERATION
		case GL_INVALID_OPERATION:
			std::cerr << "OpenGL error GL_INVALID_OPERATION: The specified operation is not allowed in the current state." << std::endl;
			break;
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << "OpenGL error GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete." << std::endl;
			break;
#endif
#ifdef GL_OUT_OF_MEMORY
		case GL_OUT_OF_MEMORY:
			std::cerr << "OpenGL error GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined now!" << std::endl;
			break;
#endif
#ifdef GL_STACK_UNDERFLOW
		case GL_STACK_UNDERFLOW:
			std::cerr << "OpenGL error GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl;
			break;
#endif
#ifdef GL_STACK_OVERFLOW
		case GL_STACK_OVERFLOW:
			std::cerr << "OpenGL error GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl;
			break;
#endif
		}
	}

	return allOk;
}



int main(int argc, char* args[]) 
{
	SDL sdl;

	bool runRenderLoop = true;
	Uint32 prevTicks = SDL_GetTicks();

	scene mainScene;
	if (!mainScene.init())
	{
		std::cerr << "Unable to init scene." << std::endl;
		return -1;
	}

	if (!checkOpenGLErrors())
	{
		std::cerr << "OpenGL Errors detected during scene.init()" << std::endl;
		return -1;
	}

	mainScene.resize(sdl.window_width, sdl.window_height);

	while (runRenderLoop)
	{
		// Update the scene
		Uint32 curTicks = SDL_GetTicks();
		prevTicks = curTicks;

		// Render the scene
		mainScene.render();

		// Check for any errors that might have happened inside render call.
		runRenderLoop &= checkOpenGLErrors();

		// Display window
		SDL_GL_SwapWindow(sdl.getWindow());

		// Process events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			// Sample event handling code
			switch (e.type)
			{
				// Program window closed etc.
			case SDL_QUIT:
				runRenderLoop = false;
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					mainScene.resize(e.window.data1, e.window.data2);
					break;
				}
				break;
			}

			runRenderLoop &= mainScene.handleEvent(e);
		}

	}
	return 0;
}



