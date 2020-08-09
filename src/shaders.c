#include "shaders.h"
#include <stdio.h>
#include <stdbool.h>

#include "fileUtil.h"


#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	unsigned char* data = stbi_load(imagePath, &width, &height, &channel, 0);
	if (data == NULL) {
		//std::cout << "load image2 failed" << std::endl;
		printf("%s\n", "textureLoadImg() load failed,  because stbi_load() failed.");
		return FALSE;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgbType, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	free(imagePath);

	return TRUE;
}