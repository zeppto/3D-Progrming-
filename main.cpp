//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//     - updated by FLL
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <crtdbg.h>
#include <Draw.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

// read docs: https://github.com/g-truc/glm/blob/master/manual.md#section1
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <gl/GL.h>
#include <SOIL2.h>
#include "ParticleSystem.h"
#include "OBJInporter.h"
#include "waterModel.h"
#include "groundModel.h"
#include "FBXAnimations.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

#define WIDTH 900
#define HEIGHT 900
GLFWwindow *gWindow;


void initWindow(unsigned int w, unsigned int h);

// OpenGL uses unsigned integers to keep track of
// created resources (shaders, vertices, textures, etc)
// For simplicity, we make them global here, but it is
// safe to put them in a class and pass around...
GLuint gWaterVertexBuffer = 0;
GLuint gWaterVertexAttribute = 0;
GLuint gWaterShaderProgram = 0;

// full screen quad stuff
GLuint gVertexBufferFS = 0;
GLuint gVertexAttributeFS = 0;
GLuint gShaderProgramFS = 0;

GLuint gGroundVertexBuffer = 0;
GLuint gGroundShader = 0;
GLuint gGroundVertexAttribute = 0;

//particels
GLuint gVertexAttributePartical = 0;
GLuint gShaderProgramPartical = 0;

GLuint gShaderProgramDepth = 0;

//OBJ
//GLuint gVertexBufferOBJ = 0;
//GLuint gVertexAttributeOBJ = 0;
//GLuint gShaderProgramOBJ = 0;

unsigned int particelTextur;
unsigned int particelTextur2;

float gTx, gTy;

float gFloat = 0;
float gClearColour[3] {};

float gUniformColour[3] {};
GLint gUniformColourLoc = -1;

float gOffsetX = 0.0f;
float gIncrement = 0.0f;
float gRotateZ = 0.0f;

//-----glow intensity-----//
float glowIntensity = 0.0f;

//texture
GLuint grassTexture;
GLuint blendTexture;
GLuint dirtTexture;
GLuint cobblestoneTexture;
GLuint testTexture;
GLuint normalMap;
GLuint normalMap1;
GLuint glowMap;



//light
glm::vec3 lightPos(-2.0f, 4.0f, 2.0f);

//move everything in scene
glm::vec3 position(0.0f, 0.0f, 0.0f);
glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::vec3 updateScale(10.0f, 10.0f, 10.0f);

//camera
glm::vec3 camPos(-10.f, 10.f, -12.f);
glm::vec3 upVec(0.f, 1.f, 0.f);
glm::vec3 lookAtVec(0.f, 0.f, -1.f);
glm::mat4 viewMatrix(1.f);

//projection
float FOV = 45.0f;
float aspectRatio = 900 / 900;
float nearPlane = 0.1f;
float farPlane = 100.0f;

//matrix
glm::mat4 projectionMatrix(1.0f);
glm::mat4 worldMatrix(1.0f);
glm::mat4 gRotate2D;

//mouse Update
glm::vec3 directions(0, 0, 0);
glm::vec3 rightVec(0, 0, 0);
float positionTest;
float horizontalAngle = 3.14f*2;
float verticalAngle = -0.3f;
float initialFoV = 45.0f;
float speed = 10.0f;
float mouseSpeed = 0.05;

bool pause = FALSE;
float deltaTime;

// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

static void mouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	if (FOV >= 0.0f && FOV <= 100.0f)
		FOV -= yoffset;
	if (FOV <= 0.0f)
		FOV = 0.0f;
	if (FOV >= 100.0f)
		FOV = 100.0f;
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//move camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camPos += directions * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camPos -= directions * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camPos -= rightVec * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camPos += rightVec * speed * deltaTime;
	//move Object
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		position.z -= speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		position.z += speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		position.x -= speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		position.x += speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		position.y -= speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		position.y += speed * deltaTime;
	//rotate Object
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		rotation.x -= speed + 5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		rotation.x += speed + 5 * deltaTime;
	//-----Increase/Decrease glow-----//
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glowIntensity += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glowIntensity -= 0.1f;
}

