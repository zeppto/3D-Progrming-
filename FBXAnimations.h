#pragma once
#include "Mesh.h"

class FBXAnimatios
{
public:
	FBXAnimatios(char *path, glm::vec3 worldPos);
	~FBXAnimatios();
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	void FbxRender(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 camraPos, glm::vec3 lightPos);
	void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 camraPos, glm::vec3 lightPos, int index);
		//,float rotation, float positionX, float positionY, float positionZ, GLuint shadowMap, glm::mat4 lightSpaceMatrix);
	GLuint *getShaderProgramFbx();
	void setUpModel();


private:
	std::vector<Mesh> meshes;
	std::vector<int> boneIDs;
	std::vector<float> boneWeights;
	glm::vec3 worldPos;
	std::string directory;
	void loadFBX(std::string path);
	std::vector<Joint> createJointConection(aiNode *joint, aiMesh * mesh);
};
