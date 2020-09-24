#include "PractiseAntiAliasing_4_11.h"


static double deltaTime;
static double lastFrame;
static double lastX = DEFAULT_SCREEN_WIDTH / 2, lastY = DEFAULT_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));

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

int PractiseAntiAliasing_4_11::practise0(string projectDir) {
	//TODO
}


/// <summary>
/// 通过草地来查看抗锯齿效果
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseAntiAliasing_4_11::practise(string projectDir) {
	glfwWindowHint(GLFW_SAMPLES, 4);  //提示(Hint) GLFW，我们希望使用一个包含N个样本的多重采样缓冲
	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"anti aliasing", buffer_window_callback);
	if (window == NULL) { return -1; }

	std::cout << "prepare data." << std::endl;
	//-------------prepare data
	MyShader myshader(projectDir.c_str(), vertFile, fragFile);
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	int texPlane = RenderUtil::textureLoad2D(projectDir, imgFilePlane);

	MyShader shaderGrass(projectDir.c_str(), vertFileGrass, fragFileGrass);
	shaderGrass.use();
	shaderGrass.setInt("texture_diffuse1", 0);

	GLuint grassVAO, grassVBO;
	RenderUtil::makeVertexArrayAndBuffer(&grassVAO, &grassVBO, grassVertices, sizeof(grassVertices), 5);

	int texGrass = RenderUtil::textureLoad2D(projectDir, imgFileGrass, 
		GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	//---prepare uniform buffer
	glUniformBlockBinding(myshader.id,
		glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	glUniformBlockBinding(shaderGrass.id,
		glGetUniformBlockIndex(shaderGrass.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
	//---prepare uniform buffer done 

	srand(glfwGetTime());
	glm::mat4* modelMatrices;
	float offset = 4.5f;
	int amount = 5120;
	modelMatrices = new glm::mat4[amount];
	for (int i = 0; i < amount; i++) {
		//随机位移位置
		float x = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//随机沿着y轴旋转角度
		float rot = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//在 [0.5 , 2.5f] 之间随机缩放系数
		float scaleX = (rand() % 200) / 100.0f + 0.5;
		float scaleY = (rand() % 200) / 100.0f + 0.5;
		glm::mat4 model(1.0f);
		//因为缩放了,所以草不一定在地上,在y轴上也有一个位移量
		model = glm::translate(model, glm::vec3(x, 0.5f* (scaleY - 1), z));
		model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0));////2. 缩放, 
		model = glm::rotate(model, rot * glm::radians(360.0f), glm::vec3(0.0f, 10.0f, 0.0f)); //有轴旋转
		modelMatrices[i] = model;
	}

	//--- prepare instancing array
	int sizeVec4 = sizeof(glm::vec4);
	GLuint instancingVBO;
	glBindVertexArray(grassVAO);
	glGenBuffers(1, &instancingVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancingVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, )
	glBufferData(GL_ARRAY_BUFFER, amount * 4 * sizeVec4, &modelMatrices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(sizeVec4));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(2 *sizeVec4));
	
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(3 *sizeVec4));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//--- prepare instancing array done

	//-------------prepare data done
	std::cout << "prepare data done." << std::endl;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);  //开启深度缓冲
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);  //开启透明颜色混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE); //开启多重采样缓冲,抗锯齿

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//-------------render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		myshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texPlane);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		shaderGrass.use();
		glBindVertexArray(grassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texGrass);
		//for (int i = 0; i < amount; i++) {
		//	shaderGrass.setMat4("model", modelMatrices[i]);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, amount);

		//-------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &grassVBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &grassVAO);
	myshader.deleteProgram();
	shaderGrass.deleteProgram();

	return 1;
}