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

class Ground
{
public:
	Ground(GLuint *gGroundShader, GLuint *gGroundVertexAttribute, GLuint *gGroundVertexBuffer, GLuint *cobblestoneTexture, GLuint *grassTexture, GLuint *blendTexture, GLuint *dirtTexture);
	~Ground();

	void setUpGround();
	void renderUpdateGround(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 camPos,
		glm::vec3 lightPos, glm::vec3 updateScale, float rotation, float positionX, float positionY, float positionZ);

private:
	struct GroundVertex { float x, y, z; float u, v; };
	
	GLuint *gGroundShader, *gGroundVertexAttribute, *gGroundVertexBuffer;
	GLuint *cobblestoneTexture, *grassTexture, *blendTexture, *dirtTexture;
};