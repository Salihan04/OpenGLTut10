#ifndef MAIN_COLORS_H
#define MAIN_COLORS_H

#include <GLFW/glfw3.h>

//Function prototypes
void createCubeVertexBuffer();
void renderScene();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

#endif