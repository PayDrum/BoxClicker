#include "cube.hpp"

cube::cube() 
{
	//Hardcoded scale factor for the model. Also modelmat is calculated here at the start because it doesnt change in this case
	this->scaleMat = glm::scale(this->scaleMat, glm::vec3(1.0f)*0.125f);
	this->modelMat = translateMat * rotationMat * scaleMat;

	//Fill out a vector of possible positions for the cubes based on the index
	for (int i = -rows / 2; i <= rows / 2; i++) 
		for (int j = -columns / 2; j <= columns / 2; j++) 
			if (!(i == 0 && j == 0)) 
				instancePos.push_back(std::make_tuple(i, j));

	//Button cube is absolute, so its added seperately
	instanceTranslationMats.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

cube::~cube() 
{
	glUseProgram(0);
	glDeleteBuffers(1, &vboPos);
	glDeleteBuffers(1, &vboNorms);
	glDeleteBuffers(1, &vboInstanceMats);
	glDeleteVertexArrays(1, &vaoCube);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}

void cube::loadGeometry()
{
	//we load a cube object through a seperate obj loader.
	obj.loadObj("cube.obj");
}

void cube::load() 
{
	//we load two shaders, one for colorpicking, one for normal
	shaderProgram.load(vertexShaderName, fragmentShaderName);
	shaderProgramMin.load(minimalisticVertexShaderName, minimalisticFragmentShaderName);

	loadGeometry();
	buildBuffers();
}

void cube::buildBuffers()
{
	//Generate buffers
	glGenBuffers(1, &vboPos);
	glGenBuffers(1, &vboNorms);
	glGenVertexArrays(1, &vaoCube);

	//position buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, obj.verticesList.size() * 3 * sizeof(GLfloat), &obj.verticesList[0], GL_STATIC_DRAW);

	//bind to vao
	glBindVertexArray(vaoCube);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);

	//normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboNorms);
	glBufferData(GL_ARRAY_BUFFER, obj.normals.size() * 3 * sizeof(GLfloat), &obj.normals[0], GL_STATIC_DRAW);

	//bind to vao
	glBindVertexArray(vaoCube);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	buildInstanceBuffer();
}

void cube::buildInstanceBuffer() 
{
	//since we might rebuild the buffers
	if (vboInstanceMats != 0)
		glDeleteBuffers(1, &vboInstanceMats);

	//(re)generate buffer
	glGenBuffers(1, &vboInstanceMats);

	glBindBuffer(GL_ARRAY_BUFFER, vboInstanceMats);
	glBufferData(GL_ARRAY_BUFFER, instanceTranslationMats.size() * 16 * sizeof(GLfloat), &instanceTranslationMats[0], GL_STATIC_DRAW);

	//vao is limited to max 4 per, so we do it 4 times to transfer the translations matrices
	glBindVertexArray(vaoCube);
	for (int i = 2; i < 6; i++)
	{
		glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GL_FLOAT), (const GLvoid*)(sizeof(GLfloat) * (i - 2) * 4));
		glEnableVertexAttribArray(i);
		glVertexAttribDivisor(i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void cube::draw(glm::mat4* viewMat, glm::mat4* projectionMat, lightSource* light)
{

	glUseProgram(shaderProgram.getShaderProgramHandle());

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "viewMat"), 1, GL_FALSE, glm::value_ptr(*viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "projectionMat"), 1, GL_FALSE,glm::value_ptr(*projectionMat));

	//pass the light in uniform, useful for lightsource changing
	glUniform3fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "lightSourcePos"), 1, glm::value_ptr(light->position));
	glUniform3fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "lightColor"), 1, glm::value_ptr(light->color));

	//draw call
	glBindVertexArray(vaoCube);
	glDrawArraysInstanced(GL_TRIANGLES,0, static_cast<GLsizei>(obj.verticesList.size()),instanceTranslationMats.size());
	glBindVertexArray(0);
}

void cube::drawMinimalistic(glm::mat4* viewMat, glm::mat4* projectionMat)
{
	glUseProgram(shaderProgramMin.getShaderProgramHandle());

	glm:: mat4 mvpMat = (*projectionMat) * (*viewMat) * this->modelMat;
	glUseProgram(shaderProgramMin.getShaderProgramHandle());
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramMin.getShaderProgramHandle(), "mvpMat"), 1, GL_FALSE, glm::value_ptr(mvpMat));

	glBindVertexArray(vaoCube);
	glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(obj.verticesList.size()), instanceTranslationMats.size());
	glBindVertexArray(0);
}


void cube::addInstance() 
{
	//we shuffle the positions in the Q, to make the cube adding look cool!
	shuffleInstancePositions();

	if (!instancePos.empty()) 
	{
		//pop a position from the Q, create a translate matrice from it, add it to the active position vector for later retrieval
		std::tuple<int, int> pop = instancePos.back();
		int x = std::get<0>(pop);
		int y = std::get<1>(pop);
		instanceTranslationMats.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(2.2f*x, 2.2f*y, 0.0f)));
		instancePosActive.push_back(pop);

		//rebuild the instance buffers so the new cube gets added
		buildInstanceBuffer();

		//remove the popped tuple from the position vector
		instancePos.pop_back();
	}

}
void cube::deleteInstance(GLuint index) 
{
	//erase the location of the clicked cube from the translation matrices vector
	instanceTranslationMats.erase(instanceTranslationMats.begin() + index);
	//pop the tuple from the active pool of instance locations(these are the index positions of the cubes currently on display)
	std::tuple<int, int> pop = instancePosActive[index-1];
	//push it back to the original pool for unused cube positions
	instancePos.push_back(pop);
	//rebuild the buffer to update for the deleted cube
	buildInstanceBuffer();
	//erase the deleted tuple from the active pool(no longer used)
	instancePosActive.erase(instancePosActive.begin() + index-1);
}

void cube::shuffleInstancePositions() 
{
	//this needs C++ 11 I think. Hopefully this form is not depecrated. 
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(instancePos.begin(), instancePos.end(), g);
}