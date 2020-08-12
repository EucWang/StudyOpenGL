#include "../include/test.h"
#include "../include/shaders.h"
#include "../include/fileUtil.h"
#include "../include/shaderSource.h"
#include "../include/util.h"

#include <stdlib.h>
#include <time.h>
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

static void useMatrixDraw();

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

//为每个立方体定义一个位移向量来指定它在世界空间的位置
static glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),

	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),

	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),

	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


static const char* shaderDirName = "shader";
static char* shaderDir;

static const char* imagesDirName = "images";
static char* imagesDir;

int practiseDrawWithPerspective14(const char * projectDir) {

	//初始化获取 opengl的shader目录和 图片目录
	if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }
	if (!getChildPath(&imagesDir, projectDir, imagesDirName)) { return -1; }

	GLFWwindow *window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with Perspective");
	if (window == NULL){ return -1;}

	char* vertexPath;
	char* fragPath;
	if (!getChildPath(&vertexPath, shaderDir, vertextFile)) {return -1;}
	if (!getChildPath(&fragPath, shaderDir, fragFile)) {return -1;	}
	if (!createShaderProgram(vertexPath, fragPath, &shaderProgram)) { return -1;}

	if (!prepare(projectDir)) { return -1; }

	//所有深度信息于一个Z缓冲(Z-buffer)中，也被称为深度缓冲(Depth Buffer)。
	//GLFW会自动为你生成这样一个缓冲
	//通过glEnable函数来开启深度测试。
	glEnable(GL_DEPTH_TEST);       //glDisable()可以关闭这项OpenGL功能

	while (!glfwWindowShouldClose(window))
	{
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
	if (!textureLoadImg(imagesDir, imageFile, GL_RGB)) { return false; }

	textureGenSet(&TEX2);
	if (!textureLoadImg(imagesDir, imageFile2, GL_RGBA)) { return false; }

	glUseProgram(shaderProgram);  //设置uniform时，必须先调用glUseProgram()

	int locTexture = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(locTexture, 0);

	int locTexture2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(locTexture2, 1);

	//-----------------------------下面设置变换矩阵-----------------------------------
	//useMatrix();

	return true;
}

static float a = 1.0f, b = 0.3f, c = 0.5f;
static bool af = false, bf = false, cf = false;
const float as = 0.001;
const float bs = 0.0005;
const float cs = 0.0002;

void setParam(float* p, bool* pf, float ps) {
	if (*p >= 1.0f) {
		*p = *p - ps;
		*pf= false;
	}
	else if (*p <= 0.0f) {
		*p = *p + ps;
		*pf = true;
	}
	else {
		if (*pf) {
			*p = *p + ps;
		}
		else {
			*p = *p - ps;
		}
	}
}

/**
* 应用
* 应用矩阵变换，设置坐标系
*/
void useMatrixDraw() {

	int locUniformModel = glGetUniformLocation(shaderProgram, "model");
	int locUniformView = glGetUniformLocation(shaderProgram, "view");
	int locUniformProjection = glGetUniformLocation(shaderProgram, "projection");

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//让我们变换一下我们的平面，将其绕着x轴旋转，使它看起来像放在地上一样。
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	//// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//投影矩阵,我们希望在场景中使用透视投影
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600, 0.1f, 100.0f);

	srand((unsigned)time(NULL));

	setParam(&a, &af, as);
	setParam(&b, &bf, bs);
	setParam(&c, &cf, cs);
	 
	for (unsigned int i = 0; i <10; i++)	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;

		//model = glm::rotate(model, (float)glfwGetTime() *glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::rotate(model, (float)glfwGetTime() *glm::radians(angle), glm::vec3(a, b, c));
		//model = glm::rotate(model, (float)glfwGetTime() *glm::radians(angle), glm::vec3(a, b, c));

		glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / 5);
	}

}

void render() {
	glUseProgram(shaderProgram);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) /5);

	useMatrixDraw();
}

void destory() {
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
