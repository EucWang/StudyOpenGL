#include <stdio.h>
#include "test.h"
#include "shaders.h"

static void prepare();
static void render(unsigned int shaderProgram);
static void destroy(unsigned int shaderProgram);

static int VAO, VBO;

int practiseDrawTriangles5() {
	printf("%s\n", "practiseDrawTriangles5()");

	GLFWwindow * window = createGLWindow(800, 640, "Practise Draw Triangles5");
	if (window == NULL)
	{
		return -1;
	}

	// 再顶点着色器中定义了颜色传递到片段着色器中，为颜色赋值
	int shaderProgram = makeShaderProgram(dVertexShaderSource, dFragmentShaderSource);
	if (shaderProgram < 0){
		return -1;
	}

	prepare();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderProgram);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	destroy(shaderProgram);
	glfwTerminate();
	return 1;
}

void prepare() {
	float vertices[] = {
		0.7f, 0.6f, 0.0f,
		-0.7f, 0.6f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render(unsigned int shaderProgram) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void destroy(unsigned int shaderProgram) {

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}