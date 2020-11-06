 #include "OBJInporter.h"
#include <string>
#include <fstream>
#include <streambuf>

////temp OBJ code
//string ObjFile("OBJFolder/objInportTest.obj"), special;
//string line;
//ifstream(ObjFile);
//std::istringstream inputString;

Model::Model(char* folder, std::string objFileName, glm::vec3 worldPos, bool haveNormalMap, GLuint *normalTexture, bool haveGlowMap, GLuint *glowTexture)
{
	this->folder = folder;
	this->objFileName = objFileName;
	this->worldPos = worldPos;
	this->normalTexture = normalTexture;
	this->haveNormalMap = haveNormalMap;
	this->haveGlowMap = haveGlowMap;
	this->glowTexture = glowTexture;
	haveTexture = -1;
	ObjOpener();
}

Model::~Model()
{
}

void Model::ObjOpener()
{
	//temp OBJ code
	std::string ObjFile(folder + objFileName), type;
	std::string line;
	std::ifstream myFile(ObjFile);
	
	struct objVecPos { float x, y, z; };
	std::vector<objVecPos> objPos;
	objVecPos vtx;

	struct objVecTextur { float u, v; };
	std::vector<objVecTextur> objUv;
	objVecTextur uv;

	struct objVecNormal { float x, y, z; };
	std::vector<objVecNormal> objNormal;
	objVecNormal norm;

	struct objVec { unsigned int v[3], vt[3], vn[3]; };
	std::vector<objVec> objData;
	objVec face;

	std::string matFileName;

	while (std::getline(myFile, line))
	{
		std::istringstream inputString(line);
		//to crate the material
		if (line.substr(0, 7) == "mtllib ")
		{
			inputString >> type >> matFileName;
		}

		//to crate the objet
		if (line.substr(0, 2) == "v ")
		{
			inputString >> type >> vtx.x >> vtx.y >> vtx.z;
			objPos.push_back(vtx);
		}
		if (line.substr(0, 3) == "vt ")
		{
			inputString >> type >> uv.u >> uv.v;
			objUv.push_back(uv);
		}
		if (line.substr(0, 3) == "vn ")
		{
			inputString >> type >> norm.x >> norm.y >> norm.z;
			objNormal.push_back(norm);
		}
		if (line.substr(0, 2) == "f ")
		{
			//moste klara fyrkanter och trekanter minst 
			//f 1/1/1 2/2/2 4/4/3 3/3/4 <--- en fyrkant
			//f 1/1/1 2/2/2 4/4/3 <------ en trekant
			// 1/1/1 v/vt/vn <------- vilken av dom åvan man ska ta (1 == pos 0)
			//gör bara så att trangulerade obijekt kan hämtas
			inputString >> type;
			char temp;
			for (int i = 0; i < 3; i++)
			{
				inputString >> face.v[i] >> temp >> face.vt[i] >> temp >> face.vn[i];
			}
			objData.push_back(face);
		}
	}
	myFile.close();
	for (int i = 0; i < objData.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			objVecFinal aVtx{ objPos[objData[i].v[j]-1].x, objPos[objData[i].v[j]-1].y, objPos[objData[i].v[j]-1].z,
				objNormal[objData[i].vn[j]-1].x, objNormal[objData[i].vn[j]-1].y, objNormal[objData[i].vn[j]-1].z,
				objUv[objData[i].vt[j]-1].u, objUv[objData[i].vt[j]-1].v };
			endVal.push_back(aVtx);
		}
	}
	//open material
	std::string matrialFile(folder + matFileName);
	std::ifstream matFile(matrialFile);

	while (std::getline(matFile, line))
	{
		std::istringstream inputString(line);
		if (line.substr(0, 3) == "Kd ")
		{
			inputString >> type >> color.x >> color.y >> color.z;
		}
		if (line.substr(0, 3) == "Ka ")
		{
			inputString >> type >> ambientColor.x >> ambientColor.y >> ambientColor.z;
		}
		if (line.substr(0, 3) == "Tf ")
		{
			inputString >> type >> transparancy;
		}
		if (line.substr(0, 3) == "Ks ")
		{
			inputString >> type >> specularColor.x >> specularColor.y >> specularColor.z;
		}
		if (line.substr(0, 7) == "map_Kd ")
		{
			inputString >> type >> texturName;
		}
	}
	matFile.close();

	//kd = color
	//ka = ambiantColor
	//Tf = transparansy
	//Ks = specelerColor (finns inte på alla)(gånger ljusets färg)
	//map_Kd  = name of texture (finns inte på alla)
	
}
	
