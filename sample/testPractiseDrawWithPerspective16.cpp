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
* �����ƶ������ص��¼�
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

	//��ʼ����ȡ opengl��shaderĿ¼�� ͼƬĿ¼
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

	glEnable(GL_DEPTH_TEST);  //������Ȳ��ԣ���Ĭ���ǹرյġ�
	//���ع�꣬����׽(Capture)��꣬���Ӧ��ͣ���ڴ����У����ǳ���ʧȥ��������˳�����
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//��������ƶ��¼���ע��ص�����
	glfwSetCursorPosCallback(window, mouse_callback);

	//�����������¼�
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

	//-----------------------------�������ñ任����-----------------------------------

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


//�ӽ��ƶ��� ŷ���ǣ�  �����Ǻ�ƫ�������������
static float pitch;    //������
static float yaw;     //ƫ����
static float roll;     // ��ת�ǣ� ������������������

//��Ұ(Field of View)��fov���������ǿ��Կ��������ж��ķ�Χ
//ƽ��ͷ��͸�Ӿ���ĵ�һ������
static float fieldOfView = 45.0f;  

void useMatrix() {

	 model = glm::mat4(1.0f);
	 view = glm::mat4(1.0f);
	 projection = glm::mat4(1.0f);

	//�����Ǳ任һ�����ǵ�ƽ�棬��������x����ת��ʹ������������ڵ���һ����
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, getFVal() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


	//ͶӰ����,����ϣ���ڳ�����ʹ��͸��ͶӰ
	//��֮ǰ�Ľ̳�������˵��Ұ(Field of View)��fov���������ǿ��Կ��������ж��ķ�Χ��
	//����Ұ��Сʱ������ͶӰ�����Ŀռ�ͻ��С�������Ŵ�(Zoom In)�˵ĸо���
	//���ǻ�ʹ�����Ĺ������Ŵ�
	projection = glm::perspective(glm::radians(fieldOfView), 800.0f / 600, 0.1f, 100.0f);

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	 
}

static float lastFrame = 0.0f;

/**
* ���ư��� W, S, A, D ���� �� �ƶ��ӽǾ���
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
* ���ڵ�һ�λ�ȡ�����ʱ���������ͻȻ��һ��
* ���������ƶ������ڵ���һ�̣����ص������ͻᱻ���ã���ʱ���xpos��ypos��������ոս�����Ļ���Ǹ�λ�á�
* ��ͨ����һ��������Ļ���ĺ�Զ�ĵط����������һ���ܴ��ƫ���������Ծͻ����ˡ�
* ����ǣ���ô�����Ȱ����ĳ�ʼλ�ø���Ϊxpos��yposֵ���������ܽ��������⣻
*/
static bool isFirstMouseIn = true;

/**
* �������Ļص��¼�
*/
void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	std::cout << "(" << xpos << ", " << ypos << ")" << std::endl;
 
	//0 . �ж��ǲ��ǵ�һ�����봰��
	if (isFirstMouseIn) {
		lastX = xpos;
		lastY = ypos;
		isFirstMouseIn = false;
	}

	// 1. ����������һ֡��ƫ������
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;  //// ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
	
	lastX = xpos;
	lastY = ypos;

	//��ƫ����������sensitivity�������ȣ�ֵ��������Ǻ������ֵ������ƶ��ͻ�̫���ˣ�
	//������Լ�ʵ��һ�£��ҵ��ʺ��Լ���������ֵ��
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// 2. 	��ƫ������ӵ�������ĸ����Ǻ�ƫ�����С�
	yaw += xoffset;
	pitch += yoffset;
	
	// 3. 	��ƫ���Ǻ͸����ǽ���������Сֵ�����ơ�
	//���ڸ����ǣ�Ҫ���û����ܿ������89�ȵĵط�����90��ʱ�ӽǻᷢ����ת���������ǰ�89����Ϊ���ޣ���ͬ��Ҳ������С��-89�ȡ�
	if (pitch > 89.0f)	{		pitch = 89.0f;	}
	if (pitch < -89.0f)	{		pitch = -89.0f;	}
	//û�и�ƫ�����������ƣ�������Ϊ���ǲ�ϣ�������û���ˮƽ��ת

	// 4. ���㷽��������
	//��������ķ��������ͻ������������ƶ����������������ת�ˡ�
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

/**
* �����ֵļ���
* ��scroll_callback���������ú����Ǹı�ȫ�ֱ���fov���������ݡ�
* ��Ϊ45.0f��Ĭ�ϵ���Ұֵ�����ǽ�������ż���(Zoom Level)������1.0f��45.0f��
* @param window  
* @param xoffset
* @param yoffset   : �����������ֵ�ʱ��yoffsetֵ����������ֱ�����Ĵ�С��
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
