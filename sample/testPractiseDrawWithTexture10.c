#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../include/test.h" 
#include "../include/fileUtil.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

static char * vertextFile = "shader_texture_test10_vertex.txt";
static char* fragFile = "shader_texture_test10_fragment.txt";
static char* imageFile1 = "container.jpg";
static char* imageFile2 = "awesomeface.png";

static int VAO, VBO, EBO, shaderProgram, TEX1, TEX2;

static float uniRateVal = 0.2f;

static void processInput(GLFWwindow* window);

static int prepare(char * projectDir);

static void render();

static void destroy();


static const char* shaderDirName = "shader";
static char* shaderDir;

static const char* imagesDirName = "images";
static char* imagesDir;

int practiseDrawWithTexture10(char * projectDir) {

	//初始化获取 opengl的shader目录和 图片目录
	if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }
	if (!getChildPath(&imagesDir, projectDir, imagesDirName)) { return -1; }

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw WIth Texture 10");
	if (window == NULL) {
		return -1;
	}
	char* vertexPath;
	char* fragPath;
	if (!getChildPath(&vertexPath, shaderDir, vertextFile) ||
		!getChildPath(&fragPath, shaderDir, fragFile)) {
		return -1;
	}

	if (!createShaderProgram(vertexPath, fragPath, &shaderProgram)) {
		return -1;
	}

	if (prepare(projectDir) < 0) {
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	destroy();
	glfwTerminate();

	free(vertexPath);
	free(fragPath);
	return 1;
}

int prepare(char* projectDir) {
//	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
//	};

//	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
//	}; 
//	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标  第二个纹理坐标-
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 1.0f,   1.0f, 1.0f,    // 右上
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   1.0f, 0.0f,   // 右下
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    -1.0f, 0.0f,  // 左下
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    -1.0f, 1.0f    // 左上
//	}; 
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标  第二个纹理坐标-
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.0f, 1.0f,    // 右上
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.0f, 0.0f,   // 右下
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    0.0f, 0.0f,  // 左下
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    0.0f, 1.0f    // 左上
	};
	unsigned int slices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(slices), slices, GL_STATIC_DRAW);

//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);

//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);

//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	//加载2个图片
	char* imagePath1;
	char* imagePath2;
	if (!getChildPath(&imagePath1, imagesDir, imageFile1) ||
		!getChildPath(&imagePath2, imagesDir, imageFile2)) {
		printf("%s\n", "prepare() call failed, because getChildPath() failed, cannot get imagePath1 or imagePath2");
		return -1;
	}

	int width1, height1, channel1, width2, height2, channel2;

	char * data1 = stbi_load(imagePath1, &width1, &height1, &channel1, 0);

	//因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。如下方法翻转y轴
	stbi_set_flip_vertically_on_load(true);
	char* data2 = stbi_load(imagePath2, &width2, &height2, &channel2, 0);

	if (!data1) {
		printf("%s\n", "stbi_load() failed, data1 is null");
		return -1;
	}
	if (!data2) {
		printf("%s\n", "stbi_load() failed, data2 is null");
		return -1;
	}

	//构建2个纹理对象

	//第一个纹理对象
	glGenTextures(1, &TEX1); 
	glBindTexture(GL_TEXTURE_2D, TEX1);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);	//纹理加载图片
	glGenerateMipmap(GL_TEXTURE_2D);

	//第二个纹理对象
	glGenTextures(1, &TEX2);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);		//环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);//纹理加载图片
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data1);  //释放图片资源
	stbi_image_free(data2); 
	free(imagePath1);  //释放malloc创建的字符串
	free(imagePath2);

	//应用采样器，插值器
	glUseProgram(shaderProgram);

	//设置2个纹理
	//使用glUniform1i设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。
	int uni_texture1 = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(uni_texture1, 0);
	 
	int uni_texture2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(uni_texture2, 1);

	return 1;
}

void render() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glUseProgram(shaderProgram);

	//float timeVal = (float)glfwGetTime();
	//float rateVal = (float)(sin(timeVal) / 2.0f + 0.5f);

	glUniform1f(glGetUniformLocation(shaderProgram, "rate"), uniRateVal);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

static float timeVal = 0.0f;
void processInput(GLFWwindow* window) {

	//检查用户是否按下了返回键(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		float newTimeVal = (float)glfwGetTime();
		printf("%s%f\n", "newTimeVal is ", newTimeVal);
		if (newTimeVal - timeVal > 0.1f) {
			timeVal = newTimeVal;
		}
		else {
			return;
		}
		if (uniRateVal + 0.1f <= 1.0f) {
			uniRateVal += 0.1f;
		}
		printf("%s%f\n", "uniRateVal is ", uniRateVal);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		float newTimeVal = (float)glfwGetTime();
		printf("%s%f\n", "newTimeVal is ", newTimeVal);
		if (newTimeVal - timeVal > 0.1f) {
			timeVal = newTimeVal;
		}
		else {
			return;
		}
		if (uniRateVal  - 0.1f>= 0.0f) {
			uniRateVal -= 0.1f;
		}
		printf("%s%f\n", "uniRateVal is ", uniRateVal);
	}
}
