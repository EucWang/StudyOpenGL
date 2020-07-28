#include "shaders.h"
#include "test.h"
#include <stdio.h>

const char* vertexShaderSource3 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

static unsigned int VBO, VAO, EBO;

static void prepareData();

static void render(int shaderProgram);

static void onDestroy(int shaderProgram);

int drawUnfillRectangle() {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "DRAW UNFILL RECTANGLE", NULL, NULL);
	if (window == NULL)	{
		printf("%s\n", "create glfwWindow failed. exit");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		printf("%s\n", "Failed to Initialize glad.");
		return -1;
	}

	int shaderProgram = makeShaderProgram(vertexShaderSource3, fragmentShaderSource3);
	if (shaderProgram < 0)	{
		return -1;
	}

	prepareData();

	while (!glfwWindowShouldClose(window))	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	onDestroy(shaderProgram);
	glfwTerminate();

	return 1;
}


void prepareData() {
	float vertices[] = {
		0.5f, 0.5f, 0.0f,  //top right
		0.5f, -0.5f, 0.0f,   //bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f, 0.5f, 0.0f  //top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	//һ�������������ᴢ��������Щ���ݣ�
	//  glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ��á�
	//	ͨ��glVertexAttribPointer���õĶ����������á�
	//	ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//�Ȱ�VAO�� �Ӱ�֮��������Ӧ�ð󶨺����ö�Ӧ��VBO������ָ�룬֮����VAO��֮��ʹ�á�
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���Ӷ�������
	//ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//ʹ��glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õġ�
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  //������VBO�� ���ǲ�Ҫ���EBO
	glBindVertexArray(0);       //���VAO���������õ�VBO��EBO����ӵİ󶨵�VAO���ˣ�����ʱ�����°󶨼���

	//�߿�ģʽ(Wireframe Mode)
	//��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ��棬�ڶ������������������������ơ�
	//��Ļ��Ƶ��û�һֱ���߿�ģʽ���������Σ�
	//ֱ��������glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�������û�Ĭ��ģʽ��
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void render(int shaderProgram) {

	glUseProgram(shaderProgram);  //ʹ����ɫ������
	glBindVertexArray(VAO);      //���°�VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //��������
}

void onDestroy(int shaderProgram) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}