#pragma once
#include <string>
#include <fstream>
#include <streambuf>

//GL math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Particle
{
private:
	glm::vec3 pos;
	glm::vec3 velocity;
	float lifeTime;
	bool dead;
	glm::vec4 color;
	float size;
	float rotation;

	float rotationSpeed;
	float sizeToAdd;
	glm::vec4 colorToAdd;

public:
	Particle(glm::vec3 pos, glm::vec4 color, glm::vec3 velocity, float lifeTime, float size, float rotation, float endSize, glm::vec4 endColer);
	~Particle();

	void addSpeed(glm::vec3 accl);
	void addRotation(float rotationSpeed);
	void addSize(float dt);
	void addColor(float dt);


	bool isDead() const;

	void move(float dt);

	glm::vec4 getColor();
	void setColor(glm::vec4 newColor);
	glm::vec3 getPos();
	void setPos(glm::vec3 newPos);
	float getSize();
    float getRotation();
};