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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Setup and compile our shaders
	//need to be after glewInit(), otherwise got error
	Shader cubeShader("CubeVertexShader_MultipleLights.vs", "CubeFragmentShader_MultipleLights.fs");
	cubeShaderPtr = &cubeShader;
	Shader lampShader("LampVertexShader.vs", "LampFragmentShader.fs");
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

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////Change the light's position values over time
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

	/////CUBE OBJECT/////
	//Use the cube shader
	cubeShaderPtr->Use();

	//Set uniform objectColor in Fragment Shader
	//GLint objectColorLoc = glGetUniformLocation(cubeShaderPtr->Program, "objectColor");
	//glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);

	//Setting the uniform light position in Fragment Shader
	GLint lightPosLoc = glGetUniformLocation(cubeShaderPtr->Program, "light.position");
	//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

	//The below is needed for spotlight
	glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	//Setting uniform view position in Fragment Shader
	GLint viewPosLoc = glGetUniformLocation(cubeShaderPtr->Program, "viewPos");
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	//Create camera transformations
	mat4 view;
	view = camera.GetViewMatrix();
	mat4 projection;
	projection = perspective(camera.Zoom, 700.0f / 500.0f, 0.1f, 1000.0f);
	//Get the uniform locations
	GLint modelLoc = glGetUniformLocation(cubeShaderPtr->Program, "model");
	GLint viewLoc = glGetUniformLocation(cubeShaderPtr->Program, "view");
	GLint projLoc = glGetUniformLocation(cubeShaderPtr->Program, "projection");

	// Set diffuse map
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "diffuseSampler"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	//Set specular map
	glUniform1i(glGetUniformLocation(cubeShaderPtr->Program, "specularSampler"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	GLint matShineLoc = glGetUniformLocation(cubeShaderPtr->Program, "material.shininess");
	//GLint lightDirPos = glGetUniformLocation(cubeShaderPtr->Program, "light.direction");
	GLint lightAmbientLoc = glGetUniformLocation(cubeShaderPtr->Program, "light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(cubeShaderPtr->Program, "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(cubeShaderPtr->Program, "light.specular");

	//Set material properties
	glUniform1f(matShineLoc, 64.0f);

	//Set lighting properties
	//glUniform3f(lightDirPos, -0.2f, -1.0f, -0.3f);	//Needed for directional light
	glUniform3f(lightAmbientLoc, 0.1f, 0.1f, 0.1f);
	glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f);
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
	//The below 3 are needed for point light
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "light.linear"), 0.09);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "light.quadratic"), 0.032);

	glUniform3f(glGetUniformLocation(cubeShaderPtr->Program, "light.spotDir"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "light.spotCutOff"), cos(radians(12.5f)));
	glUniform1f(glGetUniformLocation(cubeShaderPtr->Program, "light.spotOuterCutOff"), cos(radians(17.5f)));

	//Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

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

	// Positions of all our containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	mat4 model;
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

	///////LAMP OBJECT/////
	////Use the shader for the lamp
	//lampShaderPtr->Use();

	////Get location objects for the matrices on our lamp shader 
	////(these could be different on a different shader)
	//modelLoc = glGetUniformLocation(lampShaderPtr->Program, "model");
	//viewLoc = glGetUniformLocation(lampShaderPtr->Program, "view");
	//projLoc = glGetUniformLocation(lampShaderPtr->Program, "projection");
	////Set matrices
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	////Draw lamp
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, cubeVertexPositionBuffer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//model = mat4();
	//model = translate(model, lightPos);
	//model = scale(model, vec3(0.2f)); // Make it a smaller cube
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//Disable vertex atrributes
	//glDisableVertexAttribArray(0);
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