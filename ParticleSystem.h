#pragma once

#include "Particle.h"
#include <ctime>
#include <cstdlib>
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <gl/GL.h>
#include <SOIL2.h>
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class ParticleSystem
{
	//gör rotation för systemet och kanske storlek
private:
	int maxNrOfParticals;
	int nrOfParticels;
	Particle** ParticleArr;
	glm::vec3 pos;
	float spwanTimer = 0;
	glm::vec3 gravity;
	float size;
	glm::vec4 color;
	float endSize;
	glm::vec4 endColor;
	float rotationSpeed;
	bool shiftToEnd;


public:
	ParticleSystem(int maxNrOfParticals, glm::vec3 pos, glm::vec4 color, glm::vec3 gravity, float size, float rotationSpeed = 0);
	ParticleSystem(int maxNrOfParticals, glm::vec3 pos, glm::vec4 startColor, glm::vec4 endColor, glm::vec3 gravity,
		float StartSize, float endSize, float rotationSpeed = 0);
	~ParticleSystem();
	 
	void setMaxNrOfPartials(int newValue);
	int getNrOfParticels();
	bool getIsParticalDead(int index);

	bool addPartical(glm::vec3 velocity, float lifeTime, float rotat = 0);
	bool addPartical( glm::vec3 maxVelocity, glm::vec3 minVelocity, float maxLifeTime, float minLifeTime, glm::vec3 MaxDistensfromMidel,
		glm::vec4 maxColor, glm::vec4 minColor, float maxSize, float minSize, float maxRotat, float minRotat);

	glm::vec4 aParticelColor(int index) const;
	void setAParticelColor(int index, glm::vec4 newColor);
	glm::vec3 aParticelPos(int index) const;
	void setParticelPos(glm::vec3 newPos);
	float aParticelSize(int index) const;
	float aParticelRotatio(int index) const;
	glm::mat4 aParticelWorldMatrix(int index);
	glm::mat4 aParticelRotationMatrix(int index);
	glm::mat4 WorldMatrix();

	void update(float dt, float speedToSpwan, int nrToSpwan, float lifeTime, glm::vec3 velocity, float rotat);
	void update(float dt, float speedToSpwan, int nrToSpwan, float maxLifeTime, float minLifeTime, glm::vec3 maxVelocity, 
		glm::vec3 minVelocity, glm::vec3 MaxDistensfromMidel = glm::vec3(0, 0, 0), glm::vec4 maxColor = glm::vec4(1,1,1,1),
		glm::vec4 minColor = glm::vec4(1, 1, 1, 1), float maxSize = 1, float minSize = 1, float maxRotat = 0, float minRotat = 0);
	void moveAll(float dt);

	void killAllParticals();
	void removeDeadParticals();

	void renderParticals(unsigned int *gShaderProgram, unsigned int *gVertexAttribute, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, unsigned int *texture);

private:
	float rndFloatVal(float max, float min);

};



