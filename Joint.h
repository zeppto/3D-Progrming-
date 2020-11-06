#pragma once
#include <common\assimp\cimport.h>
#include <common\assimp\postprocess.h>
#include <common\assimp\Importer.hpp>
#include <common\assimp\scene.h>

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

class Joint
{
public:
	Joint(int ID, aiString name, std::vector<Joint> children, aiMatrix4x4 transformLocal);
	Joint(const Joint &other);
	~Joint();
	void creatGlobalTransform(aiMatrix4x4 parantGT, aiMesh * mesh, aiNode *rootNode);
	void rootGlobalTransform(aiMesh * mesh, aiNode *rootNode);
	void creatAnimatedTransform(aiMatrix4x4 parantGT);
	void rootAnimatedTransform();
	void updateAnimation(aiAnimation *animation);
	void calceAnimation(aiAnimation *animation);
	aiVector3D calcKeys(aiVectorKey * key, int nrOfKeys , int * currentKey, int * nextKey);
	aiQuaternion calcKeys(aiQuatKey * key, int nrOfKeys, int * currentKey, int * nextKey);
	aiString getName();
	void getMatrixes(std::vector<glm::mat4>* returnMatrixs);

private:
	int ID;
	aiString name;
	std::vector<Joint> children;
	aiMatrix4x4 transformLocal;
	aiMatrix4x4 transformGlobal;
	aiMatrix4x4 inversBindPose;
	aiMatrix4x4 skiningMatrix;
	aiMatrix4x4 offset;
	float animationTimeCurrent;
};
