#include "PractiseGeometryShader_4_9.h"

static double deltaTime;
static double lastFrame;
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
 
int PractiseGeometryShader_4_9::practise(string projectDir) {

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"Practise Geometry Shader.", buffer_window_callback);
	if (window == NULL) { return -1; }

	//MyShader myshader(projectDir.c_str(), vertFile, fragFile);
	//MyShader myshader(projectDir.c_str(), vertFile, fragFile, geomFile);
	MyShader myshader(projectDir.c_str(), vertFile, fragFile, geomFile2);

	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO,
		points, sizeof(points), 2,
		pointsColor, sizeof(pointsColor), 3);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_move_callback);
	//glfwSetScrollCallback(window, mouse_scroll_callback);

	//glUniformBlockBinding(myshader.id, 
	//	glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	//
	//GLuint UBO;
	//glGenBuffers(1, &UBO);
	//glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	//glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//
	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glm::mat4 view(1.0f);
		//glm::mat4 projection(1.0f);
		//view = camera.GetViewMatrix();
		//projection = glm::perspective(glm::radians(camera.Zoom), DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT,
		//	0.1f, 100.0f);
		//
		//glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		//glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//-------------render
		myshader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);

		//-------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}