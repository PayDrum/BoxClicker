#include <iostream>
#include <fstream>
#include <sstream>

#include "shaderprogram.hpp"

bool ShaderProgram::loadFile(const std::string &filename, std::string &contents) const
{
	std::ifstream is(filename.c_str(), std::ifstream::binary);

	if (!is.is_open())
	{
		std::cerr << "ShaderProgram::loadFile(): Unable to open '" << filename << "' for reading." << std::endl;
		return false;
	}

	is.seekg(0, std::ios::end);
	contents.resize(is.tellg());
	is.seekg(0, std::ios::beg);
	is.read(&contents[0], contents.size()); 

	return is.good();
}


// Compile a single shader.
bool ShaderProgram::compile(GLenum shadertype, const std::string &source, GLuint &shaderhandle)
{
	// Allocate handle for new shader
	shaderhandle = glCreateShader(shadertype);

	// Set shader source to come from a single string. glShaderSource() supports multiple strings that are concatenated together.
	const GLchar *src = source.c_str();
	glShaderSource(shaderhandle, 1, &src, 0);

	// Try to compile it
	GLint isCompiled;
	glCompileShader(shaderhandle);
	glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		GLint maxlen;
		// Something was wrong. Query for any error/information string lengths. It includes \0 at the end.
		glGetShaderiv(shaderhandle, GL_INFO_LOG_LENGTH, &maxlen);
		std::string errstr;
		errstr.resize(maxlen);
		glGetShaderInfoLog(shaderhandle, maxlen, &maxlen, &errstr[0]);

		std::cerr << "Unable to compile shader: " << std::endl << errstr << std::endl;

		// Release shader
		glDeleteShader(shaderhandle);
		shaderhandle = 0;
		return false;
	}

	return true;
}

ShaderProgram::ShaderProgram() :
	vertexshader(0),
	fragmentshader(0),
	shaderprogramID(0)
{
}

ShaderProgram::ShaderProgram(const std::string &vertexshaderfile, const std::string &fragmentshaderfile) :
	vertexshader(0),
	fragmentshader(0),
	shaderprogramID(0)
{
	load(vertexshaderfile, fragmentshaderfile);
}

ShaderProgram::~ShaderProgram()
{
	if (vertexshader)
		glDeleteShader(vertexshader);
	if (fragmentshader)
		glDeleteShader(fragmentshader);
	if (shaderprogramID)
		glDeleteProgram(shaderprogramID);
}

bool ShaderProgram::load(const std::string &vertexshaderfile, const std::string &fragmentshaderfile)
{
	std::string vs;
	std::string fs;

	// Load shader strings
	if ((!loadFile(vertexshaderfile, vs)) ||
		(!loadFile(fragmentshaderfile, fs)))
	{
		// Unable to load one of the shader sources
		return false;
	}

	// Compile vertex shader
	if (!compile(GL_VERTEX_SHADER, vs, vertexshader))
	{
		std::cerr << "Unable to compile '" << vertexshaderfile << std::endl;
		return false;
	}

	// Compile fragment shader
	if (!compile(GL_FRAGMENT_SHADER, fs, fragmentshader))
	{
		std::cerr << "Unable to compile '" << fragmentshaderfile << std::endl;
		return false;
	}

	// Create shader program from above fragment shaders
	shaderprogramID = glCreateProgram();

	if (shaderprogramID == 0)
	{
		std::cerr << "Unable to create shader program using " << vertexshaderfile << " and " << fragmentshaderfile << std::endl;
		return false;
	}

	// Attach different shader passes to shader program
	glAttachShader(shaderprogramID, vertexshader);
	glAttachShader(shaderprogramID, fragmentshader);


	glLinkProgram(shaderprogramID);

	glDetachShader(shaderprogramID, vertexshader);
	glDeleteShader(vertexshader);
	vertexshader = 0;
	glDetachShader(shaderprogramID, fragmentshader);
	glDeleteShader(fragmentshader);
	fragmentshader = 0;

	return true;
}
