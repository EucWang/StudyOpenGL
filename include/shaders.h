#ifndef SHADER_H_
#define SHADER_H_

#ifdef __cplusplus    //__cplusplus��cpp���Զ����һ����
extern "C" {          //���߱��������ⲿ�ִ��밴C���Եĸ�ʽ���б��룬������C++��
#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

//#define true 1
#define TRUE 1
//#define false 0
#define FALSE 0

#define SMALL_SCREEN_WIDTH 800
#define SMALL_SCREEN_HEIGHT 640

GLFWwindow* createGLWindow(int width, int height, const char* title);

//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//����ͬ��Ҳϣ���ܹ���GLFW��ʵ��һЩ������ƣ������ͨ��ʹ��GLFW�ļ������뺯������ɡ�
//���ǽ���ʹ��GLFW��glfwGetKey����������Ҫһ�������Լ�һ��������Ϊ���롣
void processInput(GLFWwindow* window);

/**
* ���� ������� ������ �����������Ĭ�ϻ���Ϊ GL_REPEAT /���� ΪGL_LINEAR 
* @param texture ��������intֵ�� �ᴴ��������� ���ﴫ��ַ
*/
void textureGenSet(/*in,out*/GLuint* texture);

/**
* ���� ������� ������ �����������Ĭ�ϻ��� /����
* @param texture ��������intֵ�� �ᴴ��������� ���ﴫ��ַ
* @param wrapS  ȡֵΪ GL_REPEAT ,  �������Ĭ����Ϊ���ظ�����ͼ��
										GL_MIRRORED_REPEAT, ��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
										GL_CLAMP_TO_EDGE, ��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
										GL_CLAMP_TO_BORDER, ����������Ϊ�û�ָ���ı�Ե��ɫ��
* @param wrapT ȡֵΪͬ wrapS
* @param minFilter  ȡֵΪ  GL_NEAREST  ��Ҳ���ڽ����ˣ�Nearest Neighbor Filtering����OpenGLĬ�ϵ�������˷�ʽ��
*																		����ΪGL_NEAREST��ʱ��OpenGL��ѡ�����ĵ���ӽ�����������Ǹ����ء�
*																		GL_NEAREST�����˿���״��ͼ���������ܹ��������������������أ�
*											  GL_LINEAR      ��Ҳ�����Թ��ˣ�(Bi)linear Filtering����������������긽�����������أ������һ����ֵ�����Ƴ���Щ��������֮�����ɫ��
*																		 GL_LINEAR�ܹ�������ƽ����ͼ�������ѿ����������������ء�
*/
void textureGenSets(/*in,out*/GLuint* texture, /*in*/int wrapS, /*in*/int wrapT, /*in*/int minFilter, /*in*/int magFilter);

/**
* �ڰ�����֮�� ��������ͼƬ
* @param parentDir   ͼƬ����Ŀ¼
* @param imgName  ͼƬ�ļ���
* @param rgbType   ͼƬ��rgb���� ȡֵ GL_RGB (jpgͼƬȡֵ) ,
*																 GL_RGBA (pngͼƬȡֵ����͸��ͨ��)
*/
bool textureLoadImg(/*in*/const char* parentDir, /*in*/const char* imgName, /*in*/int rgbType);

void textureBind(GLuint shaderProgram, const char* sampler2dName, int index);

void textureUse(GLuint texture, int index);

#ifdef __cplusplus
}
#endif

#endif