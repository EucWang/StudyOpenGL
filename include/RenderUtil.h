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
	/// �������ز�����֡���� �� �򵥵�֡���� ��
	/// ����Ⱦѭ����, �����ز���֡���� �Ѿ���ɻ���֮��,
	///  �����ز���֡����� ��Ⱦ��� ����� framebuffer�����ͨ֡������,
	/// �Ա㴫�ݸ� ��Ļ��Ĭ�ϻ�����������ʾ
	/// </summary>
	/// <param name="multisampleFramebuffer">�����ز�����֡���� </param>
	/// <param name="framebuffer">�򵥵�֡����,�����м���ɵĻ���</param>
	/// <param name="width">֡����Ŀ��</param>
	/// <param name="height">֡����ĸ߶�</param>
	/// <returns></returns>
	static void blitFrameBuffer(unsigned int* multisampleFramebuffer,
		unsigned int* intermediateFrameBuffer, int width, int height);

	/// <summary>
	/// ����һ����ͨ��һ��� ֡����
	/// </summary>
	/// <param name="framebuffer">֡����</param>
	/// <param name="texColorBuffer">����, ֡���帽��</param>
	/// <param name="renderBuffer">��Ⱦ����, ֡���帽��</param>
	/// <param name="bufferWidth">���, �������Ⱦ����ĸ߶�</param>
	/// <param name="bufferHeight">���, �������Ⱦ����Ŀ��</param>
	/// <param name="sample">����ֵ, 0  : ���������ز���;
	///								>0 : ���������ز�����֡����,һ��ȡֵ4</param>
	/// <returns>�Ƿ񴴽���֡�������� ȡֵ1 : ����;��������</returns>
	bool static makeFramebuffer(unsigned int* framebuffer,
		unsigned int* texColorBuffer, unsigned int* renderBuffer,
		int bufferWidth, int bufferHeight, int sample);
	 
	/// <summary>
	/// ����2DͼƬ����
	/// </summary>
	/// <param name="parentDir">ͼƬ���ڸ���Ŀ¼</param>
	/// <param name="imgName">ͼƬ��Ŀ¼�Լ�ͼƬ�ļ�����</param>
	/// <param name="isSRGB">diffuse��������Ϊ������ɫ��������������sRGB�ռ��еġ�
	/// ��Ϊ�˻�ȡ���ղ�����������specular��ͼ�ͷ�����ͼ�����������Կռ��У�
	/// ��������������Ҳ����ΪsRGB����Ļ������վͻ����ˡ�ָ��sRGB����ʱҪ���ġ�</param>
	/// <param name="wrapS">����glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);Ĭ��ȡֵGL_REPEAT</param>
	/// <param name="wrapT">����glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);Ĭ��ȡֵGL_REPEAT</param>
	/// <param name="minFilter">����glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); Ĭ��ȡֵGL_LINEAR_MIPMAP_LINEAR</param>
	/// <param name="magFilter">����glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAN_FILTER, magFilter); Ĭ��ȡֵGL_LINEAR</param>
	/// <returns> -1����ʾ����ʧ�ܣ� >=0 ��ʾ���سɹ�,����ͼƬ����id</returns>
	int static textureLoad2D(std::string parentDir, std::string imgName, bool isSRGB,
		int wrapS, int wrapT, int minFilter, int magFilter);

	/// <summary>
	/// ����2DͼƬ����
	/// </summary>
	/// <param name="parentDir">ͼƬ���ڸ���Ŀ¼</param>
	/// <param name="imgName">ͼƬ��Ŀ¼�Լ�ͼƬ�ļ�����</param>
	/// <param name="isSRGB">diffuse��������Ϊ������ɫ��������������sRGB�ռ��еġ�
	/// ��Ϊ�˻�ȡ���ղ�����������specular��ͼ�ͷ�����ͼ�����������Կռ��У�
	/// ��������������Ҳ����ΪsRGB����Ļ������վͻ����ˡ�ָ��sRGB����ʱҪ���ġ�</param>
	/// <returns>  -1����ʾ����ʧ�ܣ� >=0 ��ʾ���سɹ�,����ͼƬ����id</returns>
	int static textureLoad2D(std::string parentDir, std::string imgName,bool isSRGB);

	/// <summary>
	/// ��������
	/// deprecated 
	/// �Ѿ�����, ʹ�� WindowHelper�����
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
