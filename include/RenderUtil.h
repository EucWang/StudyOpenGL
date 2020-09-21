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
	/// 加载 天空盒子 的3d纹理
	/// </summary>
	/// <param name="projectDir">图片所在的父级目录</param>
	/// <param name="images">天空纹理的6个面的图片集合</param>
	/// // loads a cubemap texture from 6 individual texture faces
		// order:
		// +X (right)
		// -X (left)
		// +Y (top)
		// -Y (bottom)
		// +Z (front) 
		// -Z (back)
	/// <returns>返回天空盒子的3d纹理id, 返回小于0表示加载失败</returns>
	int static texureLoadCubmap(const char* projectDir, vector<string> images);

	/// <summary>
	/// 创建 帧缓冲
	/// 附件有 2d纹理和渲染缓冲
	/// </summary>
	/// <param name="framebufferobj"></param>
	/// <param name="frametexture"></param>
	/// <param name="renderbuffer"></param>
	/// <returns>1表示成功,否则失败</returns>
	int static createFramebuffer(GLuint* framebufferobj, GLuint* frametexture, GLuint* renderbuffer);

	/// <summary>
	/// 加载2D图片纹理，
	/// </summary>
	/// <param name="parentDir">图片所在父级目录</param>
	/// <param name="imgName">图片子目录以及图片文件名称</param>
	/// <returns>  -1：表示加载失败， >=0 表示加载成功,返回图片纹理id</returns>
	int static textureLoad2D(std::string parentDir, std::string imgName);

	/// <summary>
	/// 创建窗口
	/// </summary>
	/// <param name="width">窗口宽度</param>
	/// <param name="height">窗口高度</param>
	/// <param name="title">窗口标题</param>
	/// <param name="func">窗口宽高改变函数, 
	/// 当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)
	/// 一般的处理就是在这个回调函数中调用 : glViewport(0, 0, width, height);
	/// </param>
	/// <returns></returns>
	static GLFWwindow* createWindow(int width, int height, const char* title,
		void (*func)(GLFWwindow* window, int width, int height));

	/// <summary>
	/// 创建 VAO VBO 
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
