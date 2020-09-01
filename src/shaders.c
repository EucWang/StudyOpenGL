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

	//两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	glViewport(0, 0, width, height);
	//注册这个回调函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数： 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	//检查用户是否按下了返回键(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW
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
* 在绑定纹理之后， 加载纹理图片
* @param parentDir   图片所在目录
* @param imgName  图片文件名
* @param rgbType   图片的rgb类型 取值 GL_RGB (jpg图片取值) , 
*																 GL_RGBA (png图片取值，有透明通道)
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
	else if (channel == 2) { format = GL_RGB; }
	else if (channel == 4) { format = GL_RGBA; }

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	free(imagePath);

	return TRUE;
}

/**
* 加载图片纹理，
* 绑定并生成图片纹理id
* 
* 可以替代上面的  textureGenSet(), textureGenSets(), textureLoadImg() 三个方法
* 
* @return  -1：表示加载失败， >=0 表示加载成功
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
	else if (channel == 2) { format = GL_RGB; }
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
* 在 调用 textureGenSets() 和 textureLoadImg()之后
* 可以将 纹理对象绑定到 着色器中对用的 sampler2D变量上了
* 
* @param shaderProgream   着色器程序
* @param sampler2dName   片段着色器中需要绑定的sampler2D数据的字符串名称
* @param index                     纹理对象的index， 从0开始计数
*/
void textureBind(GLuint shaderProgram, const char * sampler2dName, int index) {
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, sampler2dName), index);
}

/**
* 在render()循环中使用纹理对象
* 
* @param texture  已经绑定过了的纹理对象
* @param index   绑定的纹理对象的index
*/
void textureUse(GLuint texture, int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
}

