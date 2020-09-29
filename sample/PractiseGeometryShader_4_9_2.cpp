#include "PractiseGeometryShader_4_9_2.h"


static double deltaTime;
static double lastFrame;
static double lastX = DEFAULT_SCREEN_WIDTH / 2, lastY = DEFAULT_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 2.0f, 1.0f));

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

int PractiseGeometryShader_4_9_2::practise(string projectDir) {

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"sampler of geometry shader", buffer_window_callback);

	if (window == NULL) { return -1; }

	MyShader myshader(projectDir.c_str(), vertFile, fragFile, geomFile);
	myshader.setInt("texture_diffuser1", 0);

	MyShader myshader2(projectDir.c_str(), vertFile2, fragFile2, geomFile2);
	myshader2.setInt("texture_diffuse1", 0);

	MyShader myshader3(projectDir.c_str(), vertFile3, fragFile3);
	myshader3.setInt("texture_diffuse1", 0);

	char* modelPath;
	if (!getChildPath(&modelPath, projectDir.c_str(), modelFile)){return -1; }
 	std::cout << "modelPath is :" << modelPath << std::endl;

	Model nanoModel(modelPath);

	MyShader planeshader(projectDir.c_str(), vertFilePlane, fragFilePlane);
	planeshader.use();
	planeshader.setInt("texture_diffuse1", 0);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayFromSubData(&planeVAO, &planeVBO, false,
		planePosition, sizeof(planePosition), 3,
		planeNormal, sizeof(planeNormal), 3,
		planeTexCoords, sizeof(planeTexCoords), 2);

	int texPlane = RenderUtil::textureLoad2D(projectDir.c_str(), planeFile, true);

	glUniformBlockBinding(myshader3.id,
		glGetUniformBlockIndex(myshader3.id, "Matrices4"), 0);
	glUniformBlockBinding(myshader2.id, 
		glGetUniformBlockIndex(myshader2.id, "Matrices3"), 0);
	glUniformBlockBinding(myshader.id, 
		glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	glUniformBlockBinding(planeshader.id,
		glGetUniformBlockIndex(planeshader.id, "Matrices2"), 0);
	GLuint uniformBuffer;
	glGenBuffers(1, &uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBuffer, 0, 2 * sizeof(glm::mat4));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------render
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//-----------------render done

		//------------------ 第一套纳米服
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		myshader.use();
		myshader.setMat4("model", model);
		myshader.setFloat("time", (float)curFrame);
		nanoModel.draw(&myshader);

		//------------------ 第二套纳米服
		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, glm::vec3(3.0f, -0.5f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));
		myshader2.use();
		myshader2.setMat4("model", model2);
		nanoModel.draw(&myshader2);

		//------------------ 第二套纳米服
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //线框模式显示
		glm::mat4 model3(1.0f);
		model3 = glm::translate(model3, glm::vec3(3.0f, -0.5f, 0.0f));
		model3 = glm::scale(model3, glm::vec3(0.2f, 0.2f, 0.2f));
		myshader3.use();
		myshader3.setMat4("model", model3);
		nanoModel.draw(&myshader3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //填充模式显示

		//------------------ 地面
		planeshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texPlane);
		glm::mat4 model1(1.0f);
		model1 = glm::translate(model1, glm::vec3(0.0f, -0.004f, 0.0f));
		myshader.setMat4("model", model1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 1;
}