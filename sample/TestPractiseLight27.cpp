#include "../include/TestPractiseLight27.h"

static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
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


int TestPractiseLight27::practiseLight27(const char * projectDir) {

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "draw model practise");
	if (window == NULL)	{		
		glfwTerminate();		
		return -1;	
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glEnable(GL_DEPTH_TEST);

	MyShader ourShader(projectDir, vertextFile, fragFile);

	char* modelObjPath;
	if (!getChildPath(&modelObjPath, projectDir, modelFile)) { return -1; }
	std::cout << "modelObjPath : " << modelObjPath << std::endl;
	Model ourModel(modelObjPath);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
	
		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ourShader.use();

		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);

		projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SMALL_SCREEN_WIDTH / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		ourModel.draw(&ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}
