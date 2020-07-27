#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

//���봴��������ɫ��
int makeVertexShader(char* vertexShaderSource);

//���봴��Ƭ����ɫ��
int makeFragmentShader(char* fragShaderSource);


//��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
//���Ҫʹ�øղű������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������Ȼ������Ⱦ�����ʱ�򼤻������ɫ������
//�Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ�á�
int makeShaderProgram(char* vertexShaderSource, char* fragShaderSource);

//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//����ͬ��Ҳϣ���ܹ���GLFW��ʵ��һЩ������ƣ������ͨ��ʹ��GLFW�ļ������뺯������ɡ�
//���ǽ���ʹ��GLFW��glfwGetKey����������Ҫһ�������Լ�һ��������Ϊ���롣
void processInput(GLFWwindow* window);

#endif