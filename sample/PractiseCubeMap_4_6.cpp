#include "PractiseCubeMap_4_6.h"


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

int PractiseCubeMap_4_6::practise(const char* projectDir) {
	std::cout << "PractiseCubeMap_4_6.practise() running..." << std::endl;
	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw cube map");
	if (window == NULL) { return -1; }


	//--------- prepare
	//立方体贴图纹理
	unsigned int textCubeMap;
	glGenTextures(1, &textCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textCubeMap);  //绑定立方体纹理贴图
	int width, height, nrChannels;
	unsigned char* data;
	//依顺序
	for (unsigned int i = 0; i < skybox.images.size(); i++) {
		char* imagePath;
		if (!getChildPath(&imagePath, projectDir, skybox.images[i].c_str())) { return -1; }
		
		data = NULL;
		data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
		if (data == NULL) {
			std::cout << "Cubemap texture failed to laod at path : " << std::endl;
			stbi_image_free(data);
			return -1;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
			0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//我们将环绕方式设置为GL_CLAMP_TO_EDGE，
		//这是因为正好处于两个面之间的纹理坐标可能不能击中一个面（由于一些硬件限制），
		//所以通过使用GL_CLAMP_TO_EDGE，OpenGL将在我们对两个面之间采样的时候，永远返回它们的边界值。

		free(imagePath);
	}

	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);  //解绑立方体纹理贴图

	MyShader skyboxShader(projectDir, vertFile, fragFile);
	skyboxShader.use();
	skyboxShader.setInt("cubemap", 0);

	GLuint skyVAO, skyVBO;
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);

	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox.skyboxVertices), skybox.skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//--------- prepare done

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);  //深度缓冲
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);  //透明混淆
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2, 0.2, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------ render
		glDepthMask(GL_FALSE);  //禁止深度写入

		skyboxShader.use();
		glBindVertexArray(skyVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textCubeMap);

		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);

		//我们通过取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分。
		//我们可以将观察矩阵转换为3x3矩阵（移除位移），再将其转换回4x4矩阵，来达到类似的效果。
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		projection = glm::perspective(glm::radians(camera.Zoom),
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthMask(GL_TRUE);  //允许深度写入

		//------ render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &skyVAO);
	glDeleteBuffers(1, &skyVBO);
	skyboxShader.deleteProgram();

	glfwTerminate();
	return 1;
}