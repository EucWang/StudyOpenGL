#ifndef RENDER_H_
#define RENDER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <string>
#include <iostream>
#include <vector>
#include <stb_image.h>

#include "fileUtil.h"

using namespace std;

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 640

class RenderUtil {

public:

	/// <summary>
	/// ���� ��պ��� ��3d����
	/// </summary>
	/// <param name="projectDir">ͼƬ���ڵĸ���Ŀ¼</param>
	/// <param name="images">��������6�����ͼƬ����</param>
	/// // loads a cubemap texture from 6 individual texture faces
		// order:
		// +X (right)
		// -X (left)
		// +Y (top)
		// -Y (bottom)
		// +Z (front) 
		// -Z (back)
	/// <returns>������պ��ӵ�3d����id, ����С��0��ʾ����ʧ��</returns>
	int static texureLoadCubmap(const char* projectDir, vector<string> images);

	/// <summary>
	/// ���� ֡����
	/// ������ 2d�������Ⱦ����
	/// </summary>
	/// <param name="framebufferobj"></param>
	/// <param name="frametexture"></param>
	/// <param name="renderbuffer"></param>
	/// <returns>1��ʾ�ɹ�,����ʧ��</returns>
	int static createFramebuffer(GLuint* framebufferobj, GLuint* frametexture, GLuint* renderbuffer);

	/// <summary>
	/// ����2DͼƬ����
	/// </summary>
	/// <param name="parentDir">ͼƬ���ڸ���Ŀ¼</param>
	/// <param name="imgName">ͼƬ��Ŀ¼�Լ�ͼƬ�ļ�����</param>
	/// <returns>  -1����ʾ����ʧ�ܣ� >=0 ��ʾ���سɹ�,����ͼƬ����id</returns>
	int static textureLoad2D(std::string parentDir, std::string imgName);

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="width">���ڿ��</param>
	/// <param name="height">���ڸ߶�</param>
	/// <param name="title">���ڱ���</param>
	/// <param name="func">���ڿ�߸ı亯��, 
	/// ���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)
	/// һ��Ĵ������������ص������е��� : glViewport(0, 0, width, height);
	/// </param>
	/// <returns></returns>
	static GLFWwindow* createWindow(int width, int height, const char* title,
		void (*func)(GLFWwindow* window, int width, int height));

	/// <summary>
	/// ���� VAO VBO 
	/// </summary>
	/// <param name="vao"> in/out </param>
	/// <param name="vbo"> in/out </param>
	/// <param name="dataArr"> float data array </param>
	/// <param name="layoutSize"> vertex file layout size , must be values of 3/5/8</param>
	void static makeVertexArrayAndBuffer(GLuint* vao, GLuint* vbo, const float* dataArr, int dataArrSize, int layoutSize);

	void static makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo,
		const float* positions, const int positionSize, const int arr1Offset,
		const float* normals, const int normalsSize, const int arr2Offset,
		const float* texCoords, const int texCoordsSize, const int arr3Offset);

	void static makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo,
		const float* positions, const int positionSize, const int arr1Offset,
		const float* normals, const int normalsSize, const int arr2Offset);

	void static makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo,
		const float* positions, const int positionsSize, const int arr1Offset);
	 
};

#endif // !RENDER_H_
