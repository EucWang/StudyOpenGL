#include "../include/camera.h"
#include "../include/fileUtil.h"
#include "../include/shaders.h"
#include "../include/shaderSource.h"
#include "../include/test.h"
#include "../include/util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

static const char* vertextFile = "shader_light_test18_vertex.txt";
static const char* fragFile = "shader_light_test18_fragment.txt";
static const char* lightFragFile = "shader_light_test18_fragment_light.txt";
static char* vertextFilePath;
static char* fragFilePath;
static char* lightFragFilePath;

static const char* shaderDirName = "shader";
static char* shaderDir;

static GLuint VAO, VBO;
static int shaderId, shaderLightId;
static GLuint lightVAO;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
 
static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH/2, lastY = SMALL_SCREEN_HEIGHT/2;
bool isMouseFirstIn = true;

//表示光源在场景的世界空间坐标中的位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

static float vertices[] = {
	// ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	-0.5f, -0.5f, -0.5f,     
	 0.5f, -0.5f, -0.5f,      
	 0.5f,  0.5f, -0.5f,		
	-0.5f,  0.5f, -0.5f,		
	 0.5f,  0.5f, -0.5f,		
	-0.5f, -0.5f, -0.5f,		

	-0.5f, -0.5f,  0.5f,		
	 0.5f, -0.5f,  0.5f,		
	 0.5f,  0.5f,  0.5f,		
	 0.5f,  0.5f,  0.5f,		
	-0.5f,  0.5f,  0.5f,		
	-0.5f, -0.5f,  0.5f,		

	-0.5f,  0.5f,  0.5f,		
	-0.5f,  0.5f, -0.5f,		
	-0.5f, -0.5f, -0.5f,		
	-0.5f, -0.5f, -0.5f,		
	-0.5f, -0.5f,  0.5f,		
	-0.5f,  0.5f,  0.5f,		

	 0.5f,  0.5f,  0.5f,		
	 0.5f,  0.5f, -0.5f,		
	 0.5f, -0.5f, -0.5f,		
	 0.5f, -0.5f, -0.5f,		
	 0.5f, -0.5f,  0.5f,		
	 0.5f,  0.5f,  0.5f,		

	-0.5f, -0.5f, -0.5f,		
	 0.5f, -0.5f, -0.5f,		
	 0.5f, -0.5f,  0.5f,		
	 0.5f, -0.5f,  0.5f,		
	-0.5f, -0.5f,  0.5f,		
	-0.5f, -0.5f, -0.5f,		

	-0.5f,  0.5f, -0.5f,		
	 0.5f,  0.5f, -0.5f,		
	 0.5f,  0.5f,  0.5f,		
	 0.5f,  0.5f,  0.5f,		
	-0.5f,  0.5f,  0.5f,		
	-0.5f,  0.5f, -0.5f,		
};

static bool prepare();

static void render();

static void processInput(GLFWwindow * window);

static void mouse_move_callback(GLFWwindow*, double, double);

static void mouse_scroll_callback(GLFWwindow*, double, double);

int practise2Light(const char* projectDir) {
	if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }

	GLFWwindow* window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with light");
	if (window == NULL)		{	return -1;	 }

	if (!getChildPath(&vertextFilePath, shaderDir, vertextFile)) { return -1; }
	if (!getChildPath(&fragFilePath, shaderDir, fragFile)) { return -1; }
	if (!getChildPath(&lightFragFilePath, shaderDir, lightFragFile)) { return -1; }
	if (!createShaderProgram(vertextFilePath, fragFilePath, &shaderId)) {return -1; }
	if (!createShaderProgram(vertextFilePath, lightFragFilePath, &shaderLightId)) {return -1; }

	prepare();

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderId);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	free(shaderDir);
	free(vertextFilePath);
	free(fragFilePath);
	glfwTerminate();
	return 1;
}

bool prepare() {
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	 
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	
	glUseProgram(shaderId);
	glUniform3f(glGetUniformLocation(shaderId, "objectColor"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(shaderId, "lightColor"), 1.0f, 1.0f, 1.0f);

	return true;
}

void render() {
	glUseProgram(shaderId);
	glBindVertexArray(VAO);
	 
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.Zoom), SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 
		1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 
		1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 
		1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	//画光源
	glUseProgram(shaderLightId);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, lightPos);
	model2 = glm::scale(model2, glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "model"),
		1, GL_FALSE, glm::value_ptr(model2));
	glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "view"),
		1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "projection"),
		1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void processInput(GLFWwindow* window) {
	float curFrame = glfwGetTime();
	deltaTime = (float)(curFrame - lastFrame);
	lastFrame = curFrame;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	{
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
}

void mouse_move_callback(GLFWwindow*, double xpos, double ypos) {
	if (isMouseFirstIn) {
		isMouseFirstIn = false;
		lastX = xpos;
		lastY = ypos;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);  
}

void mouse_scroll_callback(GLFWwindow*, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}