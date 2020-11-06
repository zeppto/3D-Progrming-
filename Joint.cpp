#include "Joint.h"


Joint::Joint(int ID, aiString name, std::vector<Joint> children, aiMatrix4x4 transformLocal)
{
	this->ID = ID;
	this->name = name;
	this->children = children;
	this->transformLocal = transformLocal;
}

Joint::Joint(const Joint & other)
{
 	this->ID = other.ID;
	this->name = other.name;
	this->transformLocal = other.transformLocal;
	this->children = other.children;
	this->inversBindPose = other.inversBindPose;
	this->transformGlobal = other.transformGlobal;
	this->skiningMatrix = other.skiningMatrix;
	this->offset = other.offset;
}

Joint::~Joint()
{

}
void Joint::creatGlobalTransform(aiMatrix4x4 parantGT, aiMesh * mesh, aiNode *rootNode)
{
	//kan vara tvärt om
	transformGlobal = parantGT * transformLocal;
	//transformGlobal = rootNode->mTransformation.Inverse();
	inversBindPose = rootNode->mTransformation.Inverse();
	//offset = transformGlobal.Inverse();
	offset = mesh->mBones[ID]->mOffsetMatrix;
	for (int i = 0; i < children.size(); i++)
		children[i].creatGlobalTransform(transformGlobal, mesh, rootNode);
}

void Joint::rootGlobalTransform(aiMesh * mesh, aiNode *rootNode)
{
	transformGlobal = transformLocal;
	//transformGlobal = rootNode->mTransformation;
	inversBindPose = rootNode->mTransformation.Inverse();
	//offset = transformGlobal.Inverse();
	offset = mesh->mBones[ID]->mOffsetMatrix;
	for (int i = 0; i < children.size(); i++)
		children[i].creatGlobalTransform(transformLocal, mesh, rootNode);
}

void Joint::creatAnimatedTransform(aiMatrix4x4 parantGT)
{
	transformGlobal = parantGT * transformLocal;
	for (int i = 0; i < children.size(); i++)
		children[i].creatAnimatedTransform(transformGlobal);
	skiningMatrix = inversBindPose * transformGlobal * offset;
}

void Joint::rootAnimatedTransform()
{
	transformGlobal = transformLocal;
	for (int i = 0; i < children.size(); i++)
		children[i].creatAnimatedTransform(transformGlobal);
	skiningMatrix = inversBindPose * transformGlobal * offset;
}

void Joint::updateAnimation(aiAnimation * animation)
{
	//gör bara första jointen först
	//uppdate
	//längd på animation
	//om dt 
	//float curentTime += animation->mTicksPerSecond / dt;
	animationTimeCurrent += animation->mTicksPerSecond / 60;
	if(animation->mDuration < animationTimeCurrent)
		animationTimeCurrent = 0;
	calceAnimation(animation);
	rootAnimatedTransform();
	//aiQuaternion aiRotTest = animation->mChannels[2]->mRotationKeys[1].mValue;
}

void Joint::calceAnimation(aiAnimation * animation)
{
	
	aiMatrix4x4 scale, pos, rotation;
	animationTimeCurrent = animation->mChannels[0]->mPositionKeys[0].mTime;
	int currentKey, nextKey;
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim * nodeAnim = animation->mChannels[i];

		//char test[nodeAnim->mNodeName->length] = nodeAnim->mNodeName.C_Str();
		std::string sNodeName = nodeAnim->mNodeName.C_Str();
		if (sNodeName.substr(0, name.length) == name.C_Str())
		{
			if (sNodeName.substr(name.length, sNodeName.length()) == "_$AssimpFbx$_Translation")
			{
				calcKeys(nodeAnim->mPositionKeys, nodeAnim->mNumPositionKeys, &currentKey, &nextKey);
				//pos calc
				pos.Translation(nodeAnim->mPositionKeys[currentKey].mValue, pos);
			}
			else if (sNodeName.substr(name.length, sNodeName.length()) == "_$AssimpFbx$_Rotation")
			{
				calcKeys(nodeAnim->mRotationKeys, nodeAnim->mNumRotationKeys, &currentKey, &nextKey);

				rotation = aiMatrix4x4(nodeAnim->mRotationKeys[currentKey].mValue.GetMatrix());
			}
			else if (sNodeName.substr(name.length, sNodeName.length()) == "_$AssimpFbx$_Scaling")
			{
				calcKeys(nodeAnim->mRotationKeys, nodeAnim->mNumRotationKeys, &currentKey, &nextKey);

				scale.Scaling(nodeAnim->mScalingKeys[currentKey].mValue, scale);
			}
		}
	}
	//----------------!!!!!!!!!!!!!!!!!!!!!!!!!!!POS!!!!!!!!
	transformLocal = pos * rotation * scale;
	for (int i = 0; i < children.size(); i++)
		children[i].calceAnimation(animation);
}

aiVector3D Joint::calcKeys(aiVectorKey * key, int nrOfKeys, int * currentKey, int * nextKey)
{
	//-för att lista ut vilken key du är på / mellan
	for (int j = 0; j < nrOfKeys; j++)
		if (key[j].mTime <= animationTimeCurrent)
			*currentKey = j;
	*nextKey = *currentKey + 1;
	if (*nextKey > nrOfKeys)
		*nextKey--;
	float percentFromNext = (animationTimeCurrent - key[*currentKey].mTime) / (key[*nextKey].mTime - key[*currentKey].mTime);
	return (1 - percentFromNext) * key[*currentKey].mValue + percentFromNext * key[*nextKey].mValue;
	//----
}

aiQuaternion Joint::calcKeys(aiQuatKey * key, int nrOfKeys, int * currentKey, int * nextKey)
{
	//-för att lista ut vilken key du är på / mellan
	for (int j = 0; j < nrOfKeys; j++)
		if (key[j].mTime <= animationTimeCurrent)
			*currentKey = j;
	*nextKey = *currentKey + 1;
	if (*nextKey > nrOfKeys)
		*nextKey--;
	float percentFromNext = (animationTimeCurrent - key[*currentKey].mTime) / (key[*nextKey].mTime - key[*currentKey].mTime);
	aiQuaternion toReturn;
	toReturn.Interpolate(toReturn, key[*currentKey].mValue, key[*nextKey].mValue, percentFromNext);
	return toReturn ;//glm::mix(key[*currentKey].mValue, key[*nextKey].mValue, percentFromNext);
	//----
}

aiString Joint::getName()
{
	return name;
}

void Joint::getMatrixes(std::vector<glm::mat4>* returnMatrixs )
{
	//behövs e koll?
	//if (returnMatrixs->size() == ID)
	returnMatrixs->push_back(glm::mat4(skiningMatrix.a1, skiningMatrix.a2, skiningMatrix.a2, skiningMatrix.a4,
		skiningMatrix.b1, skiningMatrix.b2, skiningMatrix.b3, skiningMatrix.b4,
		skiningMatrix.c1, skiningMatrix.c2, skiningMatrix.c3, skiningMatrix.c4, 
		skiningMatrix.d1, skiningMatrix.d2, skiningMatrix.d3, skiningMatrix.d4));
	//returnMatrixs.insert(returnMatrixs.begin()+10,transformGlobal);
	for (int i = 0; i < children.size(); i++)
		children[i].getMatrixes(returnMatrixs);

}
