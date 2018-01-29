#pragma once

//#include <GL/glew.h>
#include <SDL/SDL.h>
#include <string>
#include "glewwrapper.hpp"



class SDL {


	SDL_Window *sdlWindow;
	SDL_GLContext glcontext;
	bool initSuccess;

	Uint32 sdl_init_flags = SDL_INIT_TIMER | SDL_INIT_VIDEO;  
	std::string window_name = "Rightware Task";      

	//initial window resolutions


public:

	const Uint32 window_width = 640;
	const Uint32 window_height = 480;

	SDL();
	~SDL();

	bool isOk() const;

	/**
	* \brief Get application window
	* \return Pointer to window information.
	*/
	SDL_Window *getWindow()
	{
		return sdlWindow;
	}
};