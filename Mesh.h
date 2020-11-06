#pragma once
#include "Joint.h"

struct vertex
{
	float x, y, z, nx, ny, nz, u, v, weigth[4] = { 0,0,0,0 };
	unsigned int bones[4] = { 0,0,0,0 };
};

class Mesh
{
public:
	Mesh(glm::vec3 worldPos, std::vector<vertex> vertices, Joint * rootJoint);
	~Mesh();
	GLuint *getShaderProgramFbx();
	GLuint *getVertexAttributeFbx();
	GLuint *getVertexBufferFbx();
	int vertexCount();
	void setUpModel();
	void createTexture();
	Joint * getRootJoint();
	void updateJointAnimation(aiAnimation * animation);
	void getJointMatixes(std::vector<glm::mat4>* returnMatrixs);

private:
	GLuint gShaderProgram, gVertexAttribute, gVertexBuffer, *normalTexture;// , *glowTexture;
	glm::vec3 worldPos;
	std::vector<vertex> vertices;
	Joint * rootJoint;
};
