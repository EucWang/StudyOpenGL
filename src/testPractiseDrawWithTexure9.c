#include "test.h"
#include "shaders.h"
#include "shaderSource.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h> 
#include "fileUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

static void prepare(char* projectDir);
static void render();
static void destroy();

unsigned int VAO, VBO, EBO, texture, shaderId;

//static char* vertexPath = "D:/workspace/cppWorkspace/StudyOpenGL/shader_texture_test9_vertex.txt";
//static char* fragPath = "D:/workspace/cppWorkspace/StudyOpenGL/shader_texture_test9_fragment.txt";
//static char* imagePath = "D:/workspace/cppWorkspace/StudyOpenGL/container.jpg";
static char* vertexfile = "shader_texture_test9_vertex.txt";
static char* fragfile = "shader_texture_test9_fragment.txt";
static char* imagefile = "container.jpg";

int testPractiseDrawWithTexure9(char* projectDir) {
	GLFWwindow * window = createGLWindow(800, 640, "Draw Triangle With Texure");
	if (window == NULL) {
		return -1;
	}

	char* vertexPath;
	char* fragPath;
	if (!getChildPath(&vertexPath, projectDir, vertexfile)) {
		printf("%s\n", "testPractiseDrawWithTexure9() call failed, because getChildPath(vertexPath) called failed.");
		return -1;
	}
	if (!getChildPath(&fragPath, projectDir, fragfile)) {
		printf("%s\n", "testPractiseDrawWithTexure9() call failed, because getChildPath(fragPath) called failed.");
		return -1;
	}

	if (!createShaderProgram(vertexPath, fragPath, &shaderId)) {
		return -1;
	}
	
	prepare(projectDir); 
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy();
	glfwTerminate();

	free(vertexPath);
	free(fragPath);

	return 1;
}

void prepare(char* projectDir) {
//	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
			//	-1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f
//	};
//	float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.75f, 0.75f,   // 右上
//			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.75f, 0.25f,   // 右下
//			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.25f,   // 左下
//			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.25f, 0.75f    // 左上
		//	-1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f
//	};
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, -1.0f,   // 右下
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   -1.0f, -1.0f,   // 左下
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   -1.0f, 2.0f    // 左上
			//	-1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f
	};
	unsigned int slices[] = {
		0, 1, 3,
		1, 2, 3,
	//	2, 3, 4
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(slices), slices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenTextures(1, &texture); //生产纹理对象
	glBindTexture(GL_TEXTURE_2D, texture);  //绑定纹理对象

	//纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //x轴的环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //y轴的环绕方式

	//纹理过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //缩小时用 近似过滤，产生像素风格
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //放大时用直线过滤， 产生平滑过滤

	//多级渐进纹理 , 纹理跨越时的过滤方案
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//加载图片
	int width, height, nrChannels;

	char* imagePath;
	if (!getChildPath(&imagePath, projectDir, imagefile)) {
		printf("%s\n", "testPractiseDrawWithTexure9() call failed, because getChildPath(fragPath) called failed.");
		return;
	}
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (data) {
		//用图片数据产生一个纹理
		//第一个参数， 指定纹理目标，
		//		 设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理
		//		（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
		//第二个参数纹理指定多级渐远纹理的级别
		//		如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
		//第三个参数  把纹理储存为何种格式
		//		我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
		//第四，五个参数 设置最终的纹理的宽度和高度
		//第六个参数   总是被设为0（历史遗留的问题）。
		//第七，八个参数  源图的格式和数据类型。
		//第九个参数  图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//当前绑定的纹理对象就会被附加上纹理图像
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		printf("%s\n", "failed to load texture.");
		return;
	}

	stbi_image_free(data);
	free(imagePath);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void render() {
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shaderId);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroy() {
	glDeleteProgram(shaderId);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}