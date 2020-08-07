#include "test.h"
#include "shaders.h"
#include "shaderSource.h"
#include <stdio.h>
#include <stdbool.h> 
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

static void prepare();
static void render();
static void destroy();

unsigned int VAO, VBO, EBO, texture, shaderId;

static char* vertexPath = "D:/workspace/cppWorkspace/StudyOpenGL/shader_texture_test9_vertex.txt";
static char* fragPath = "D:/workspace/cppWorkspace/StudyOpenGL/shader_texture_test9_fragment.txt";
static char* imagePath = "D:/workspace/cppWorkspace/StudyOpenGL/container.jpg";

int testPractiseDrawWithTexure9() {
	GLFWwindow * window = createGLWindow(800, 640, "Draw Triangle With Texure");
	if (window == NULL) {
		return -1;
	}

	if (!createShaderProgram(vertexPath, fragPath, &shaderId)) {
		return -1;
	}
	
	prepare(); 
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy();
	glfwTerminate();
	return 1;
}

void prepare() {
	float vertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
		//	-1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f
	};
	unsigned int slices[] = {
		0, 1, 3,
		1, 2, 3,
	//	2, 3, 4
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(slices), slices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenTextures(1, &texture); //�����������
	glBindTexture(GL_TEXTURE_2D, texture);  //���������

	//�����Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x��Ļ��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y��Ļ��Ʒ�ʽ

	//�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //��Сʱ�� ���ƹ��ˣ��������ط��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //�Ŵ�ʱ��ֱ�߹��ˣ� ����ƽ������

	//�༶�������� , �����Խʱ�Ĺ��˷���
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//����ͼƬ
	int width, height, nrChannels;

	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (data) {
		//��ͼƬ���ݲ���һ������
		//��һ�������� ָ������Ŀ�꣬
		//		 ����ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ�����
		//		���κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
		//�ڶ�����������ָ���༶��Զ����ļ���
		//		�����ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ�������������0��Ҳ���ǻ�������
		//����������  ��������Ϊ���ָ�ʽ
		//		���ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
		//���ģ�������� �������յ�����Ŀ�Ⱥ͸߶�
		//����������   ���Ǳ���Ϊ0����ʷ���������⣩��
		//���ߣ��˸�����  Դͼ�ĸ�ʽ���������͡�
		//�ھŸ�����  ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//��ǰ�󶨵��������ͻᱻ����������ͼ��
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		printf("%s\n", "failed to load texture.");
		return -1;
	}

	stbi_image_free(data);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void render() {
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shaderId);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroy() {
	glDeleteProgram(shaderId);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}