#include "PractiseAdvancedGLSL_4_8.h"

static double deltaTime;
static float lastFrame;
static double lastX = DEFAULT_SCREEN_WIDTH / 2, lastY = DEFAULT_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));

static void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

static void mouse_move_callback(GLFWwindow* window, double posX, double posY) {
	if (isMouseFirstIn) {
		isMouseFirstIn = false;
		lastX = posX;
		lastY = posY;
	}

	double offsetX = posX - lastX;
	double offsetY = lastY - posY;

	lastX = posX;
	lastY = posY;

	camera.ProcessMouseMovement(offsetX, offsetY);
}

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.ProcessMouseScroll(offsetY);
}

static void buffer_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int PractiseAdvancedGLSL_4_8::practise(string projectDir) {

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 
		"draw cube with advanced GLSL", buffer_window_callback);
	if (window == NULL) { return -1; }

	MyShader myshader(projectDir.c_str(), vertFile, fragFile);

	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, cubePosition, sizeof(cubePosition),
		3, 0, 0, 0, 0, 0, 0);

	GLuint someVAO, someVBO;
	glGenVertexArrays(1, &someVAO);
	glGenBuffers(1, &someVBO);
	glBindVertexArray(someVAO);
	glBindBuffer(GL_ARRAY_BUFFER, someVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(somePosition), somePosition, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	int somePositionIndex[] = {
		0, 1, 2,
		0, 2, 3
	};
	GLuint someVAO2, someVBO2, someEBO;
	glGenVertexArrays(1, &someVAO2);
	glGenBuffers(1, &someVBO2);
	glGenBuffers(1, &someEBO);
	glBindVertexArray(someVAO2);
	glBindBuffer(GL_ARRAY_BUFFER, someVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(somePosition), somePosition, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, someEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(somePositionIndex), somePositionIndex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //开启线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //开启颜色填充模式

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		myshader.use();
		//glBindVertexArray(VAO);
		glBindVertexArray(someVAO);
		myshader.setVec3("cubeColor", 1.0, 1.0, 1.0);

		glm::mat4 model(1.0);
		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), 
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 100.0f);

		myshader.setMat4("model", model);
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);

		//glDrawArrays(GL_TRIANGLES, 0, 36);  //画三角形
		glDrawArrays(GL_POINTS, 0, 4);  //画点

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(someVAO2);
		myshader.setVec3("cubeColor", 1.0, 0.5, 0.5);
		glm::mat4 model3(1.0);
		model3 = glm::translate(model3, glm::vec3(0.0f, 0.0f, 1.0f));
		myshader.setMat4("model", model3);
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO);
		glm::mat4 model1(1.0);
		model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.0f));
		myshader.setMat4("model", model1);
		myshader.setVec3("cubeColor", 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &someVAO);
	glDeleteVertexArrays(1, &someVAO2);
	glDeleteBuffers(1, &someVBO);
	glDeleteBuffers(1, &someVBO2);
	glDeleteBuffers(1, &someEBO);
	myshader.deleteProgram();
}