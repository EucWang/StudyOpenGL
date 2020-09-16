#include "../include/shaders.h"
#include <stdio.h>
#include <stdbool.h>

#include "../include/fileUtil.h"


#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

GLFWwindow* createGLWindow(int width, int height, const char* title) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	GLFWwindow* window = glfwCreateWindow(width, height,  title, NULL, NULL);
	if (window == NULL) {
		printf("%s\n", "Failed to create GLFW window");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("%s\n", "Failed to initialize GLAD.");
		return NULL;
	}

	//�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, width, height);
	//ע������ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ�������������� 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	//����û��Ƿ����˷��ؼ�(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊ true�ķ����ر�GLFW
		glfwSetWindowShouldClose(window, TRUE);
	}
}

void textureGenSet(GLuint* texture) {
	textureGenSets(texture, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

void textureGenSets(GLuint* texture, int wrapS, int wrapT, int minFilter, int magFilter) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

/**
* �ڰ�����֮�� ��������ͼƬ
* @param parentDir   ͼƬ����Ŀ¼
* @param imgName  ͼƬ�ļ���
* @param rgbType   ͼƬ��rgb���� ȡֵ GL_RGB (jpgͼƬȡֵ) , 
*																 GL_RGBA (pngͼƬȡֵ����͸��ͨ��)
*/
bool textureLoadImg(const char * parentDir, const char * imgName, int rgbType) {
	int width, height, channel;
	char* imagePath;
	if (!getChildPath(&imagePath, parentDir, imgName)) {
		//std::cout << "get imagePath2 failed." << std::endl;
		printf("%s\n", "textureLoadImg() load failed, because getChildPath() failed.");
		return FALSE;
	}

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &channel, 0);
	if (data == NULL) {
		//std::cout << "load image2 failed" << std::endl;
		printf("%s\n", "textureLoadImg() load failed,  because stbi_load() failed.");
		return FALSE;
	}

	GLenum format;
	if (channel == 1) { format = GL_RED; }
	else if (channel == 3) { format = GL_RGB; }
	else if (channel == 4) { format = GL_RGBA; }

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	free(imagePath);

	return TRUE;
}

/**
* ����ͼƬ����
* �󶨲�����ͼƬ����id
* 
* ������������  textureGenSet(), textureGenSets(), textureLoadImg() ��������
* 
* @return  -1����ʾ����ʧ�ܣ� >=0 ��ʾ���سɹ�
*/
int textureLoad(const char * parentDir, const char * imgName) {
	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, channel;
	char* imagePath;
	if (!getChildPath(&imagePath, parentDir, imgName)) {
		//std::cout << "get imagePath2 failed." << std::endl;
		printf("%s\n", "textureLoadImg() load failed, because getChildPath() failed.");
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &channel, 0);
	if (data == NULL) {
		//std::cout << "load image2 failed" << std::endl;
		printf("%s\n", "textureLoadImg() load failed,  because stbi_load() failed.");
		stbi_image_free(data);
		return -1;
	}

	GLenum format;
	if (channel == 1) { format = GL_RED; }
	else if (channel == 3) { format = GL_RGB; }
	else if (channel == 4) { format = GL_RGBA; }

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	free(imagePath);

	return textureId;
}


/**
* ����ͼƬ����
* �󶨲�����ͼƬ����id
*
* ������������  textureGenSet(), textureGenSets(), textureLoadImg() ��������
*
* @param wrapS  ȡֵΪ GL_REPEAT ,  �������Ĭ����Ϊ���ظ�����ͼ��
*										GL_REPEAT
										GL_MIRRORED_REPEAT, ��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
										GL_CLAMP_TO_EDGE, ��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
										GL_CLAMP_TO_BORDER, ����������Ϊ�û�ָ���ı�Ե��ɫ��
* @param wrapT ȡֵΪͬ wrapS
* @param minFilter  ȡֵΪ  GL_NEAREST  ��Ҳ���ڽ����ˣ�Nearest Neighbor Filtering����OpenGLĬ�ϵ�������˷�ʽ��
*						 ����ΪGL_NEAREST��ʱ��OpenGL��ѡ�����ĵ���ӽ�����������Ǹ����ء�
*						 GL_NEAREST�����˿���״��ͼ���������ܹ��������������������أ�
*											  GL_LINEAR      ��Ҳ�����Թ��ˣ�(Bi)linear Filtering����������������긽�����������أ������һ����ֵ�����Ƴ���Щ��������֮�����ɫ��
*																		 GL_LINEAR�ܹ�������ƽ����ͼ�������ѿ����������������ء�

* @return  -1����ʾ����ʧ�ܣ� >=0 ��ʾ���سɹ�
*/
int textureLoad2(const char* parentDir, const char* imgName, 
	int wrapS, int wrapT, int minFilter, int magFilter) {
	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, channel;
	char* imagePath;
	if (!getChildPath(&imagePath, parentDir, imgName)) {
		//std::cout << "get imagePath2 failed." << std::endl;
		printf("%s\n", "textureLoadImg() load failed, because getChildPath() failed.");
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &channel, 0);
	if (data == NULL) {
		//std::cout << "load image2 failed" << std::endl;
		printf("%s\n", "textureLoadImg() load failed,  because stbi_load() failed.");
		stbi_image_free(data);
		return -1;
	}

	GLenum format;
	if (channel == 1) { format = GL_RED; }
	else if (channel == 3) { format = GL_RGB; }
	else if (channel == 4) { format = GL_RGBA; }

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	stbi_image_free(data);
	free(imagePath);

	return textureId;
}


/**
* �� ���� textureGenSets() �� textureLoadImg()֮��
* ���Խ� �������󶨵� ��ɫ���ж��õ� sampler2D��������
* 
* @param shaderProgream   ��ɫ������
* @param sampler2dName   Ƭ����ɫ������Ҫ�󶨵�sampler2D���ݵ��ַ�������
* @param index                     ��������index�� ��0��ʼ����
*/
void textureBind(GLuint shaderProgram, const char * sampler2dName, int index) {
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, sampler2dName), index);
}

/**
* ��render()ѭ����ʹ���������
* 
* @param texture  �Ѿ��󶨹��˵��������
* @param index   �󶨵���������index
*/
void textureUse(GLuint texture, int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
}


/// <summary>
/// ���� VAO VBO 
/// </summary>
/// <param name="vao"> in/out </param>
/// <param name="dataArr"> float data array </param>
/// <param name="dataArrSize"> float data array size </param>
/// <param name="layoutSize"> vertex file layout size , must be values of 3/5/8</param>
void makeVAOVBO(GLuint* vao, const float* dataArr, int dataArrSize, int layoutSize) {
	GLuint vbo;
	printf("dataArrSize = %d, layoutSize = %d\n", dataArrSize, layoutSize);
	glGenVertexArrays(1, vao);
	glGenBuffers(1, &vbo);
	printf("generate vao = %d, vbo = %d\n", *vao, vbo);

	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, dataArrSize, dataArr, GL_STATIC_DRAW);

	int len = layoutSize / 3;
	printf("len = %d", len);
	for (size_t i = 0; i <= len; i++) {
		glEnableVertexAttribArray(i);

		int size = 3;
		if (i == len) { size = layoutSize % 3; }
		printf("i  = %d, size = %d\n", i, size);
		glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)( i * 3 * sizeof(float)));
	}
}
 