#include "test.h"
#include "shaders.h"
#include "shaderSource.h"
#include <stdio.h>

static void prepareData();

static void render(int shaderProgram);

static void onDestroy(int shaderProgram);

static unsigned int VAO, VBO;

int practiseDrawTriangles2() {

	GLFWwindow * window = createGLWindow(800, 600, "Practise Draw Triangle2");
	if (window == NULL) {
		return -1;
	}

	int shaderProgram = makeShaderProgram(cVertexShaderSource, cFragmentShaderSource);
	if (shaderProgram < 0)
	{
		return -1;
	}

	prepareData();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
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
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void render(int shaderProgram) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 2, 3);

}

void onDestroy(int shaderProgram) {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);
}
