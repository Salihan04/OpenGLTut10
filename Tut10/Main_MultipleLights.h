#ifndef MAIN_MULTIPLELIGHTS_H
#define MAIN_MULTIPLELIGHTS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Function prototypes
void initCubeVertexPositionBuffer();
void initCubeVertexNormalBuffer();
void initCubeTextureCoordsBuffer();
void initLampColorBuffer(glm::vec3 pointLightColor);
void renderScene();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void loadTexture(char *diffuseMapPath, char *specularMapPath);

#endif