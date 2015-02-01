#ifndef MAIN_LIGHTMAPS_H
#define MAIN_LIGHTMAPS_H

#include <GLFW/glfw3.h>

//Function prototypes
void initCubeVertexPositionBuffer();
void initCubeVertexNormalBuffer();
void initCubeTextureCoordsBuffer();
void renderScene();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void loadTexture(char *diffuseMapPath, char *specularMapPath);

#endif