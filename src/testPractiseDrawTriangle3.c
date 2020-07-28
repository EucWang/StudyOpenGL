#include "test.h"
#include "shaders.h"

#include <stdio.h>


static void prepare();

static void render(unsigned int shaderProgram);

static void destroy(unsigned int shaderProgram);

int practiseDrawTriangles3() {

	GLFWwindow * window = createGLWindow(800, 640, "Practise Draw Triangle3");
	if (window == NULL) {
		return -1;
	}

	int shaderProgram = makeShaderProgram(cVertexShaderSource, cFragmentShaderSource);
	if (shaderProgram < 0) {
		return -1;
	}

	prepare();

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy(shaderProgram);

	glfwTerminate();
	return 1;
}

unsigned int VAO1, VAO2, VBO1, VBO2;

void prepare() {
	float vertex1[] = {
		-0.5, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	float vertex2[] = {
	0.0f, 0.0f, 0.0f ,
	0.5f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f,
	};

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//然后再绑定第二个

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2), vertex2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render(unsigned int shaderProgram) {
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void destroy(unsigned int shaderProgram) {

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	glDeleteProgram(shaderProgram);
}