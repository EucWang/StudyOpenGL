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

static void processInput(GLFWwindow * window);

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

static float cameraSpeed = 0.005f;  //���ư���ʱ��view�ľ�ͷ���ƶ�����

static float deltaTime = 0.0f;   //��ǰ֡����һ֡��ʱ���
static float lastFrame = 0.0f;   //��һ֡��ʱ��

static glm::mat4 model;     //ģ�;���   ��λ�ã��Ƕ�
static glm::mat4 view;        //�ӽǾ���  ����� Զ��������
static glm::mat4 projection;     //�۲��߾��� ͸�Ӿ���

static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);        //���λ��
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);    //��cameraPos��ӵõ��������
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);        //������λ�����ϵ�����

/**
* ��ϰ
* �������������� ������ ��ͷ�ӽ�
*/
int practiseDrawWithPerspective15(const char * projectDir) {
	std::cout << "practiseDrawWithPerspective15" << std::endl;

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

	return true;
}


void useMatrix() {

	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);

	//�����Ǳ任һ�����ǵ�ƽ�棬��������x����ת��ʹ������������ڵ���һ����
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model,  glm::radians(30.0f), glm::vec3(0.2f, 0.3f, 0.1f));

	//cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	//cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	//cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//�����ǵ�ǰ��λ�ü������Ǹոն���ķ��������� 
	//�����ܱ�֤����������ô�ƶ������������ע����Ŀ�귽��
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//ͶӰ����,����ϣ���ڳ�����ʹ��͸��ͶӰ
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600, 0.1f, 100.0f);

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}


/**
* ���ư��� W, S, A, D ���� �� �ƶ��ӽǾ���
*/
void processInput(GLFWwindow* window) {
	cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	{
		cameraPos += cameraSpeed * cameraFront;
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize( glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}


void render() {
	//��Щ��������ܻ��������ÿ����Ƹ���֡��Ҳ�����Ը��ߵ�Ƶ�ʵ���processInput������
	//������ǣ��������õĲ�ͬ����Щ�˿����ƶ��ܿ죬����Щ�˻��ƶ�������
	//��Ҫ���ٻ���ÿ֡��ʱ���
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

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