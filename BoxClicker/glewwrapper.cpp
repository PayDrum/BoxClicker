#include "glewwrapper.hpp"

glewWrapper::glewWrapper() {
}

bool glewWrapper::init() {

	glewExperimental = enableOpenGLDebugging; // This is a global variable provided by GLEW
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cerr << "SDL::SDL(): glewInit error: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	return true;
	// Initialize GLEW now that we have a context
		// Setting glewExperimental allows OpenGL symbols to be found even when they are not listed in driver's extension list - Needed when using core profiles due to GLEW bugs
		// Depending on the drivers, this might be necessary to get even some of the basic functionality to work.


		// Ignore GL errors caused by GLEW initialization (invalid ENUM)
		// Wait for all OpenGL functions to complete first and then cleate error state.
		//glFinish();
		//while (glGetError() != GL_NO_ERROR);

		/*std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;*/

		// Initialize OpenGL debugging if available (See debugmessagecallback.cpp)
		// Requires SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); above

		/*if (doOpenGLDebug)
		installDebugMessageCallback();*/

		/*{
		int v;
		bool version_ok = true;
}
*/
}
glewWrapper::~glewWrapper() {
}