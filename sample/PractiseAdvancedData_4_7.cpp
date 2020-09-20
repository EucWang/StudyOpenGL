#include "PractiseAdvancedData_4_7.h"

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

void buffer_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int PractiseAdvancedData_4_7::practise(string projectDir) {
	std::cout << "PractiseAdvancedData_4_7::practise() runing .." << std::endl;

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"draw box to practise advanced data.",  buffer_window_callback);
	if (window == NULL) { return -1; }

	MyShader myshader(projectDir.c_str(), vertFile.c_str(), fragFile.c_str());
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);
	myshader.setInt("texture_specular1", 1);

	MyShader floorshader(projectDir.c_str(), vertFile.c_str(), fragFileFloor.c_str());
	floorshader.use();
	floorshader.setInt("texture_diffuse1", 0);

	MyShader lightshader(projectDir.c_str(), vertFile.c_str(), fragFileLight.c_str());

	int texContainer = RenderUtil::textureLoad2D(projectDir.c_str(), imgFileContainer);
	int texContainerSpecular = RenderUtil::textureLoad2D(projectDir.c_str(), imgFileContainerSpecular);

	int texFloor = RenderUtil::textureLoad2D(projectDir.c_str(), imgFilePlane);

	GLuint VAO, VBO;
	//RenderUtil::makeVertexArrayAndBuffer(&VAO, &VBO, cubeVertices, sizeof(cubeVertices), 8);
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, 
		cubePosition, sizeof(cubePosition), 3,
		cubeNormal, sizeof(cubeNormal), 3, 
		cubeTexCoords, sizeof(cubeTexCoords), 2);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//--------------------render

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT,
			0.1f, 100.0f);

		myshader.use();
		glBindVertexArray(VAO);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texContainerSpecular);

		///////////
		//定向光
		myshader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
		myshader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
		myshader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
		myshader.setVec3(("dirlight.direction"), -0.2f, -1.0f, -0.3f);
		myshader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

		//1个点光源
		myshader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);
		myshader.setVec3(("pointlight.ambient"), pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		myshader.setVec3(("pointlight.diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		myshader.setVec3(("pointlight.specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		
		myshader.setFloat(("pointlight.constant"), pointLightColors[4].x);
		myshader.setFloat(("pointlight.linear"), pointLightColors[4].y);
		myshader.setFloat(("pointlight.quadratic"), pointLightColors[4].z);

		//1个聚光
		myshader.setVec3(("spotlight.ambient"), spotlightArgs[0].x, spotlightArgs[0].y, spotlightArgs[0].z);
		myshader.setVec3(("spotlight.diffuse"), spotlightArgs[1].x, spotlightArgs[1].y, spotlightArgs[1].z);
		myshader.setVec3(("spotlight.specular"), spotlightArgs[2].x, spotlightArgs[2].y, spotlightArgs[2].z);
		myshader.setVec3(("spotlight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
		myshader.setVec3(("spotlight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);

		myshader.setFloat(("spotlight.cutoff"), glm::cos(glm::radians(spotlight_cutoff.x)));
		myshader.setFloat(("spotlight.outCutoff"), glm::cos(glm::radians(spotlight_cutoff.y)));
		myshader.setFloat(("spotlight.constant"), spotlightArgs[3].x);
		myshader.setFloat(("spotlight.linear"), spotlightArgs[3].y);
		myshader.setFloat(("spotlight.quadratic"), spotlightArgs[3].z);

		myshader.setVec3("viewPos", camera.Front.x, camera.Front.y, camera.Front.z);
		myshader.setFloat("shininess", 64.0f);

		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)curFrame * glm::radians(15.0f), glm::vec3(0.0f, 0.5f, 0.0f));

		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

////////////////////

		floorshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texFloor);

		glm::mat4 model2(1.0);
		model2 = glm::translate(model2, glm::vec3(0.0f, -0.005f, 0.0f));

		floorshader.setMat4("view", view);
		floorshader.setMat4("projection", projection);
		floorshader.setMat4("model", model2);

		///////////
		//定向光
		floorshader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
		floorshader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
		floorshader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
		floorshader.setVec3(("dirlight.direction"), -0.2f, -1.0f, -0.3f);
		floorshader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

		//1个点光源
		floorshader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);
		floorshader.setVec3(("pointlight.ambient"), pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		floorshader.setVec3(("pointlight.diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		floorshader.setVec3(("pointlight.specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		
		floorshader.setFloat(("pointlight.constant"), pointLightColors[4].x);
		floorshader.setFloat(("pointlight.linear"), pointLightColors[4].y);
		floorshader.setFloat(("pointlight.quadratic"), pointLightColors[4].z);

		//1个聚光
		floorshader.setVec3(("spotlight.ambient"), spotlightArgs[0].x, spotlightArgs[0].y, spotlightArgs[0].z);
		floorshader.setVec3(("spotlight.diffuse"), spotlightArgs[1].x, spotlightArgs[1].y, spotlightArgs[1].z);
		floorshader.setVec3(("spotlight.specular"), spotlightArgs[2].x, spotlightArgs[2].y, spotlightArgs[2].z);
		floorshader.setVec3(("spotlight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
		floorshader.setVec3(("spotlight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
		
		floorshader.setFloat(("spotlight.cutoff"), glm::cos(glm::radians(spotlight_cutoff.x)));
		floorshader.setFloat(("spotlight.outCutoff"), glm::cos(glm::radians(spotlight_cutoff.y)));
		floorshader.setFloat(("spotlight.constant"), spotlightArgs[3].x);
		floorshader.setFloat(("spotlight.linear"), spotlightArgs[3].y);
		floorshader.setFloat(("spotlight.quadratic"), spotlightArgs[3].z);
		
		floorshader.setVec3("viewPos", camera.Front.x, camera.Front.y, camera.Front.z);
		floorshader.setFloat("shininess", 64.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

/////////

		lightshader.use();
		glBindVertexArray(VAO);
		lightshader.setVec3("lightColor", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		
		lightshader.setMat4("view", view);
		lightshader.setMat4("projection", projection);
		
		glm::mat4 model3 = glm::mat4(1.0);
		model3 = glm::translate(model3, lightPos);
		model3 = glm::scale(model3, glm::vec3(0.1));
		lightshader.setMat4("model", model3);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//--------------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 1;

}