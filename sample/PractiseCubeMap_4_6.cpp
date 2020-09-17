#include "PractiseCubeMap_4_6.h"

static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static float cubeVertices[] = {
	// Back face
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // Bottom-left
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f, // bottom-right         
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f, // top-left
	// Front face
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top-right
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top-right
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f, // top-left
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom-left
	// Left face
	-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		 1.0f, 0.0f, // top-right
	-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-right
	// Right face
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-left
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // top-right         
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-left
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f, // top-left
	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, // top-right
	// Top face
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // top-left
	 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f, // top-right     
	 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // bottom-right
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // top-left
	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f  // bottom-left        
};

static float quadVertices[] = {
	// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
};

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

static int texureLoadCubmap(const char* projectDir, vector<string> images) {

	unsigned int textCubeMap;
	glGenTextures(1, &textCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textCubeMap);  //绑定立方体纹理贴图
	int width, height, nrChannels;
	unsigned char* data;
	//依顺序
	for (unsigned int i = 0; i < images.size(); i++) {
		char* imagePath;
		if (!getChildPath(&imagePath, projectDir, images[i].c_str())) { return -1; }

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
	return textCubeMap;
}

int PractiseCubeMap_4_6::practise(const char* projectDir) {
	std::cout << "PractiseCubeMap_4_6.practise() running..." << std::endl;
	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw cube map");
	if (window == NULL) { return -1; }

	//--------- prepare
	//立方体贴图纹理
	MyShader skyboxShader(projectDir, vertFile, fragFile);
	skyboxShader.use();
	int textCubeMap = 0;
	if ((textCubeMap = texureLoadCubmap(projectDir, skybox.images)) <= 0) { return -1; }
	skyboxShader.setInt("cubemap", 0);

	GLuint skyVAO, skyVBO;
	makeVAOVBO(&skyVAO, &skyVBO, skybox.skyboxVertices, sizeof(skybox.skyboxVertices), 3);

	//箱子
	MyShader boxShader(projectDir, vertFileCube, fragFileCube);
	boxShader.use();
	int texContainer = textureLoad(projectDir, imageFileContainer);
	boxShader.setInt("texture_diffuse1", 0);

	GLuint boxVAO, boxVBO;
	makeVAOVBO(&boxVAO, &boxVBO, cubeVertices, sizeof(cubeVertices), 8);

	// 帧缓冲
	GLuint framebufferobj;
	glGenFramebuffers(1, &framebufferobj);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferobj);

	GLuint framebufferTex;
	glGenTextures(1, &framebufferTex);
	glBindTexture(GL_TEXTURE_2D, framebufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTex, 0);
	std::cout << "attach texture to framebuffer." << std::endl;

	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	std::cout << "attach renderbuffer to framebuffer." << std::endl;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: framebuffer is not complete." << std::endl;
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);  //解除帧缓冲的绑定

	MyShader screenShader(projectDir, vertFileScreen, fragFileScreen);
	screenShader.use();
	screenShader.setInt("screen_texture", 0);

	GLuint quadVAO, quadVBO;
	makeVAOVBO(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 4);

	//--------- prepare done

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);  //深度缓冲
	//glDepthFunc(GL_LESS);
	glDepthFunc(GL_LEQUAL);  //既需要保证天空盒子在值小于等于深度缓冲而不是小于时才通过深度测试
	glDepthMask(GL_TRUE);  //允许深度写入

	//glEnable(GL_BLEND);  //透明混淆
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//glBindFramebuffer(GL_FRAMEBUFFER, framebufferobj);
		glEnable(GL_DEPTH_TEST);	
		glDepthFunc(GL_LEQUAL);  //既需要保证天空盒子在值小于等于深度缓冲而不是小于时才通过深度测试
		glDepthMask(GL_TRUE);  //允许深度写入

		glClearColor(0.2, 0.2, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------ render

		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		projection = glm::perspective(glm::radians(camera.Zoom),
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

		//------- box
		glEnable(GL_CULL_FACE);
		boxShader.use();
		glBindVertexArray(boxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainer);

		view = glm::mat4(1.0);
		view = camera.GetViewMatrix();
		boxShader.setMat4("view", view);
		boxShader.setMat4("projection", projection);
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		boxShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDisable(GL_CULL_FACE);
		//------- box done
		
		//------- skybox 
		skyboxShader.use();
		glBindVertexArray(skyVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textCubeMap);
		
		//我们通过取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分。
		//我们可以将观察矩阵转换为3x3矩阵（移除位移），再将其转换回4x4矩阵，来达到类似的效果。
		view = glm::mat4(1.0);
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		//------- skybox done

		//TODO  there still has bug.
		//render framebuffer texture to main screen
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//screenShader.use();
		//glBindVertexArray(quadVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, framebufferTex);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);

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