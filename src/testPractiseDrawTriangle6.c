#include "shaders.h"
#include "test.h"
#include <stdio.h>
#include <math.h>

static int VAO, VBO;

static void prepare();

static void render(int shaderProgram);

static void destroy(int shaderProgram);

int practiseDrawTriangle6() {
	printf("%s\n", "practiseDrawTriangle6()");

	GLFWwindow* window = createGLWindow(800, 640, "Practise Draw Triangle6");
	if (window == NULL)
	{
		return -1;
	}

	int shaderProgram = makeShaderProgram(cVertexShaderSource, eFragmentShaderSource);
	if (shaderProgram < 0)
	{
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

static void prepare() {

	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
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

static void render(int shaderProgram) {
	glBindVertexArray(VAO);

	float timeVal = (float)glfwGetTime();
	float greenVal = (float)(sin(timeVal) / 2.0f + 0.5f);

	//获得Uniform类型的变量
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	
	glUseProgram(shaderProgram);  //必须在glUniform4f() 之前

	glUniform4f(vertexColorLocation, 0.0f, greenVal, 0.0f, 1.0f); //设置渐变的颜色给uniform变量

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

static void destroy(int shaderProgram) {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteProgram(shaderProgram);
}