void Model::setUpModel()
{
	// Vertex Array Object (VAO), description of the inputs to the GPU 
		glGenVertexArrays(1, &gVertexAttribute);
		// bind is like "enabling" the object to use it
		glBindVertexArray(gVertexAttribute);
		// this activates the first and second attributes of this VAO
		// think of "attributes" as inputs to the Vertex Shader
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	
	
		// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
		glGenBuffers(1, &gVertexBuffer);
	
		// Bind the buffer ID as an ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	
		// This "could" imply copying to the GPU, depending on what the driver wants to do, and
		// the last argument (read the docs!)
		glBufferData(GL_ARRAY_BUFFER, endVal.size() * sizeof(objVecFinal), &endVal[0], GL_STATIC_DRAW);

	
		// query which "slot" corresponds to the input vertex_position in the Vertex Shader 
		// if this returns -1, it means there is a problem, and the program will likely crash.
		// examples: the name is different or missing in the shader

	
		// tell OpenGL about layout in memory (input assembler information)
		glVertexAttribPointer(
			0,				// location in shader
			3,						// how many elements of type (see next argument)
			GL_FLOAT,				// type of each element
			GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
			sizeof(objVecFinal), // distance between two vertices in memory (stride)
			BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
		);

		//for normals
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(objVecFinal), // distance between two vertexColor 
			BUFFER_OFFSET(sizeof(float) * 3)	// note, the first color starts after the first vertex.
		);
	
		// repeat the process for the second attribute.
		// query which "slot" corresponds to the input vertex_color in the Vertex Shader 
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(objVecFinal), // distance between two vertexColor 
			BUFFER_OFFSET(sizeof(float) * 6)	// note, the first color starts after the first vertex.
		);
		glBindVertexArray(0);

		if (texturName != "")
		{
			createTexture();
			haveTexture = 1;
		}
}

void Model::ObjRender(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 lightSpaceMatrix, glm::vec3 camraPos, glm::vec3 lightPos,
	float rotation, float positionX, float positionY, float positionZ, GLuint shadowMap, float glowIntensity)
{
	glUseProgram(gShaderProgram);
	glUniform1i(glGetUniformLocation(gShaderProgram, "myTexture"), 0);
	glUniform1i(glGetUniformLocation(gShaderProgram, "shadowMap"), 10);
	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram);

	if (texturName != "")
	{
		//bindes texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	if (haveNormalMap)
	{
		glUniform1i(glGetUniformLocation(gShaderProgram, "normalTexture"), 1);

		float haveNormals = 1;
		unsigned int haveNormalsLoc = glGetUniformLocation(gShaderProgram, "haveNormals");
		glUniform1f(haveNormalsLoc, haveNormals);

		//bindes texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *normalTexture);
	}

	if(haveGlowMap)
	{
		glUniform1i(glGetUniformLocation(gShaderProgram, "glowTexture"), 4);

		float haveGlow = 1;
		unsigned int haveGlowLoc = glGetUniformLocation(gShaderProgram, "haveGlow");
		glUniform1f(haveGlowLoc, haveGlow);

		//bindes texture
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, *glowTexture);
	}


	// tell opengl we are going to use the VAO we described earlier
	glBindVertexArray(gVertexAttribute);

	//dot have transparansy yet
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//sending values that change
	glm::mat4 world(1.0f);
	world = glm::translate(world, worldPos);
	world = glm::translate(world, glm::vec3(positionX, positionY, positionZ));
	world = glm::rotate(world, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
	unsigned int worldLoc = glGetUniformLocation(gShaderProgram, "world");
	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
	unsigned int viewLoc = glGetUniformLocation(gShaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	unsigned int projectionLoc = glGetUniformLocation(gShaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	unsigned int lightSpaceLoc = glGetUniformLocation(gShaderProgram, "lightSpace");
	glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	unsigned int lightLoc = glGetUniformLocation(gShaderProgram, "lightPos");
	glUniform3fv(lightLoc, 1, glm::value_ptr(lightPos));
	unsigned int camLoc = glGetUniformLocation(gShaderProgram, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(camraPos));
	unsigned int colorLoc = glGetUniformLocation(gShaderProgram, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	unsigned int ambientColorLoc = glGetUniformLocation(gShaderProgram, "ambientColor");
	glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));
	//-----glow intensity-----//
	unsigned int glow = glGetUniformLocation(gShaderProgram, "glowIntensity");
	glUniform1f(glow, glowIntensity);
	//texture
	unsigned int specColorLoc = glGetUniformLocation(gShaderProgram, "specColor_Textur");
	glm::vec4 in = glm::vec4(specularColor, haveTexture);
	glUniform4fv(specColorLoc, 1, glm::value_ptr(in));
	unsigned int transLoc = glGetUniformLocation(gShaderProgram, "transparency");
	glUniform1f(transLoc, transparancy);
	glDrawArrays(GL_TRIANGLES, 0, endVal.size());

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Model::ObjShadowRender(glm::mat4 lightSpaceMatrix, GLuint shader)
{
	glUseProgram(shader);

	glBindVertexArray(gVertexAttribute);

	glm::mat4 world(1.0f);
	world = glm::translate(world, worldPos);
	unsigned int worldLoc = glGetUniformLocation(shader, "world");
	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
	unsigned int lightLoc = glGetUniformLocation(shader, "lightSpace");
	glUniformMatrix4fv(lightLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glDrawArrays(GL_TRIANGLES, 0, endVal.size());
}

GLuint * Model::getVertexBufferOBJ()
{
	return &gVertexBuffer;
}

GLuint * Model::getVertexAttributeOBJ()
{
	return &gVertexAttribute;
}

GLuint * Model::getShaderProgramOBJ()
{
	return &gShaderProgram;
}

void Model::closeProgram()
{
	glDeleteVertexArrays(1, &gVertexAttribute);
	glDeleteBuffers(1, &gVertexBuffer);
}


void Model::createTexture()
{
	if (texturName != "")
	{
		std::string findTextureFile = folder + texturName;
		int image_width = 0;
		int image_height = 0;
		unsigned char* image = SOIL_load_image(findTextureFile.c_str(), &image_width, &image_height, NULL, SOIL_LOAD_RGBA);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		SOIL_free_image_data(image);
	}
}

