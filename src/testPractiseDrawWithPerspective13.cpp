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
	// ---- λ�� ----       ---- ��ɫ ----     - �������� -
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

	//���������Ϣ��һ��Z����(Z-buffer)�У�Ҳ����Ϊ��Ȼ���(Depth Buffer)��
	//GLFW���Զ�Ϊ����������һ������
	//ͨ��glEnable������������Ȳ��ԡ�
	glEnable(GL_DEPTH_TEST);       //glDisable()���Թر�����OpenGL����

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//ÿ����Ⱦ����֮ǰ�����Ȼ��壨����ǰһ֡�������Ϣ��Ȼ�����ڻ����У���
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

	glUseProgram(shaderProgram);  //����uniformʱ�������ȵ���glUseProgram()

	int locTexture = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(locTexture, 0);

	int locTexture2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(locTexture2, 1);

	//-----------------------------�������ñ任����-----------------------------------
	useMatrix();

	return true;
}

/**
* Ӧ��
* Ӧ�þ���任����������ϵ
*/
void useMatrix() {

	int locUniformModel = glGetUniformLocation(shaderProgram, "model");
	int locUniformView = glGetUniformLocation(shaderProgram, "view");
	int locUniformProjection = glGetUniformLocation(shaderProgram, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//�����Ǳ任һ�����ǵ�ƽ�棬��������x����ת��ʹ������������ڵ���һ����
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	//// ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���  //�������ӽ���һ������ε�
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f + getFVal() * (-10.0f)));
	//ͶӰ����,����ϣ���ڳ�����ʹ��͸��ͶӰ
	//perspective()  4������
	//��һ�������� fov �� ��ʾ������Ұ Field of View, ͨ������Ϊ45f���ܱ�ʾһ����ʵ�Ĺ۲�Ч��; 
	//									����Ҫһ��ĩ�շ��Ľ������Խ�������һ�������ֵ��
	//�ڶ��������� �����˿�߱ȣ� ���ӿڵĿ���Ը����á�
	//�����ĸ����� ������ƽ��ͷ��Ľ���Զƽ�档 
	//									ͨ�����ý�����Ϊ0.1f,  
	//												Զ����Ϊ100f��  �����ڽ�ƽ���Զƽ�����Ҵ���ƽ��ͷ���ڵĶ��㶼�ᱻ��Ⱦ
	// ����ѵ���������near���ù�������10f�� ��Ὣ��������������꣨��0.0f��10.f֮�䣩���ü���
	// ��ᵼ�µ��Ӿ�Ч������̫������һ�������ʱ��������߻�ֱ�Ӵ���ȥ
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
