#include "PractiseInstancing_4_10.h"


static double deltaTime;
static double lastFrame;
static double lastX = DEFAULT_SCREEN_WIDTH / 2, lastY = DEFAULT_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));

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


int PractiseInstancing_4_10::practise(string projectDir) {

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "draw cube with instancing",
		buffer_window_callback);

	if (window == nullptr) { return -1; }

	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, GL_TRUE, quadVertices, sizeof(quadVertices), 2,
		quadColors, sizeof(quadColors), 3);

	MyShader myshader(projectDir.c_str(), vertFile, fragFile);
	
	//创建100个位移向量，表示10x10网格上的所有位置。
	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2) {
		for (int x = -10; x < 10; x += 2) {
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	myshader.use();
	for (unsigned int i = 0; i < 100; i++) {
		stringstream ss;
		string index;
		ss << i;
		index = ss.str();
		myshader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
	}


	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_move_callback);
	//glfwSetScrollCallback(window, mouse_scroll_callback);

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//---------------render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myshader.use();
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

		//---------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}


/// <summary>
/// 第二个例子
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseInstancing_4_10::practise2(string projectDir) {

	GLFWwindow* window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"draw cube with instancing2",
		buffer_window_callback);

	if (window == nullptr) { return -1; }


	//创建100个位移向量，表示10x10网格上的所有位置。
	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2) {
		for (int x = -10; x < 10; x += 2) {
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, GL_TRUE, quadVertices, sizeof(quadVertices), 2,
		quadColors, sizeof(quadColors), 3);

	MyShader myshader(projectDir.c_str(), vertFile2, fragFile);

	glBindVertexArray(VAO);
	//使用实例化数组来传递偏移量数组数据
	unsigned int instanceVBO;   //单独一个数组缓冲存偏移量
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glVertexAttribDivisor()函数告诉OpenGL该什么时候更新顶点属性的内容至新一组数据。
	//第一个参数是需要的顶点属性
	//第二个参数是属性除数(Attribute Divisor)。
	//*****默认情况下，属性除数是0，告诉OpenGL我们需要在顶点着色器的每次迭代时更新顶点属性。
	//*****将它设置为1时，我们告诉OpenGL我们希望在渲染一个新实例的时候更新顶点属性。
	//*****而设置为2时，我们希望每2个实例更新一次属性，以此类推。
	//*****我们将属性除数设置为1，是在告诉OpenGL，处于位置值2的顶点属性是一个实例化数组。
	glVertexAttribDivisor(2, 1);
	glBindVertexArray(0);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_move_callback);
	//glfwSetScrollCallback(window, mouse_scroll_callback);

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//---------------render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myshader.use();
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);


		//---------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}


