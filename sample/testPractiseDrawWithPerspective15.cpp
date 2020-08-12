#include "../include/test.h"
#include "../include/shaders.h"
#include "../include/fileUtil.h"
#include "../include/shaderSource.h"

#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

static const char* vertextFile = "shader_perspective_test15_vertex.txt";
static const char* fragFile = "shader_perspective_test15_fragment.txt";
static const char* imageFile1 = "container.jpg";
static const char* imageFile2 = "awesomeface.png";

static GLuint VAO, VBO, TEX1,TEX2;
static int shaderProgram;

static bool prepare();

static void render();

static void destory();

static void useMatrix();

static void getLookAt(glm::mat4 * view);

static int locUniformModel;
static int locUniformView;
static int locUniformProjection;

static float vertices[] = {
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static const char* shaderDirName = "shader";
static char* shaderDir;

static const char* imagesDirName = "images";
static char* imagesDir;

int practiseDrawWithPerspective15(const char * projectDir) {

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

	if (!prepare()) { return -1; }

	glEnable(GL_DEPTH_TEST);  //启用深度测试；它默认是关闭的。
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

bool prepare() {

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	textureGenSet(&TEX1);
	if (!textureLoadImg(imagesDir, imageFile1, GL_RGB)) { return false; }

	textureGenSet(&TEX2);
	if (!textureLoadImg(imagesDir, imageFile2, GL_RGBA)) { return false; }

	//-----------------------------下面设置变换矩阵-----------------------------------

	glUseProgram(shaderProgram);

	int locTexture1 = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(locTexture1, 0);

	int locTexture2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(locTexture2, 1);

	 locUniformModel = glGetUniformLocation(shaderProgram, "model");
	 locUniformView = glGetUniformLocation(shaderProgram, "view");
	 locUniformProjection = glGetUniformLocation(shaderProgram, "projection");

	 //useMatrix();

	return true;
}

void useMatrix() {

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//让我们变换一下我们的平面，将其绕着x轴旋转，使它看起来像放在地上一样。
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model,  glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	//// 注意，我们将矩阵向我们要进行移动场景的反方向移动。

	float radius = 5.0f;
	float camX = cos(glfwGetTime()) * radius;
	//float camY = cos(glfwGetTime()) * radius;
	float camZ = sin(glfwGetTime()) * radius;

	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));

	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//投影矩阵,我们希望在场景中使用透视投影
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600, 0.1f, 100.0f);

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);


	//让我们变换一下我们的平面，将其绕着x轴旋转，使它看起来像放在地上一样。
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 mode2 = glm::mat4(1.0f);
	mode2 = glm::translate(mode2, glm::vec3(1.5f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(mode2));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);	
	
	glm::mat4 mode3 = glm::mat4(1.0f);
	mode3 = glm::translate(mode3, glm::vec3(-1.5f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(mode3));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

/**
*  获得观察矩阵
*/
void getLookAt(glm::mat4* lookAt) {
	//定义一个摄像机位置， vec3 向量 ,  
	//不要忘记正z轴是从屏幕指向你的，如果我们希望摄像机向后移动，我们就沿着z轴的正方向移动。
//	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	//让摄像机指向场景原点：(0, 0, 0)。还记得如果将两个矢量相减，我们就能得到这两个矢量的差吗？
	//用场景原点向量减去摄像机位置向量的结果就是摄像机的指向向量。
	//摄像机指向z轴负方向，我们希望方向向量(Direction Vector)指向摄像机的z轴正方向。
	//交换相减的顺序，我们就会获得一个指向摄像机正z轴方向的向量：
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);  //场景原点
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	//上向量(Up Vector)
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  //y轴向上
	//把上向量和第二步得到的方向向量进行叉乘。两个向量叉乘的结果会同时垂直于两向量，
	//因此我们会得到指向x轴正方向的那个向量（如果我们交换两个向量叉乘的顺序就会得到相反的指向x轴负方向的向量）：
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));   //得到右向量，即右轴，它代表摄像机空间的x轴的正方向。

	//得到摄像机得正y轴向量
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);//我们把右向量和方向向量进行叉乘：

	//使用这些摄像机向量我们就可以创建一个LookAt矩阵了，它在创建摄像机的时候非常有用。


	//GLM已经提供了这些支持。
	//我们要做的只是定义一个摄像机位置，一个目标位置和一个表示世界空间中的上向量的向量（我们计算右向量使用的那个上向量）。
	//接着GLM就会创建一个LookAt矩阵，我们可以把它当作我们的观察矩阵
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	*lookAt = view;
}

void render() {
	glUseProgram(shaderProgram);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	useMatrix();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void destory() {
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
