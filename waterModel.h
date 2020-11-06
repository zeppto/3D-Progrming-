#pragma once
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <vector>
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <gl/GL.h>
#include <SOIL2.h>
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Water
{

public:
	Water(GLuint *gWaterShaderProgram, GLuint *gWaterVertexAttribute, GLuint *gWaterVertexBuffer);
	~Water();

	void setUpWater();
	void updateWaterTime(float deltaTime);
	void renderWater(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 camPos,
		glm::vec3 lightPos, float positonX, float positonY, float positonZ, float rotation);

private:
	struct waterVertex { float x, y, z; };
	float waterTime = 0;
	const float waterSpeed = 0.4;
	GLuint *gWaterShaderProgram, *gWaterVertexAttribute, *gWaterVertexBuffer;
};