#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

GLFWwindow* createGLWindow(int width, int height, const char* title);

//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//我们同样也希望能够在GLFW中实现一些输入控制，这可以通过使用GLFW的几个输入函数来完成。
//我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。
void processInput(GLFWwindow* window);

#endif