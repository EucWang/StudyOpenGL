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

	 //useMatrix();

	return true;
}

void useMatrix() {

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//�����Ǳ任һ�����ǵ�ƽ�棬��������x����ת��ʹ������������ڵ���һ����
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model,  glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	//// ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���

	float radius = 5.0f;
	float camX = cos(glfwGetTime()) * radius;
	//float camY = cos(glfwGetTime()) * radius;
	float camZ = sin(glfwGetTime()) * radius;

	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));

	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//ͶӰ����,����ϣ���ڳ�����ʹ��͸��ͶӰ
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600, 0.1f, 100.0f);

	glUniformMatrix4fv(locUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(locUniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locUniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);


	//�����Ǳ任һ�����ǵ�ƽ�棬��������x����ת��ʹ������������ڵ���һ����
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
*  ��ù۲����
*/
void getLookAt(glm::mat4* lookAt) {
	//����һ�������λ�ã� vec3 ���� ,  
	//��Ҫ������z���Ǵ���Ļָ����ģ��������ϣ�����������ƶ������Ǿ�����z����������ƶ���
//	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	//�������ָ�򳡾�ԭ�㣺(0, 0, 0)�����ǵ����������ʸ����������Ǿ��ܵõ�������ʸ���Ĳ���
	//�ó���ԭ��������ȥ�����λ�������Ľ�������������ָ��������
	//�����ָ��z�Ḻ��������ϣ����������(Direction Vector)ָ���������z��������
	//���������˳�����Ǿͻ���һ��ָ���������z�᷽���������
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);  //����ԭ��
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	//������(Up Vector)
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  //y������
	//���������͵ڶ����õ��ķ����������в�ˡ�����������˵Ľ����ͬʱ��ֱ����������
	//������ǻ�õ�ָ��x����������Ǹ�������������ǽ�������������˵�˳��ͻ�õ��෴��ָ��x�Ḻ�������������
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));   //�õ��������������ᣬ������������ռ��x���������

	//�õ����������y������
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);//���ǰ��������ͷ����������в�ˣ�

	//ʹ����Щ������������ǾͿ��Դ���һ��LookAt�����ˣ����ڴ����������ʱ��ǳ����á�


	//GLM�Ѿ��ṩ����Щ֧�֡�
	//����Ҫ����ֻ�Ƕ���һ�������λ�ã�һ��Ŀ��λ�ú�һ����ʾ����ռ��е������������������Ǽ���������ʹ�õ��Ǹ�����������
	//����GLM�ͻᴴ��һ��LookAt�������ǿ��԰����������ǵĹ۲����
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
