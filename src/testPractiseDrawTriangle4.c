#include "test.h"
#include "shaders.h"
#include <stdio.h>


static void prepare();

static void render(int shaderProgram1, int shaderProgram2);

static void destroy(int shaderProgram1, int shaderProgram2);

static unsigned int VAO, VBO, EBO;

int practiseDrawTriangles4() {
	printf("%s\n", "practiseDrawTriangles4() called");

	GLFWwindow* window = createGLWindow(800, 640, "Practise Draw Triangle4");
	if (window == NULL) {
		return 1;
	}

	int shaderProgram1 = makeShaderProgram(cVertexShaderSource, cFragmentShaderSource);
	int shaderProgram2 = makeShaderProgram(cVertexShaderSource, cFragmentShaderSourceYellow);

	if (shaderProgram1 < 0 || shaderProgram2 < 0)	{
		return -1;
	}

	prepare();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderProgram1, shaderProgram2);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	destroy(shaderProgram1, shaderProgram2);
	glfwTerminate();

	return 1;
}


void prepare() {
	float vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	//int slices[] = {
	//	0, 1, 2,
	//	1, 2, 3,
	//	2, 3, 4
	//};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(slices), slices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render(int shaderProgram1, int shaderProgram2) {
	glUseProgram(shaderProgram1);
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(shaderProgram2);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 1, 3);
	glDrawArrays(GL_TRIANGLES, 2, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 3);
	glBindVertexArray(0);
}

void destroy(int shaderProgram1, int shaderProgram2) {
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}