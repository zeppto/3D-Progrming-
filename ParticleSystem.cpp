#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxNrOfParticals, glm::vec3 pos, glm::vec4 color, glm::vec3 gravity, float size, float rotationSpeed)
{
	this->rotationSpeed = rotationSpeed;
	this->pos = pos;
	this->color = color;
	this->endColor = color;
	this->gravity = gravity;
	this->size = size;
	this->endSize = size;
	this->maxNrOfParticals = maxNrOfParticals;
	nrOfParticels = 0;
	shiftToEnd = false;
	ParticleArr = new Particle*[maxNrOfParticals];
}

ParticleSystem::ParticleSystem(int maxNrOfParticals, glm::vec3 pos, glm::vec4 startColor, glm::vec4 endColor,
	glm::vec3 gravity, float startSize, float endSize, float rotationSpeed)
{
	this->rotationSpeed = rotationSpeed;
	this->pos = pos;
	this->color = startColor;
	this->endColor = endColor;
	this->gravity = gravity;
	this->size = startSize;
	this->endSize = endSize;
	this->maxNrOfParticals = maxNrOfParticals;
	nrOfParticels = 0;
	shiftToEnd = true;
	ParticleArr = new Particle*[maxNrOfParticals];
}

ParticleSystem::~ParticleSystem()
{
	for (int i = 0; i < nrOfParticels; i++)
		delete ParticleArr[i]; 
	delete[] ParticleArr; 
}

void ParticleSystem::setMaxNrOfPartials(int newValue)
{
	killAllParticals();
	delete[] ParticleArr;
	maxNrOfParticals = newValue;
	ParticleArr = new Particle*[maxNrOfParticals];
}

int ParticleSystem::getNrOfParticels()
{
	return nrOfParticels;
}

bool ParticleSystem::getIsParticalDead(int index)
{
	return ParticleArr[index]->isDead();
}

bool ParticleSystem::addPartical(glm::vec3 velocity, float lifeTime, float rotat)
{
	bool toReturn;
	//if it has reatch its maxCapacety dont spwan
	if (nrOfParticels == maxNrOfParticals)
	{
		toReturn = false;
	}
	else
	{
		toReturn = true;
		ParticleArr[nrOfParticels] = new Particle(glm::vec3(0,0,0), color, velocity, lifeTime, size, rotat, endSize, endColor);
		nrOfParticels++;
	}

	return toReturn;
}

bool ParticleSystem::addPartical(glm::vec3 maxVelocity, glm::vec3 minVelocity, float maxLifeTime, float minLifeTime, glm::vec3  MaxDistensfromMidel,
	glm::vec4 maxColor , glm::vec4 minColor, float maxSize, float minSize, float maxRotat, float minRotat)
{
	bool toReturn;
	//if it has reatch its maxCapacety dont spwan
	if (nrOfParticels == maxNrOfParticals)
	{
		toReturn = false;
	}
	else
	{
		toReturn = true;
		//creats random valus for velocety, lifetime, distens from center, color, size and rotation
		float velocity[3];
		float colorRnd[4];
		float particelPos[3] = { 0, 0, 0};
		int distensArr[3] = { MaxDistensfromMidel.x, MaxDistensfromMidel.y, MaxDistensfromMidel.z };
		for (int i = 0; i < 3; i++)
		{
			if (distensArr[i] <= 0)
				particelPos[i] = 0;
			else
			{
				float randDistens = rand() % distensArr[i] - (distensArr[i] / 2);
				particelPos[i] = randDistens / 100;
			}

			velocity[i] = rndFloatVal(maxVelocity[i], minVelocity[i]);
			colorRnd[i] = rndFloatVal(maxColor[i], minColor[i]);
		}
		float myRotation = maxRotat;
		if (maxRotat != minRotat)
			myRotation = rndFloatVal(maxRotat / 100, minRotat / 100) * 100;
		colorRnd[3] = rndFloatVal(maxColor[3], minColor[3]);
		ParticleArr[nrOfParticels] = new Particle(glm::vec3(particelPos[0], particelPos[1], particelPos[2]),
			glm::vec4(colorRnd[0], colorRnd[1], colorRnd[2], colorRnd[3]),
			glm::vec3(velocity[0], velocity[1], velocity[2]), rndFloatVal(maxLifeTime, minLifeTime), 
			rndFloatVal(maxSize, minSize) , myRotation, endSize, endColor);
		nrOfParticels++;
	} 

	return toReturn;
}

glm::vec4 ParticleSystem::aParticelColor(int index) const
{
	return ParticleArr[index]->getColor();
}

void ParticleSystem::setAParticelColor(int index, glm::vec4 newColor)
{
	ParticleArr[index]->setColor(newColor);
}

glm::vec3 ParticleSystem::aParticelPos(int index) const
{
	return ParticleArr[index]->getPos();
}

void ParticleSystem::setParticelPos(glm::vec3 newPos)
{
	pos = newPos;
}

float ParticleSystem::aParticelSize(int index) const
{
	return ParticleArr[index]->getSize();
}

float ParticleSystem::aParticelRotatio(int index) const
{
	return ParticleArr[index]->getRotation();
}

