#include "test.h"
#include "fileUtil.h"
#include "shaders.h"
#include "shaderSource.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
using namespace std;


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const char* vertextFile = "shader_transform_test11_vertex.txt";
static const char* fragFile = "shader_transform_test11_fragment.txt";
static const char* imageFile1 = "container.jpg";
static const char* imageFile2 = "awesomeface.png";

static GLuint VAO, VBO, EBO, TEX1, TEX2;
static int shaderProgram;

static int transLoc;

static int prepare(const char* projectDir);

static void render();

static void destroy();

/**
* 根据系统时间，获取的数值在0.0 - 1.0之间变化
*/
float getFVal() {
	float retVal = (float)(sin((float)glfwGetTime()) / 2.0f + 0.5f);
	std::cout << "retVal  = " << retVal << std::endl;
	return retVal;
}


int practiseDrawWithTransform11(const char *projectDir) {
	std::cout << "practiseDrawWithTransform11() run...." << std::endl;

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with Transform");
	if (window == NULL) {
		return -1;
	}

	char* vertexPath;
	char* fragPath;
	if (!getChildPath(&vertexPath, projectDir, vertextFile)) {
		return -1;
	}
	if (!getChildPath(&fragPath, projectDir, fragFile)) {
		return -1;
	}

	if (!createShaderProgram(vertexPath, fragPath, &shaderProgram)) {
		return -1;
	}

	if (prepare(projectDir) < 0) {
		std::cout << "prepare() call failed." << std::endl;
		return -1;
	}

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

int prepare(const char* projectDir) {
	std::cout << "prepare(" << projectDir << ") run()..." << std::endl;
	float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
				0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
				0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//第一个纹理
	textureGenSets(&TEX1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR); 
	textureLoadImg(projectDir, imageFile1, GL_RGB);
	 
	//第二个纹理
	textureGenSets(&TEX2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR); 
	textureLoadImg(projectDir, imageFile2, GL_RGBA);
	 
	glUseProgram(shaderProgram);

	int uniTex1 = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1f(uniTex1, 0);

	int uniTex2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(uniTex2, 1);

	//实际的变换顺序应该与阅读顺序相反：
	//尽管在代码中我们先位移再旋转，实际的变换却是先应用旋转再是位移的。
	//对箱子进行矩阵变换， 旋转90度，缩小0.5倍
	transLoc = glGetUniformLocation(shaderProgram, "transform");
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
	 
	return 1;
}

void render() {

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	

	//通过改变转换函数，然后再次不同位置画一个箱子
	//这个箱子不停的放大缩小
	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
	float fv = getFVal();
	trans2 = glm::scale(trans2, glm::vec3(fv, fv, fv));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans2));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void destroy() {

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}