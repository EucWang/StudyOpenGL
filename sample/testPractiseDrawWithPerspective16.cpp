#include "../include/test.h"
#include "../include/shaders.h"
#include "../include/fileUtil.h"
#include "../include/shaderSource.h"
#include "../include/util.h"

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

static void processInput(GLFWwindow* window);

static bool prepare();

static void render();

static void destory();

static void useMatrix();

/**
* 鼠标的移动监听回调事件
*/
static void mouse_callback(GLFWwindow * window, double xpos, double ypos);

static void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset);

static int locUniformModel;
static int locUniformView;
static int locUniformProjection;

static float cameraSpeed = 0.0005;
static float deltaTime;

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

int practiseDrawWithPerspective16(const char * projectDir) {
	std::cout << "practiseDrawWithPerspective16() running...." << std::endl;

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
	//隐藏光标，并捕捉(Capture)光标，光标应该停留在窗口中（除非程序失去焦点或者退出）。
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//监听鼠标移动事件，注册回调函数
	glfwSetCursorPosCallback(window, mouse_callback);

	//监听鼠标滚轮事件
	glfwSetScrollCallback(window, mouse_scroll_back);

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

static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static glm::mat4 model;
static glm::mat4 view;
static glm::mat4 projection;


//视角移动， 欧拉角，  俯仰角和偏航角由鼠标输入
static float pitch;    //俯仰角
static float yaw;     //偏航角
static float roll;     // 滚转角， 摄像机不讨论这个参数

//视野(Field of View)或fov定义了我们可以看到场景中多大的范围
//平截头的透视矩阵的第一个参数
static float fieldOfView = 45.0f;  

void useMatrix() {

	 model = glm::mat4(1.0f);
	 view = glm::mat4(1.0f);
	 projection = glm::mat4(1.0f);

	//让我们变换一下我们的平面，将其绕着x轴旋转，使它看起来像放在地上一样。
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, getFVal() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


	//投影矩阵,我们希望在场景中使用透视投影
	//在之前的教程中我们说视野(Field of View)或fov定义了我们可以看到场景中多大的范围。
	//当视野变小时，场景投影出来的空间就会减小，产生放大(Zoom In)了的感觉。
	//我们会使用鼠标的滚轮来放大。
	projection = glm::perspective(glm::radians(fieldOfView), 800.0f / 600, 0.1f, 100.0f);

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	 
}

static float lastFrame = 0.0f;

/**
* 控制按键 W, S, A, D 方向 来 移动视角矩阵
*/
void processInput(GLFWwindow* window) {
	float currentTime = glfwGetTime();
	if (lastFrame > 0)	{
		deltaTime = currentTime - lastFrame;
	} else {
		deltaTime = 0.00001;
	}
	lastFrame = currentTime;

	cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

static float lastX = SMALL_SCREEN_WIDTH / 2.0f;
static float lastY = SMALL_SCREEN_HEIGHT / 2.0f;

static float sensitivity = 0.05f;

static glm::vec3 front;

/**
* 窗口第一次获取焦点的时候摄像机会突然跳一下
* 在你的鼠标移动进窗口的那一刻，鼠标回调函数就会被调用，这时候的xpos和ypos会等于鼠标刚刚进入屏幕的那个位置。
* 这通常是一个距离屏幕中心很远的地方，因而产生一个很大的偏移量，所以就会跳了。
* 如果是，那么我们先把鼠标的初始位置更新为xpos和ypos值，这样就能解决这个问题；
*/
static bool isFirstMouseIn = true;

/**
* 鼠标监听的回调事件
*/
void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	std::cout << "(" << xpos << ", " << ypos << ")" << std::endl;
 
	//0 . 判断是不是第一鼠标进入窗口
	if (isFirstMouseIn) {
		lastX = xpos;
		lastY = ypos;
		isFirstMouseIn = false;
	}

	// 1. 计算鼠标距上一帧的偏移量。
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;  //// 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	
	lastX = xpos;
	lastY = ypos;

	//把偏移量乘以了sensitivity（灵敏度）值。如果我们忽略这个值，鼠标移动就会太大了；
	//你可以自己实验一下，找到适合自己的灵敏度值。
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// 2. 	把偏移量添加到摄像机的俯仰角和偏航角中。
	yaw += xoffset;
	pitch += yoffset;
	
	// 3. 	对偏航角和俯仰角进行最大和最小值的限制。
	//对于俯仰角，要让用户不能看向高于89度的地方（在90度时视角会发生逆转，所以我们把89度作为极限），同样也不允许小于-89度。
	if (pitch > 89.0f)	{		pitch = 89.0f;	}
	if (pitch < -89.0f)	{		pitch = -89.0f;	}
	//没有给偏航角设置限制，这是因为我们不希望限制用户的水平旋转

	// 4. 计算方向向量。
	//计算出来的方向向量就会包含根据鼠标移动计算出来的所有旋转了。
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

/**
* 鼠标滚轮的监听
* 当scroll_callback函数被调用后，我们改变全局变量fov变量的内容。
* 因为45.0f是默认的视野值，我们将会把缩放级别(Zoom Level)限制在1.0f到45.0f。
* @param window  
* @param xoffset
* @param yoffset   : 当滚动鼠标滚轮的时候，yoffset值代表我们竖直滚动的大小。
*/
void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset) {
	if (fieldOfView >= 1.0f && fieldOfView <= 45.0f)
	{
		fieldOfView -= yoffset;
	}

	if (fieldOfView <= 1.0f)
	{
		fieldOfView = 1.0f;
	}

	if (fieldOfView >= 45.0f)
	{
		fieldOfView = 45.0f;
	}

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
	//glDrawArrays(GL_TRIANGLES, 0, 36);
}

void destory() {
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
