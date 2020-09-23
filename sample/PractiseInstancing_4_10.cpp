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
	
	//����100��λ����������ʾ10x10�����ϵ�����λ�á�
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
/// �ڶ�������
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseInstancing_4_10::practise2(string projectDir) {

	GLFWwindow* window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"draw cube with instancing2",
		buffer_window_callback);

	if (window == nullptr) { return -1; }


	//����100��λ����������ʾ10x10�����ϵ�����λ�á�
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
	//ʹ��ʵ��������������ƫ������������
	unsigned int instanceVBO;   //����һ�����黺���ƫ����
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glVertexAttribDivisor()��������OpenGL��ʲôʱ����¶������Ե���������һ�����ݡ�
	//��һ����������Ҫ�Ķ�������
	//�ڶ������������Գ���(Attribute Divisor)��
	//*****Ĭ������£����Գ�����0������OpenGL������Ҫ�ڶ�����ɫ����ÿ�ε���ʱ���¶������ԡ�
	//*****��������Ϊ1ʱ�����Ǹ���OpenGL����ϣ������Ⱦһ����ʵ����ʱ����¶������ԡ�
	//*****������Ϊ2ʱ������ϣ��ÿ2��ʵ������һ�����ԣ��Դ����ơ�
	//*****���ǽ����Գ�������Ϊ1�����ڸ���OpenGL������λ��ֵ2�Ķ���������һ��ʵ�������顣
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
/// ���� С���Ǵ� �ϵ���ʯ ��ģ������glm::mat4 ������
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

	srand(glfwGetTime());  //��ʼ���������
	for (unsigned int i = 0; i < amount; i++) {
		//ֻ�ǽ�С���ǵ�x��zλ�ñ任����һ���뾶Ϊradius��Բ���ϣ������ڰ뾶�Ļ�����ƫ����[-offset,offset]
		glm::mat4 model(1.0);

		//1. λ��, �ֲ��ڰ뾶Ϊ radius ��ԭ����, ƫ�Ƶķ�Χ [-offset, offset]
		float angle = ((float)i / (float)amount) * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset; //ȡֵ��[-2.5,2.5]
		float x = sin(angle) * radius + displacement;  //x������
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; //�����Ǵ��ĸ߶ȱ�x��z�Ŀ��С
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement; //z������

		model = glm::translate(model, glm::vec3(x, y, z));

		//2. ����, �� [0.05 , 0.25f] ֮������
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		//3. ��ת, ����һ��(��)���ѡ�����ת�����������������ת
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		//4. ��ӵ������������
		modelMatrices[i] = model;
	}
}

/// <summary>
/// ����������
/// С���Ǵ� , ʹ�� Uniform ����model����, ͨ�� forѭ������Ⱦ���С��������
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

	//Uniform ����
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
/// С���Ǵ�, ���ĸ�����,
/// ʹ�� ʵ�������������� ���ģ����������, �Ż���Ⱦ
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

	//--- ȫ����ʯС���ǵ� ģ�;�����������
	glm::mat4* modelMatrices;
	int amount = 50000;
	float radius = 70.0f;
	float offset = 6.0;
	modelMatrices = new glm::mat4[amount];
	getAsteroidBeltModels(modelMatrices, amount, radius, offset);
	//--- done

	//-----------uniform ����
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
	//-----------uniform ���� done


	//-------------���㻺�����
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

		//���û���, 
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//-------------���㻺����� done

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
		// ����С����
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