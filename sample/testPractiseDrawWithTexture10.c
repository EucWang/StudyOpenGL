#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../include/test.h" 
#include "../include/fileUtil.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

static char * vertextFile = "shader_texture_test10_vertex.txt";
static char* fragFile = "shader_texture_test10_fragment.txt";
static char* imageFile1 = "container.jpg";
static char* imageFile2 = "awesomeface.png";

static int VAO, VBO, EBO, shaderProgram, TEX1, TEX2;

static float uniRateVal = 0.2f;

static void processInput(GLFWwindow* window);

static int prepare(char * projectDir);

static void render();

static void destroy();


static const char* shaderDirName = "shader";
static char* shaderDir;

static const char* imagesDirName = "images";
static char* imagesDir;

int practiseDrawWithTexture10(char * projectDir) {

	//��ʼ����ȡ opengl��shaderĿ¼�� ͼƬĿ¼
	if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }
	if (!getChildPath(&imagesDir, projectDir, imagesDirName)) { return -1; }

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw WIth Texture 10");
	if (window == NULL) {
		return -1;
	}
	char* vertexPath;
	char* fragPath;
	if (!getChildPath(&vertexPath, shaderDir, vertextFile) ||
		!getChildPath(&fragPath, shaderDir, fragFile)) {
		return -1;
	}

	if (!createShaderProgram(vertexPath, fragPath, &shaderProgram)) {
		return -1;
	}

	if (prepare(projectDir) < 0) {
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	destroy();
	glfwTerminate();

	free(vertexPath);
	free(fragPath);
	return 1;
}

int prepare(char* projectDir) {
//	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
//	};

//	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // ����
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // ����
//	}; 
//	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - ��������  �ڶ�����������-
//				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 1.0f,   1.0f, 1.0f,    // ����
//				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   1.0f, 0.0f,   // ����
//				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    -1.0f, 0.0f,  // ����
//				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    -1.0f, 1.0f    // ����
//	}; 
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - ��������  �ڶ�����������-
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.0f, 1.0f,    // ����
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.0f, 0.0f,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    0.0f, 0.0f,  // ����
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    0.0f, 1.0f    // ����
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

//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);

//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);

//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	//����2��ͼƬ
	char* imagePath1;
	char* imagePath2;
	if (!getChildPath(&imagePath1, imagesDir, imageFile1) ||
		!getChildPath(&imagePath2, imagesDir, imageFile2)) {
		printf("%s\n", "prepare() call failed, because getChildPath() failed, cannot get imagePath1 or imagePath2");
		return -1;
	}

	int width1, height1, channel1, width2, height2, channel2;

	char * data1 = stbi_load(imagePath1, &width1, &height1, &channel1, 0);

	//��ΪOpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ��������·�����תy��
	stbi_set_flip_vertically_on_load(true);
	char* data2 = stbi_load(imagePath2, &width2, &height2, &channel2, 0);

	if (!data1) {
		printf("%s\n", "stbi_load() failed, data1 is null");
		return -1;
	}
	if (!data2) {
		printf("%s\n", "stbi_load() failed, data2 is null");
		return -1;
	}

	//����2����������

	//��һ����������
	glGenTextures(1, &TEX1); 
	glBindTexture(GL_TEXTURE_2D, TEX1);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//���Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);	//��������ͼƬ
	glGenerateMipmap(GL_TEXTURE_2D);

	//�ڶ�����������
	glGenTextures(1, &TEX2);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);		//���Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);//��������ͼƬ
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data1);  //�ͷ�ͼƬ��Դ
	stbi_image_free(data2); 
	free(imagePath1);  //�ͷ�malloc�������ַ���
	free(imagePath2);

	//Ӧ�ò���������ֵ��
	glUseProgram(shaderProgram);

	//����2������
	//ʹ��glUniform1i����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�������Ԫ��
	int uni_texture1 = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(uni_texture1, 0);
	 
	int uni_texture2 = glGetUniformLocation(shaderProgram, "texture2");
	glUniform1i(uni_texture2, 1);

	return 1;
}

void render() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX2);

	glUseProgram(shaderProgram);

	//float timeVal = (float)glfwGetTime();
	//float rateVal = (float)(sin(timeVal) / 2.0f + 0.5f);

	glUniform1f(glGetUniformLocation(shaderProgram, "rate"), uniRateVal);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

static float timeVal = 0.0f;
void processInput(GLFWwindow* window) {

	//����û��Ƿ����˷��ؼ�(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊ true�ķ����ر�GLFW
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		float newTimeVal = (float)glfwGetTime();
		printf("%s%f\n", "newTimeVal is ", newTimeVal);
		if (newTimeVal - timeVal > 0.1f) {
			timeVal = newTimeVal;
		}
		else {
			return;
		}
		if (uniRateVal + 0.1f <= 1.0f) {
			uniRateVal += 0.1f;
		}
		printf("%s%f\n", "uniRateVal is ", uniRateVal);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		float newTimeVal = (float)glfwGetTime();
		printf("%s%f\n", "newTimeVal is ", newTimeVal);
		if (newTimeVal - timeVal > 0.1f) {
			timeVal = newTimeVal;
		}
		else {
			return;
		}
		if (uniRateVal  - 0.1f>= 0.0f) {
			uniRateVal -= 0.1f;
		}
		printf("%s%f\n", "uniRateVal is ", uniRateVal);
	}
}