glm::mat4 ParticleSystem::aParticelWorldMatrix(int index)
{
	//the matrix for a particel
	glm::mat4 world(1.0f);
	world = glm::translate(world, ParticleArr[index]->getPos());
	world = glm::scale(world, glm::vec3(ParticleArr[index]->getSize(), ParticleArr[index]->getSize(), ParticleArr[index]->getSize()));
	return world;
}

glm::mat4 ParticleSystem::aParticelRotationMatrix(int index)
{
	//rotaton matrix can't be a part of wold becuse it meses up bilbording
	glm::mat4 rotation(1.0f);
	rotation = glm::rotate(rotation, glm::radians(ParticleArr[index]->getRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
	return rotation;
}

glm::mat4 ParticleSystem::WorldMatrix()
{
	//the matrix for the particelsysten pos in wold
	glm::mat4 world(1.0f);
	world = glm::translate(world, pos);
	//braks bilbording
	//world = glm::rotate(world, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//havent tested
	//world = glm::scale(world, scale);
	return world;
}

void ParticleSystem::update(float dt, float speedToSpwan, int nrToSpwan, float lifeTime, glm::vec3 velocity, float rotat)
{
	spwanTimer += dt;
	if (spwanTimer > speedToSpwan)
	{
		for (int i = 0; i < nrToSpwan; i++)
			addPartical(velocity, lifeTime);
		spwanTimer = 0;
	}
	moveAll(dt);
	removeDeadParticals();
}

void ParticleSystem::update(float dt, float speedToSpwan, int nrToSpwan, float maxLifeTime, float minLifeTime, glm::vec3 maxVelocity,
	glm::vec3 minVelocity, glm::vec3 MaxDistensfromMidel, glm::vec4 maxColor, glm::vec4 minColor, float maxSize,
	float minSize, float maxRotat, float minRotat)
{

	spwanTimer += dt;
	if (spwanTimer > speedToSpwan)
	{
		for(int i = 0; i < nrToSpwan; i++)
			addPartical( maxVelocity, minVelocity, maxLifeTime, minLifeTime, MaxDistensfromMidel, maxColor, minColor, maxSize,
				minSize, maxRotat, minRotat);
		spwanTimer = 0;
	}
	moveAll(dt);
	removeDeadParticals();
}

void ParticleSystem::moveAll(float dt)
{
	//adding valus then move
	for (int i = 0; i < nrOfParticels; i++)
	{
		ParticleArr[i]->addSpeed(gravity);
		ParticleArr[i]->addRotation(rotationSpeed);
		if (shiftToEnd)
		{
			ParticleArr[i]->addColor(dt);
			ParticleArr[i]->addSize(dt);
		}
		ParticleArr[i]->move(dt);
	}
}

void ParticleSystem::killAllParticals()
{
	for (int i = 0; i < nrOfParticels; i++)
		delete ParticleArr[i];

	nrOfParticels = 0;
}

void ParticleSystem::removeDeadParticals()
{
	int removed = 0;
	for (int i = nrOfParticels - 1; 0 <= i; i--)
		if (ParticleArr[i]->isDead())
		{
			delete ParticleArr[i];
			if (i != nrOfParticels)
				ParticleArr[i] = ParticleArr[nrOfParticels - 1 - removed];
			removed++;
		}
	nrOfParticels -= removed;
}

float ParticleSystem::rndFloatVal(float max, float min)
{
	//creats a random float whit 3 desemals
	float distans = max - min;
	float returnFloat;
	if (distans <= 0)
		returnFloat = max;
	else
	{
		distans *= 1000;
		float randDistens = rand() % (int)distans;
		returnFloat = randDistens / 1000 + min;
	}
	return returnFloat;
}

void ParticleSystem::renderParticals(unsigned int *gShaderProgram, unsigned int *gVertexAttribute, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, unsigned int *texture)
{

	// tell opengl we want to use the gShaderProgram
	glUseProgram(*gShaderProgram);
		
	//bindes texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);
		
	// tell opengl we are going to use the VAO we described earlier
	glBindVertexArray(*gVertexAttribute);
		
	//dot have transparansy yet
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//sending values that change
	for (int i = 0; i < getNrOfParticels(); i++)
	{
		if (!getIsParticalDead(i))
		{
			unsigned int particalColorLoc = glGetUniformLocation(*gShaderProgram, "particalColor");
			glUniform4fv(particalColorLoc, 1, glm::value_ptr(aParticelColor(i)));
			unsigned int particalWorldLoc = glGetUniformLocation(*gShaderProgram, "particalWorld");
			glUniformMatrix4fv(particalWorldLoc, 1, GL_FALSE, glm::value_ptr(aParticelWorldMatrix(i)));
			unsigned int particalRotationLoc = glGetUniformLocation(*gShaderProgram, "particelRotation");
			glUniformMatrix4fv(particalRotationLoc, 1, GL_FALSE, glm::value_ptr(aParticelRotationMatrix(i)));
			unsigned int worldLoc = glGetUniformLocation(*gShaderProgram, "world");
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(WorldMatrix()));
			unsigned int viewLoc = glGetUniformLocation(*gShaderProgram, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			unsigned int projectionLoc = glGetUniformLocation(*gShaderProgram, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			glBindVertexArray(*gVertexAttribute);
			glDrawArrays(GL_POINTS, 0, 1);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
