#include "test.h"
#include "shaders.h"
#include <stdio.h>

static void prepareData();

static void render(int shaderProgram);

static void onDestroy(int shaderProgram);

unsigned int VAO, VBO, EBO;

int practiseDrawTriangles1() {

	GLFWwindow * window = createGLWindow(800, 640, "Practise Draw Triangle1");
	if (window == NULL) {
		return -1;
	}

	int shaderProgram = makeShaderProgram(cVertexShaderSource, cFragmentShaderSource);
	if (shaderProgram < 0) {
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


static void prepareData() {

	float vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	int slices[] = {
		0, 1, 2,
		2, 3, 4
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(slices), slices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void render(int shaderProgram) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);
}

static void onDestroy(int shaderProgram) {
	glDeleteVertexArrays(0, &VAO);
	glDeleteBuffers(0, &VBO);
	glDeleteBuffers(0, &EBO);
	glDeleteProgram(shaderProgram);
}
