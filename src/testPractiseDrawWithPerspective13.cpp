#include "test.h"
#include "shaders.h"
#include "fileUtil.h"
#include "shaderSource.h"
#include "util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

static const char* vertextFile = "shader_perspective_test13_vertex.txt";
static const char* fragFile = "shader_perspective_test13_fragment.txt";
static const char* imageFile = "container.jpg";
static const char* imageFile2 = "awesomeface.png";

static GLuint VAO, VBO, TEX, TEX2;
static int shaderProgram;

static bool prepare(const char* projectDir);

static void render();

static void destory();

static void useMatrix();

static float vertices[] = {
	// ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,		 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,		 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,		 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,		  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,		 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,		 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,		 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,		 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,		 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,		 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,		  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,		  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,		 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,		 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,		 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,		 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,		 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,		 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,		 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,		 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,		 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,		 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,		 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,		 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,		 0.0f, 1.0f
};

int practiseDrawWithPerspective13(const char * projectDir) {
	GLFWwindow *window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with Perspective");
	if (window == NULL){ return -1;}

	char* vertexPath;
	char* fragPath;
	if (!getChildPath(&vertexPath, projectDir, vertextFile)) {return -1;}
	if (!getChildPath(&fragPath, projectDir, fragFile)) {return -1;	}
	if (!createShaderProgram(vertexPath, fragPath, &shaderProgram)) { return -1;}

	if (!prepare(projectDir)) { return -1; }

	//所有深度信息于一个Z缓冲(Z-buffer)中，也被称为深度缓冲(Depth Buffer)。
	//GLFW会自动为你生成这样一个缓冲
	//通过glEnable函数来开启深度测试。
	glEnable(GL_DEPTH_TEST);       //glDisable()可以关闭这项OpenGL功能

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//每次渲染迭代之前清除深度缓冲（否则前一帧的深度信息仍然保存在缓冲中）。
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destory();
	glfwTerminate();

	free(vertexPath);
	free(fragPath);
	return 1;
}

bool prepare(const char* projectDir) {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	textureGenSet(&TEX);
	if (!textureLoadImg(projectDir, imageFile, GL_RGB)) { return false; }

	textureGenSet(&TEX2);
	if (!textureLoadImg(projectDir, imageFile2, GL_RGBA)) { return false; }

	glUseProgram(shaderProgram);  //设置uniform时，必须先调用glUseProgram()

	int locTexture = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(locTexture, 0);

	int locTexture2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(locTexture2, 1);

	//-----------------------------下面设置变换矩阵-----------------------------------
	useMatrix();

	return true;
}

/**
* 应用
* 应用矩阵变换，设置坐标系
*/
void useMatrix() {

	int locUniformModel = glGetUniformLocation(shaderProgram, "model");
	int locUniformView = glGetUniformLocation(shaderProgram, "view");
	int locUniformProjection = glGetUniformLocation(shaderProgram, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//让我们变换一下我们的平面，将其绕着x轴旋转，使它看起来像放在地上一样。
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	//// 注意，我们将矩阵向我们要进行移动场景的反方向移动。  //这里让视角在一个区间晃荡
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f + getFVal() * (-10.0f)));
	//投影矩阵,我们希望在场景中使用透视投影
	//perspective()  4个参数
	//第一个参数， fov ： 表示的是视野 Field of View, 通常设置为45f，能表示一个真实的观察效果; 
	//									但想要一个末日风格的结果你可以将其设置一个更大的值。
	//第二个参数， 设置了宽高比， 由视口的宽除以高所得。
	//第三四个参数 设置了平截头体的近和远平面。 
	//									通常设置近距离为0.1f,  
	//												远距离为100f，  所有在近平面和远平面内且处于平截头体内的顶点都会被渲染
	// 如果把第三个参数near设置过大，例如10f， 则会将靠近摄像机的坐标（在0.0f和10.f之间）都裁剪掉
	// 这会导致的视觉效果：在太过靠近一个物体的时候你的视线会直接穿过去
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600, 0.1f, 100.0f);

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
}

void render() {
	glUseProgram(shaderProgram);

	useMatrix();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) /5);
}

void destory() {
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
