#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Main_MultipleLights.h"
#include "Shader.h"
#include "Camera.h"
#include <SOIL.h>

using namespace std;
using namespace glm;

GLuint cubeVertexPositionBuffer;
GLuint cubeVertexNormalBuffer;
GLuint cubeTextureCoordBuffer;
GLuint lampColorBuffer;

//Object properties
vec3 lightPos(1.2f, 1.0f, 2.0f);

//Camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Properties
GLuint screenWidth = 700, screenHeight = 500;

//Pointers to Shader objects;
Shader *cubeShaderPtr, *lampShaderPtr;

GLuint diffuseMap;
GLuint specularMap;

int main(int argc, char **argv)
{
	//initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW\n");
		return -1;
	}

	//Creating window
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, 0);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "OpenGLTut10 - Lighting", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 400, 200);

	//Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize GLEW
	glewExperimental = true;		// Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(0.75f, 0.52f, 0.3f, 1.0f);		//Desert
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);		//Factory
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		//Horror
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);		//Biochemical Lab

	glEnable(GL_DEPTH_TEST);

	//Setup and compile our shaders
	//need to be after glewInit(), otherwise got error
	Shader cubeShader("CubeVertexShader_MultipleLights.vs", "CubeFragmentShader_MultipleLights.fs");
	cubeShaderPtr = &cubeShader;
	Shader lampShader("LampVertexShader_MultipleLights.vs", "LampFragmentShader_MultipleLights.fs");
	lampShaderPtr = &lampShader;

	initCubeVertexPositionBuffer();
	initCubeVertexNormalBuffer();
	initCubeTextureCoordsBuffer();

	//Load textures
	loadTexture("container2.png", "container2_specular.png");

	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		renderScene();

		glfwSwapBuffers(window);
		glfwPollEvents();
		Do_Movement();
	}

	glfwTerminate();

	return 0;
}

//Method to initialise cube vertex positions and bind to buffer
void initCubeVertexPositionBuffer()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f
	};

	glGenBuffers(1, &cubeVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

//Method to initialise cube vertex normals and bind to buffer
void initCubeVertexNormalBuffer()
{
	GLfloat normals[] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &cubeVertexNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
}

//Method to initialise cube texture coordinates and bind to buffer
void initCubeTextureCoordsBuffer()
{
	GLfloat textCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	glGenBuffers(1, &cubeTextureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeTextureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), textCoords, GL_STATIC_DRAW);
}