/// <summary>
/// 生成 小行星带 上的岩石 的模型坐标glm::mat4 的数组
/// </summary>
/// <param name="modelMatrices"></param>
/// <param name="amount"></param>
/// <param name="radius"></param>
/// <param name="offset"></param>
static void getAsteroidBeltModels(/*out*/glm::mat4 * modelMatrices,
	/*in*/int amount,  /*in*/float radius, 	/*in*/float offset) {
	//int amount = 1000;
	//float radius = 50.0f;
	//float offset = 2.5f;

	srand(glfwGetTime());  //初始化随机种子
	for (unsigned int i = 0; i < amount; i++) {
		//只是将小行星的x和z位置变换到了一个半径为radius的圆形上，并且在半径的基础上偏移了[-offset,offset]
		glm::mat4 model(1.0);

		//1. 位移, 分布在半径为 radius 的原形上, 偏移的范围 [-offset, offset]
		float angle = ((float)i / (float)amount) * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset; //取值在[-2.5,2.5]
		float x = sin(angle) * radius + displacement;  //x轴坐标
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; //让行星带的高度比x和z的宽度小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement; //z轴坐标

		model = glm::translate(model, glm::vec3(x, y, z));

		//2. 缩放, 在 [0.05 , 0.25f] 之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		//3. 旋转, 绕着一个(半)随机选择的旋转轴向量进行随机的旋转
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		//4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}
}

/// <summary>
/// 第三个例子
/// 小行星带 , 使用 Uniform 传递model矩阵, 通过 for循环来渲染多个小行星物体
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseInstancing_4_10::practise3(string projectDir) {
	GLFWwindow* window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"Asteroid Belt", buffer_window_callback);
	if (window == nullptr) { return -1; }

	MyShader myshader(projectDir.c_str(), vertFile3, fragFile3);
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);

	Model modelAsteroid(projectDir.c_str(), imgFileAsteroid);
	Model modelRock(projectDir.c_str(), imgFileRock);

	//Uniform 缓冲
	glUniformBlockBinding(myshader.id, glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	unsigned int amount = 25000;
	float radius = 50.0f;
	//float offset = 2.5f;
	float offset = 5.0f;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	getAsteroidBeltModels(modelMatrices, amount, radius, offset);
	
	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//---------------render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), 
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 180.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		myshader.use();
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f));
		myshader.setMat4("model", model);
		modelAsteroid.draw(&myshader);

		for (int i = 0; i < amount; i++) {
			myshader.setMat4("model", modelMatrices[i]);
			modelRock.draw(&myshader);
		}
		//---------------render done
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}


/// <summary>
/// 小行星带, 第四个例子,
/// 使用 实例化数组来传递 多个模型数据数组, 优化渲染
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseInstancing_4_10::practise4(string projectDir) {

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"render asteroid belt sample4", buffer_window_callback);
	if (window == NULL) { return -1; }

	//------------------------prepare
	MyShader planetShader(projectDir.c_str(), vertFile3, fragFile3);
	planetShader.setInt("texture_diffuse1", 0);

	MyShader rockShader(projectDir.c_str(), vertFile4, fragFile4);
	planetShader.setInt("texture_diffuse1", 0);

	Model modelAstroid(projectDir.c_str(), imgFileAsteroid);

	Model modelRock(projectDir.c_str(), imgFileRock);

	//--- 全部岩石小行星的 模型矩阵数组数据
	glm::mat4* modelMatrices;
	int amount = 50000;
	float radius = 70.0f;
	float offset = 6.0;
	modelMatrices = new glm::mat4[amount];
	getAsteroidBeltModels(modelMatrices, amount, radius, offset);
	//--- done

	//-----------uniform 缓冲
	glUniformBlockBinding(planetShader.id, 
		glGetUniformBlockIndex(planetShader.id, "Matrices"), 0);
	glUniformBlockBinding(rockShader.id, 
		glGetUniformBlockIndex(rockShader.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
	//-----------uniform 缓冲 done


	//-------------顶点缓冲对象
	unsigned int rockModelVertexBuffer;
	glGenBuffers(1, &rockModelVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rockModelVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

	for (int i = 0; i < modelRock.meshes.size(); i++) {
		int vao = modelRock.meshes[i].VAO;
		glBindVertexArray(vao);
		int vec4Size = sizeof(glm::vec4);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)vec4Size);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		//设置缓存, 
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//-------------顶点缓冲对象 done

	//------------------------prepare

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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 150.f);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		planetShader.use();
		glm::mat4 model(1.0f);
		model = glm::translate(model,  glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)curFrame * glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		planetShader.setMat4("model", model);
		modelAstroid.draw(&planetShader);

		rockShader.use();
		//for (int i = 0; i < amount; i++) {
		//	rockShader.setMat4("model", modelMatrices[i]);
		//	modelRock.draw(&rockShader);
		//}
		// 绘制小行星
		for (int i = 0; i < modelRock.meshes.size(); i++) {
			glBindVertexArray(modelRock.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, modelRock.meshes[i].indices.size(),
				GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}