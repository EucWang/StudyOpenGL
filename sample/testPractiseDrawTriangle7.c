#include "../include/shaders.h"
#include "../include/test.h"
#include "../include/shaderSource.h"
#include <stdio.h>

static void prepare();

static void render(int shaderProgram);

static void destroy(int shaderProgram);

int practiseDrawTriangle7() {

	GLFWwindow* window = createGLWindow(800, 640, "Practise Draw Triangle7");
	if (window == NULL) {
		return -1;
	}

	int shaderProgram = makeShaderProgram(fVertexShaderSource, fFragmentShaderSource);
	if (shaderProgram < 0) {
		return -1;
	}

	prepare();


	printf("%s\n", "prepare() done. then next run into while...\n");
	while (!glfwWindowShouldClose(window))	{
		processInput(window);  //ÊäÈë

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

static unsigned int VAO;
static unsigned int VBO;

void prepare() {
	printf("%s\n", "prepare() called.");
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)) );
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render(int shaderProgram) {
	//printf("%s\n", "render() runing...");

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void destroy(int shaderProgram) {
	printf("%s\n", "destroy()");
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
