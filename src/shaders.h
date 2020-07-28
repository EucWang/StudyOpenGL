#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

GLFWwindow* createGLWindow(int width, int height, const char* title);

//着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
//如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。
//已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
//这里会同时创建 顶点着色器 和 片段着色器
int makeShaderProgram(const char* tvertexShaderSource, const char* tfragShaderSource);

//编译创建顶点着色器
int makeVertexShader(const char* tvertexShaderSource);

//编译创建片段着色器
int makeFragmentShader(const char* tfragShaderSource);

//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//我们同样也希望能够在GLFW中实现一些输入控制，这可以通过使用GLFW的几个输入函数来完成。
//我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。
void processInput(GLFWwindow* window);

#endif