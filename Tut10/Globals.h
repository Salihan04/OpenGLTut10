#ifndef GLOBALS_H
#define GLOBALS_H

#include "Shader.h"
#include "Camera.h"

GLuint cubeVertexBuffer;

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

#endif