static void mouseUpdate(GLFWwindow* window, double xpos, double ypos)
{	
	horizontalAngle += mouseSpeed * deltaTime * float(900/2 -xpos);
	verticalAngle += mouseSpeed * deltaTime * float(900/2 - ypos);

}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	
	Model myModel("OBJFolder/", "orkModell.obj", glm::vec3(10,-1.5,-1), true, &normalMap1);
	Model myCube("OBJFolder/", "objtestTri.obj", glm::vec3(13, -1, 1), true, &normalMap1);
	Model myFloor("OBJFolder/", "floorPlain.obj", glm::vec3(10, -2, 0), true, &normalMap); 
	Model myGlowFloor("OBJFolder/", "floorPlain2.obj", glm::vec3(5, -1, -10), false, 0, true, &glowMap);
	Model myTempLightModel("OBJFolder/", "tempLightCube.obj", lightPos);
	Water myWater(&gWaterShaderProgram, &gWaterVertexAttribute, &gWaterVertexBuffer);
	Ground myGround(&gGroundShader, &gGroundVertexAttribute, &gGroundVertexBuffer, &cobblestoneTexture, &grassTexture, &blendTexture, &dirtTexture);

	FBXAnimatios myFBX("FBXFolder/simpelTest.fbx", glm::vec3(-10, 0, 4));
	
	
	 
	//vet inte om den är på rätt plats VVVV
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(static_cast<unsigned int>(time(0)));
	// crating a particelSystem
	ParticleSystem particalSystem(100, glm::vec3(0, 1, 0), glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 0), glm::vec3(0, -0.001, 0), 0.5f, 0.01f);
	ParticleSystem particalSystem2(100, glm::vec3(0, 0, -2), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 0), glm::vec3(0, -0.002, 0), 0.2f, 0.01f);

	initWindow(WIDTH, HEIGHT);
	glfwSetKeyCallback(gWindow, keyboard);
	glfwSetCursorPosCallback(gWindow, mouseUpdate);
	glfwSetScrollCallback(gWindow, mouseScroll);
	bool shutdown = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGui::StyleColorsDark();
	
	CreateShaders("VertexShaderWater.glsl", "GeometryShaderWater.glsl", "FragmentWater.glsl", &gWaterShaderProgram); //5. Skapa vertex- och fragment-shaders
	//creating particel shader
	CreateShaders("VertexShaderPartical.glsl", "GeometryShaderPartical.glsl", "FragmentPartical.glsl", &gShaderProgramPartical);
	//CreateFSShaders(); //5. Skapa vertex- och fragment-shaders
	CreateShaders("VertexShaderDepth.glsl", "", "FragmentDepth.glsl", &gShaderProgramDepth);
	CreateShaders("VertexShaderObj.glsl", "GeometryShaderObj.glsl", "FragmentObj.glsl", myModel.getShaderProgramOBJ());
	CreateShaders("VertexShaderObj.glsl", "GeometryShaderObj.glsl", "FragmentObj.glsl", myFloor.getShaderProgramOBJ());
	CreateShaders("VertexShaderObj.glsl", "GeometryShaderObj.glsl", "FragmentObj.glsl", myCube.getShaderProgramOBJ());
	CreateShaders("VertexShaderObj.glsl", "GeometryShaderObj.glsl", "FragmentObj.glsl", myTempLightModel.getShaderProgramOBJ());
	CreateShaders("goundVertexShader.glsl", "groundGeometryShader.glsl", "groundFragment.glsl", &gGroundShader);
	CreateShaders("VertexShaderObj.glsl", "GeometryShaderObj.glsl", "FragmentObj.glsl", myGlowFloor.getShaderProgramOBJ());
	//for (int i = 0; i < myFBX.getNrOfMeshes(); i++)
	CreateShaders("VertexShaderFbx.glsl", "GeometryShaderFbx.glsl", "FragmentFbx.glsl", myFBX.getShaderProgramFbx());

	///*if (CreateFrameBuffer() != 0)
	//	shutdown = true;*/
	glGenVertexArrays(1, &gVertexAttributePartical);
	CrateTexture("images/pusheen.png", &particelTextur);
	CrateTexture("images/particel.png", &particelTextur2);
	//ground textures
	CrateTexture("images/cobblestone.png", &cobblestoneTexture);
	CrateTexture("images/grass.png", &grassTexture);
	CrateTexture("images/dirt.png", &dirtTexture);
	CrateTexture("images/blendmap3.png", &blendTexture);
	CrateTexture("images/brickwall_normal.png", &normalMap);
	CrateTexture("images/normalmap.png", &normalMap1);
	CrateTexture("images/glowBloom.png", &glowMap);


	//CreateTriangleData(); //6. Definiera triangelvertiser, 7. Skapa vertex buffer object (VBO), 8.Skapa vertex array object (VAO)
	myFloor.setUpModel();
	myModel.setUpModel();
	myCube.setUpModel();
	myTempLightModel.setUpModel();
	myWater.setUpWater();
	myGround.setUpGround();
	myGlowFloor.setUpModel();
	myFBX.setUpModel();
	//CreateFullScreenQuad();
	
	gUniformColourLoc = glGetUniformLocation(gWaterShaderProgram, "colourFromImGui");

	//viewMatrix
	viewMatrix = glm::lookAt(camPos, camPos + directions, upVec);
	projectionMatrix = glm::perspective(FOV, aspectRatio, nearPlane , farPlane);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int gDepthMapFrameBuffer;
	glGenFramebuffers(1, &gDepthMapFrameBuffer);

	unsigned int gDepthMap;
	glGenTextures(1, &gDepthMap);
	glBindTexture(GL_TEXTURE_2D, gDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//gör så att skugger inte existerar utanför shadowmappens område
	float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, gDepthMapFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, gDepthMapFrameBuffer);
	//glClear(GL_DEPTH_BUFFER_BIT);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(gWindow))
	{
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(gWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(gWindow, 1);
		}

		if (GLFW_PRESS == glfwGetKey(gWindow, GLFW_KEY_ENTER)) 
		{
			pause = TRUE;
		}
		
		
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, 80.0f);
		//addTest flyttar ljuset men ändrar inte skuggorna men om du bara lägger addtest på den första ändras ljuset
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpace = lightProjection * lightView;

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, gDepthMapFrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		myModel.ObjShadowRender(lightSpace, gShaderProgramDepth);
		myCube.ObjShadowRender(lightSpace, gShaderProgramDepth);
		myFloor.ObjShadowRender(lightSpace, gShaderProgramDepth);
		//myTempLightModel.ObjRenderSimpel(lightSpace, gShaderProgramDepth);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		deltaTime = ImGui::GetIO().DeltaTime;
		// move along X
		gIncrement += 1.0f * deltaTime;
		gOffsetX = sin(gIncrement);
		glUniform1f(10, gOffsetX);

		// prepare IMGUI output
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::End();

		

		

		if (pause == FALSE)
		{	
			float first = cos(verticalAngle) * sin(horizontalAngle);
			float second = sin(verticalAngle);
			float third = cos(verticalAngle) * cos(horizontalAngle);
			directions = glm::vec3(first, second, third);

			rightVec = glm::vec3(sin(horizontalAngle - 3.14 / 2.0f), 0, cos(horizontalAngle - 3.14 / 2.0f));
			upVec = glm::cross(rightVec, directions);
			glfwSetCursorPos(gWindow, 900 / 2, 900 / 2);//set cursor to middle of screen
			glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //hide cursor
			viewMatrix = glm::lookAt(camPos, camPos + directions, upVec);
		
			projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
			
		}
			
		myGround.renderUpdateGround(viewMatrix, projectionMatrix, camPos, lightPos, updateScale, rotation.x, 1.0f, - 5.5f, - 4.0f);
		//creats a delta time close to one
		float dt = (1 / deltaTime) / 60;
		float increase = 0.1f;
		/*if (glowIntensity <= 0.0f)
		{
			increase = 0.1f;
		}
		else if (glowIntensity >= 2.0f)
			increase = -0.1f;
		glowIntensity += increase;*/

		//uppdats particel 
		particalSystem.update(dt, 10, 1, 200, 100, glm::vec3(0.05, 0.05, 0), glm::vec3(0.03, 0.03, 0), glm::vec3(10, 0, 5),
			glm::vec4(1, 1, 1, 0.5), glm::vec4(0.5, 0.5, 0.5, 0.5), 0.5f, 0.3f, 90.0f, -90.0f);
		//renders particels
		//ParticelRender(&particalSystem, viewMatrix, projectionMatrix, &particelTextur);
		particalSystem.renderParticals(&gShaderProgramPartical, &gVertexAttributePartical, viewMatrix, projectionMatrix, &particelTextur);

		particalSystem2.update(dt, 15, 2, 100, 100, glm::vec3(0.05, 0.1, 0.05), glm::vec3(-0.05, 0.08, -0.05),
			glm::vec3(0,0,0), glm::vec4(0.5, 0.5, 0.5, 1), glm::vec4(0, 0, 0.5, 1));
		//ParticelRender(&particalSystem2, viewMatrix, projectionMatrix, &particelTextur2);
		particalSystem2.renderParticals(&gShaderProgramPartical, &gVertexAttributePartical, viewMatrix, projectionMatrix, &particelTextur2);

		myCube.ObjRender(viewMatrix, projectionMatrix, lightSpace, camPos, lightPos, 0, 0, 0, 0, gDepthMap);
		myModel.ObjRender(viewMatrix, projectionMatrix, lightSpace, camPos, lightPos, 0, 0, 0, 0, gDepthMap);
		myFloor.ObjRender(viewMatrix, projectionMatrix, lightSpace, camPos, lightPos, rotation.x, position.x, position.y, position.z, gDepthMap);
		myTempLightModel.ObjRender(viewMatrix, projectionMatrix, lightSpace, camPos, lightPos, 0, 0, 0, 0, gDepthMap);
		myGlowFloor.ObjRender(viewMatrix, projectionMatrix, lightSpace, camPos, lightPos, 90, 0, 0, 0, gDepthMap, glowIntensity);
		myFBX.FbxRender(viewMatrix, projectionMatrix, camPos, lightPos);

		myWater.renderWater(viewMatrix, projectionMatrix, camPos, lightPos, 3.0f, - 0.5f, 3.0f, 0);
		myWater.updateWaterTime(deltaTime);
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(gWindow);
	}

	// SHUTDOWN
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	//glDeleteFramebuffers(1, &gFbo);
	//glDeleteTextures(2, gFboTextureAttachments);
	glDeleteVertexArrays(1, &gWaterVertexAttribute);
	glDeleteVertexArrays(1, &gVertexAttributeFS);
	glDeleteVertexArrays(1, &gVertexAttributePartical);
	glDeleteVertexArrays(1, &gGroundVertexAttribute);
	//glDeleteVertexArrays(1, &gVertexAttributeOBJ);
	glDeleteBuffers(1, &gWaterVertexBuffer);
	glDeleteBuffers(1, &gGroundVertexBuffer);
	//glDeleteBuffers(1, &gVertexBufferOBJ);
	glDeleteBuffers(1, &gVertexBufferFS);
	myModel.closeProgram();
	myFloor.closeProgram();
	myCube.closeProgram();
	myTempLightModel.closeProgram();
	myGlowFloor.closeProgram();
	glfwTerminate();

	return 0;

}

void initWindow(unsigned int w, unsigned int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	gWindow = glfwCreateWindow(w, h, "basic GLFW window", NULL, NULL);
	if (!gWindow) {
		fprintf(stderr, "error creating window\n");
		exit(-1);
	}
	glfwMakeContextCurrent(gWindow);
	glewExperimental = GL_TRUE;

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error GLEW: %s\n", glewGetErrorString(err));
	}
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	fprintf(stderr, "Renderer: %s\n", renderer);
	fprintf(stderr, "OpenGL version %s\n", version);
	
	// start up time
	// timerStart = glfwGetTime();
	glClearColor(0.9f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
	glViewport(0, 0, w, h);

	return;
}


