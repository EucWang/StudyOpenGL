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
	/// 将带多重采样的帧缓冲 和 简单的帧缓冲 绑定
	/// 在渲染循环中, 当多重采样帧缓冲 已经完成绘制之后,
	///  将多重采样帧缓冲的 渲染结果 输出到 framebuffer这个普通帧缓冲中,
	/// 以便传递给 屏幕的默认缓冲进行输出显示
	/// </summary>
	/// <param name="multisampleFramebuffer">带多重采样的帧缓冲 </param>
	/// <param name="framebuffer">简单的帧缓冲,用于中间过渡的缓冲</param>
	/// <param name="width">帧缓冲的宽度</param>
	/// <param name="height">帧缓冲的高度</param>
	/// <returns></returns>
	static void blitFrameBuffer(unsigned int* multisampleFramebuffer,
		unsigned int* intermediateFrameBuffer, int width, int height);

	/// <summary>
	/// 创建一个普通的一般的 帧缓冲
	/// </summary>
	/// <param name="framebuffer">帧缓冲</param>
	/// <param name="texColorBuffer">纹理, 帧缓冲附件</param>
	/// <param name="renderBuffer">渲染缓冲, 帧缓冲附件</param>
	/// <param name="bufferWidth">宽度, 纹理和渲染缓冲的高度</param>
	/// <param name="bufferHeight">宽度, 纹理和渲染缓冲的宽度</param>
	/// <param name="sample">采样值, 0  : 不产生多重采样;
	///								>0 : 创建带多重采样的帧缓冲,一般取值4</param>
	/// <returns>是否创建的帧缓冲完整 取值1 : 完整;否则不完整</returns>
	bool static makeFramebuffer(unsigned int* framebuffer,
		unsigned int* texColorBuffer, unsigned int* renderBuffer,
		int bufferWidth, int bufferHeight, int sample);
	 
	/// <summary>
	/// 加载2D图片纹理，
	/// </summary>
	/// <param name="parentDir">图片所在父级目录</param>
	/// <param name="imgName">图片子目录以及图片文件名称</param>
	/// <param name="isSRGB">diffuse纹理，这种为物体上色的纹理几乎都是在sRGB空间中的。
	/// 而为了获取光照参数的纹理，像specular贴图和法线贴图几乎都在线性空间中，
	/// 所以如果你把它们也配置为sRGB纹理的话，光照就坏掉了。指定sRGB纹理时要当心。</param>
	/// <param name="wrapS">设置glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);默认取值GL_REPEAT</param>
	/// <param name="wrapT">设置glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);默认取值GL_REPEAT</param>
	/// <param name="minFilter">设置glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); 默认取值GL_LINEAR_MIPMAP_LINEAR</param>
	/// <param name="magFilter">设置glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAN_FILTER, magFilter); 默认取值GL_LINEAR</param>
	/// <returns> -1：表示加载失败， >=0 表示加载成功,返回图片纹理id</returns>
	int static textureLoad2D(std::string parentDir, std::string imgName, bool isSRGB,
		int wrapS, int wrapT, int minFilter, int magFilter);

	/// <summary>
	/// 加载2D图片纹理，
	/// </summary>
	/// <param name="parentDir">图片所在父级目录</param>
	/// <param name="imgName">图片子目录以及图片文件名称</param>
	/// <param name="isSRGB">diffuse纹理，这种为物体上色的纹理几乎都是在sRGB空间中的。
	/// 而为了获取光照参数的纹理，像specular贴图和法线贴图几乎都在线性空间中，
	/// 所以如果你把它们也配置为sRGB纹理的话，光照就坏掉了。指定sRGB纹理时要当心。</param>
	/// <returns>  -1：表示加载失败， >=0 表示加载成功,返回图片纹理id</returns>
	int static textureLoad2D(std::string parentDir, std::string imgName,bool isSRGB);

	/// <summary>
	/// 创建窗口
	/// deprecated 
	/// 已经废弃, 使用 WindowHelper类替代
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

	void static makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, bool isnormal,
		const float* positions, const int positionSize, const int arr1Offset,
		const float* normals, const int normalsSize, const int arr2Offset,
		const float* texCoords, const int texCoordsSize, const int arr3Offset);

	void static makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, bool isnormal,
		const float* positions, const int positionSize, const int arr1Offset,
		const float* normals, const int normalsSize, const int arr2Offset);

	void static makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, bool isnormal,
		const float* positions, const int positionsSize, const int arr1Offset);
	 
};

#endif // !RENDER_H_