void initLampColorBuffer(vec3 pointLightColor)
{
	GLfloat colors[] = {
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,

		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,

		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,

		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,

		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,

		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z,
		pointLightColor.x, pointLightColor.y, pointLightColor.z
	};

	glGenBuffers(1, &lampColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lampColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void renderScene()
{
	GLint viewPosLoc, lightPosLoc, modelLoc, viewLoc, projLoc, matShineLoc, lightAmbientLoc, 
		lightDiffuseLoc, lightSpecularLoc;
	mat4 view, projection, model;
	//Positions of all our containers
	vec3 cubePositions[] = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(2.0f, 5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f, -0.4f, -3.5f),
		vec3(-1.7f, 3.0f, -7.5f),
		vec3(1.3f, -2.0f, -2.5f),
		vec3(1.5f, 2.0f, -2.5f),
		vec3(1.5f, 0.2f, -1.5f),
		vec3(-1.3f, 1.0f, -1.5f)
	};
	//Positions of all the light objects
	vec3 pointLightPositions[] = {
		vec3(0.7f, 0.2f, 2.0f),
		vec3(2.3f, -3.3f, -4.0f),
		vec3(-4.0f, 2.0f, -12.0f),
		vec3(0.0f, 0.0f, -3.0f)
	};

	/*
	//Point light colors for Desert
	vec3 pointLightColors[] = {
		vec3(1.0f, 0.6f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 1.0, 0.0),
		vec3(0.2f, 0.2f, 1.0f)
	};
	*/

	/*
	//Point light colors for Factory
	vec3 pointLightColors[] = {
		vec3(0.2f, 0.2f, 0.6f),
		vec3(0.3f, 0.3f, 0.7f),
		vec3(0.0f, 0.0f, 0.3f),
		vec3(0.4f, 0.4f, 0.4f)
	};
	*/	

	/*
	//Point light colors for Horror
	vec3 pointLightColors[] = {
		vec3(0.1f, 0.1f, 0.1f),
		vec3(0.1f, 0.1f, 0.1f),
		vec3(0.1f, 0.1f, 0.1f),
		vec3(0.3f, 0.1f, 0.1f)
	};
	*/

	
	//Point light colors for Biochemical Lab
	vec3 pointLightColors[] = {
		vec3(0.4f, 0.7f, 0.1f),
		vec3(0.4f, 0.7f, 0.1f),
		vec3(0.4f, 0.7f, 0.1f),
		vec3(0.4f, 0.7f, 0.1f)
	};
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/////CUBE OBJECT/////
	//Use the cube shader
	cubeShaderPtr->Use();
	
	//Setting uniform view position in Fragment Shader
	viewPosLoc = glGetUniformLocation(cubeShaderPtr->Program, "viewPos");
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	/*
	//Directional light
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	//Point light 1
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].quadratic"), 0.032);
	//Point light 2
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].quadratic"), 0.032);
	//Point light 3
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].quadratic"), 0.032);
	//Point light 4
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].quadratic"), 0.032);
	//SpotLight
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
	*/

	/*
	///DESERT///
	//Directional light
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.ambient"), 0.3f, 0.24f, 0.14f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.diffuse"), 0.7f, 0.42f, 0.26f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	//Point light 1
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].quadratic"), 0.032);
	//Point light 2
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].quadratic"), 0.032);
	//Point light 3
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].quadratic"), 0.032);
	//Point light 4
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].quadratic"), 0.032);
	//SpotLight
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.specular"), 0.8f, 0.8f, 0.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(13.0f)));
	*/

	/*
	///Factory///
	//Directional light
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.ambient"), 0.05f, 0.05f, 0.1f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.7);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.specular"), 0.7f, 0.7f, 0.7f);
	//Point light 1
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].quadratic"), 0.032);
	//Point light 2
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].quadratic"), 0.032);
	//Point light 3
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].quadratic"), 0.032);
	//Point light 4
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].quadratic"), 0.032);
	//SpotLight
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.linear"), 0.009);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.quadratic"), 0.0032);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(12.5f)));
	*/
	
	/*
	///HORROR///
	//Directional light
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.diffuse"), 0.05f, 0.05f, 0.05);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
	//Point light 1
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].linear"), 0.14);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].quadratic"), 0.07);
	//Point light 2
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].linear"), 0.14);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].quadratic"), 0.07);
	//Point light 3
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].linear"), 0.22);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].quadratic"), 0.20);
	//Point light 4
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].linear"), 0.14);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].quadratic"), 0.07);
	//SpotLight
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
	*/
	
	///BIOCHEMICAL LAB///
	//Directional light
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
	//Point light 1
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].linear"), 0.07);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[0].quadratic"), 0.017);
	//Point light 2
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].linear"), 0.07);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[1].quadratic"), 0.017);
	//Point light 3
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].linear"), 0.07);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[2].quadratic"), 0.017);
	//Point light 4
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].linear"), 0.07);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "pointLights[3].quadratic"), 0.017);
	//SpotLight
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.diffuse"), 0.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.specular"), 0.0f, 1.0f, 0.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.linear"), 0.07);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.quadratic"), 0.017);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.cutOff"), glm::cos(glm::radians(7.0f)));
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(10.0f)));

	//Create camera transformations
	view = camera.GetViewMatrix();
	projection = perspective(camera.Zoom, (float) screenWidth / (float) screenHeight, 0.1f, 1000.0f);
	//Get the uniform locations
	modelLoc = glGetUniformLocation(cubeShaderPtr->Program, "model");
	viewLoc = glGetUniformLocation(cubeShaderPtr->Program, "view");
	projLoc = glGetUniformLocation(cubeShaderPtr->Program, "projection");
	//Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	//Set diffuse map
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "diffuseSampler"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	//Set specular map
	glUniform1i(glGetUniformLocation(cubeShaderPtr->Program, "specularSampler"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	//Set material properties
	matShineLoc = glGetUniformLocation(cubeShaderPtr->Program, "material.shininess");
	glUniform1f(matShineLoc, 64.0f);

	//Draw the cube
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexPositionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexNormalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cubeTextureCoordBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	model;
	for (GLint i = 0; i < 10; i++)
	{
		model = mat4();
		model = translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = rotate(model, angle, vec3(1.0f, 3.0f, 5.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//Disable vertex atrributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//Unbind textures (safe practice)
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/////LAMP OBJECT/////
	//Use the shader for the lamp
	lampShaderPtr->Use();

	//Get location objects for the matrices on our lamp shader 
	//(these could be different on a different shader)
	modelLoc = glGetUniformLocation(lampShaderPtr->Program, "model");
	viewLoc = glGetUniformLocation(lampShaderPtr->Program, "view");
	projLoc = glGetUniformLocation(lampShaderPtr->Program, "projection");
	//Set matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	//Draw lamp
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexPositionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	for (GLuint i = 0; i < 4; i++)
	{
		initLampColorBuffer(pointLightColors[i]);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, lampColorBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		model = glm::mat4();
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));		//Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//Disable vertex atrributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

//Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//Method to load texture
void loadTexture(char *diffuseMapPath, char *specularMapPath) {
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	int width, height;
	unsigned char* image;
	// Diffuse map
	image = SOIL_load_image(diffuseMapPath, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	image = SOIL_load_image(specularMapPath, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	// Set texture filtering/wrapping)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}