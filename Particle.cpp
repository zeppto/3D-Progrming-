#include "Particle.h"

Particle::Particle(glm::vec3 pos, glm::vec4 color, glm::vec3 velocity, float lifeTime, float size, float rotation, float endSize, glm::vec4 endColer)
{
	this->pos = pos;
	this->color = color;
	this->velocity = velocity;
	this->lifeTime = lifeTime;
	this->size = size;
	this->rotation = rotation;
	dead = false;
	//creats the amount to add to get the final result
	colorToAdd = (endColer - color)/lifeTime;
	sizeToAdd = (endSize - size) / lifeTime;
}

Particle::~Particle()
{
}

void Particle::addSpeed(glm::vec3 accl)
{
	//like a bacic gravety
	velocity += accl;

}

void Particle::addRotation(float rotationSpeed)
{
	rotation += rotationSpeed;
}

void Particle::addSize(float dt)
{
	size += sizeToAdd * dt;
}

void Particle::addColor(float dt)
{
	color += colorToAdd * dt;
}

bool Particle::isDead() const
{
	return dead;
}

void Particle::move(float dt)
{
	pos += velocity* dt;
	// killing particals 
	lifeTime -= dt;
	if (lifeTime < 0)
		dead = true;
}

glm::vec4 Particle::getColor()
{
	return color;
}

void Particle::setColor(glm::vec4 newColor)
{
	color = newColor;
}

glm::vec3 Particle::getPos()
{
	return pos;
}

void Particle::setPos(glm::vec3 newPos)
{
	pos = newPos;
}

float Particle::getSize()
{
	return size;
}

float Particle::getRotation()
{
	return rotation;
}