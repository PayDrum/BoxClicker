#include <iostream>

#include "sdlwrapper.hpp"
#include "glewwrapper.hpp"

SDL::SDL() :
	sdlWindow(0),
	glcontext(0),
	initSuccess(false)
{
	// Initialize SDL
	if (SDL_Init(sdl_init_flags) != 0)
	{
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return;
	}

	//// Initialize SDL_image for texture loading
	//{
	//	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	//	if (IMG_Init(flags) != flags)
	//	{
	//		std::cerr << "SDL::SDL(): Initializing SDL_Image failed: " << IMG_GetError() << std::endl;
	//		return;
	//	}
	//}

	glewWrapper glew;

	// Setup OpenGL state before creating a window
	if (glew.enableOpenGLDebugging)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); // VERY useful for debugging but might affect performance! https://www.opengl.org/wiki/Debug_Output
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glew.ogl_major_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glew.ogl_minor_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1); // Vertical Sync control

	// SDL_GL_DEPTH_SIZE (in bits)
	// SDL_GL_STENCIL_SIZE (in bits)

	// Create window
	// Pos can also be SDL_WINDOWPOS_CENTERED, or SDL_WINDOWPOS_UNDEFINED

	int posX = SDL_WINDOWPOS_CENTERED, posY = SDL_WINDOWPOS_CENTERED;

	sdlWindow = SDL_CreateWindow(window_name.c_str(), posX, posY, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (sdlWindow == 0)
	{
		std::cerr << "SDL::SDL(): Error creating a window: " << SDL_GetError() << std::endl;
		return;
	}

	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(sdlWindow);
	if (glcontext == 0)
	{
		std::cerr << "SDL::SDL(): Unable to create OpenGL context: " << SDL_GetError() << std::endl;
		return;
	}

	glew.init();

	/*std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION: ";
	if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &v))
	std::cout << "error: " << SDL_GetError() << std::endl;
	else
	std::cout << v << std::endl;

	if (v != ogl_major_version)
	version_ok = false;

	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: ";
	if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &v))
	std::cout << "error: " << SDL_GetError() << std::endl;
	else
	std::cout << v << std::endl;

	if (v != ogl_minor_version)
	version_ok = false;

	if (!version_ok)
	{
	std::cerr << "SDL::SDL(): Unable to select OpenGL version " << ogl_major_version << "." << ogl_minor_version << std::endl;
	return;
	}
	}*/

	// Set up global OpenGL state
	//glPixelStorei(GL_PACK_ALIGNMENT, 1); // How data is stored to client memory - byte alignment for textures etc.
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // How data is read from client memory - byte alignment for textures etc.

	// Everything is finally ok
	initSuccess = true;
}

SDL::~SDL()
{
	// Release GL context
	if (glcontext)
		SDL_GL_DeleteContext(glcontext);

	// Release window
	if (sdlWindow)
		SDL_DestroyWindow(sdlWindow);

	// Release SDL_image resources
	//IMG_Quit();

	SDL_Quit();
}

bool SDL::isOk() const
{
	return initSuccess;
}