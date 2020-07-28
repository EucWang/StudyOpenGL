#include "shaders.h"
#include "test.h"
#include <stdio.h>

const char* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VBO, VAO, EBO;

static void prepareData();

static void render(int shaderProgram);

static void onDestroy(int shaderProgram);

int drawRectangle() {
	GLFWwindow* window = createGLWindow(SCR_WIDTH, SCR_HEIGHT, "Learn_Opengl");
	if (window == NULL)	{
		return -1;
	}

	int shaderProgram = makeShaderProgram(vertexShaderSource2, fragmentShaderSource2);
	if (makeShaderProgram < 0)		{
		return -1;
	}

	prepareData();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
	
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	onDestroy(shaderProgram);

	glfwTerminate();
	return 1;
}

//准备数据，并将数据传递给Opengl的缓冲区
void prepareData() {

	float vertices[] = {
		0.5f, 0.5f, 0.0f,  //top right
		0.5f, -0.5f, 0.0f,   //bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f, 0.5f, 0.0f  //top left
	};

	unsigned int indices[] = {
		0, 1, 3,  //first triangle
		1, 2, 3  //second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render(int shaderProgram) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void onDestroy(int shaderProgram) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}