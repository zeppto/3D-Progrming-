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

class Model
{
private:
	struct objVecFinal { float x, y, z, xn, yn, zn, u, v; };
	std::vector<objVecFinal> endVal;

	std::string folder;
	std::string objFileName;
	unsigned int texture;

	glm::vec3 color, ambientColor, specularColor = glm::vec3(-1, -1, -1);
	float transparancy;
	std::string texturName = "";
	int haveTexture;

	GLuint gShaderProgram, gVertexAttribute, gVertexBuffer, *normalTexture, *glowTexture;
	glm::vec3 worldPos;
	bool haveNormalMap;
	bool haveGlowMap;

public:
	Model(char* folder, std::string objFileName, glm::vec3 worldPos = glm::vec3(0,0,0)
		, bool haveNormalMap = false, GLuint *normalTexture = 0, bool haveGlowlMap = false, GLuint *glowTexture = 0);
	~Model();
	void setUpModel();
	void ObjRender(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 lightSpaceMatrix, glm::vec3 camraPos, glm::vec3 lightPos,
		float rotation, float positionX, float positionY, float positionZ, GLuint shadowMap, float glowIntensity = 0);
	void ObjShadowRender(glm::mat4 lightSpaceMatrix, GLuint shader);
	GLuint* getVertexBufferOBJ();
	GLuint* getVertexAttributeOBJ();
	GLuint* getShaderProgramOBJ();
	void closeProgram();

private:
	void ObjOpener();
	void createTexture();
};
