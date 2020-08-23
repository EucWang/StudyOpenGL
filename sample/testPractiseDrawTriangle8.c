#include "../include/test.h"
#include "../include/shaderSource.h"
#include "../include/shaders.h"
#include <stdio.h>

static void prepare();

static void render(int shaderProgram);

static void destroy(int shaderProgram);

int practiseDrawTriangle8() {

	GLFWwindow * window =createGLWindow(800, 640, "Draw Triangle 8");
	if (window == NULL) {
		return -1;
	}

	char* vertexPath = "D:/workspace/cppWorkspace/StudyOpenGL/shader_triangle_test8_vertex.txt";
	char* fragPath = "D:/workspace/cppWorkspace/StudyOpenGL/shader_triangle_test8_frag.txt";

	int shaderId;
	if (!createShaderProgram(vertexPath, fragPath, &shaderId)) {
		return -1;
	}

	prepare();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderId);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy(shaderId);
	glfwTerminate();

	return 1;
}

static unsigned int VAO;
static unsigned int VBO;


void prepare() {
	printf("%s\n", "prepare() call");

	float vertices[] = {
		0.5f,   -0.5f,  0.0f,	   1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.0f,	   0.0f, 1.0f, 0.0f,
		0.0f,   0.5f,   0.0f,		0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void render(int shaderProgram) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void destroy(int shaderProgram) {
	printf("%s%d%s\n", "call destroy(", shaderProgram, ")");
	glDeleteProgram(shaderProgram);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}