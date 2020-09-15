#include "PractiseBlending29.h"


static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static glm::vec3 lightPos(4.3f, 2.0f, -4.5f);

static Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));

static float cubeVertices[] = {
	// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

static float planeVertices[] = {
	// (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
	//this will cause the floor texture to repeat)
	// positions          // texture Coords
	 5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 2.0f
};

static float grassVertices[] = {
	//positions			//texture Coords
	-0.5f, 0.5f, 0.0f,   0.0f,  1.0f,
	0.5f, 0.5f, 0.0f,    1.0f,  1.0f,
	0.5f, -0.5f, 0.0f,   1.0f,  0.0f,

	-0.5f, 0.5f, 0.0f,   0.0f,  1.0f,
	0.5f, -0.5f, 0.0f,   1.0f,  0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f,  0.0f,
};

static vector<glm::vec3> grassPosition = {
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

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

//----------------��ѧʵ���ҳ���------------------------------------------>>>>>>>>>>>>
//������4������
static glm::vec3 dirLightArgs[4] = {
	glm::vec3(0.5f, 0.5f, 0.5f),  //ambient
	glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
	glm::vec3(1.0f, 1.0f, 1.0f),     //specular
	glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
};

//���Դ��6������
static glm::vec3 pointLightColors[] = {
	glm::vec3(0.4f, 0.7f, 0.1f),  //��һ�����Դ�� (ambient, diffuse, specular)  ����ɫ
	glm::vec3(0.4f, 0.7f, 0.1f),  //�ڶ������Դ�� (ambient, diffuse, specular)  ��ɫ
	glm::vec3(0.4f, 0.7f, 0.1f),   //���������Դ�� (ambient, diffuse, specular)  ��ɫ
	glm::vec3(0.4f, 0.7f, 0.1f),  //���ĸ����Դ�� (ambient, diffuse, specular)  ��ɫ

	glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
};

//�۹��6������
static glm::vec3 spotlightArgs[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
	glm::vec3(0.0f, 1.0f, 0.0f),     //diffuse
	glm::vec3(0.0f, 1.0f, 0.0f),     //specular
	glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
};

static glm::vec3 clearColor(0.9f, 0.9f, 0.9f);   //������ɫ

static glm::vec2 spotlight_cutoff(7.0f, 10.0f);  //�۹���нǺ����н�

//-----------------------------------------------------------------<<<<<<<<<<<<<<
static unsigned int cubeVAO, cubeVBO;
static unsigned int floorVAO, floorVBO;
static unsigned int grassVAO, grassVBO;

//static int tex1, tex2, tex3;

int PractiseBlending29::practiseDepth29(const char* projectDir) {
	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT,
		"draw scene with blending transiparent grass and glass.");
	if (window == NULL) { return -1; }


	// prepare ------------------------------------------------------------------------------------
	MyShader myshader(projectDir, vertFile, fragFile);
	MyShader lightShader(projectDir, vertFile, fragFileLight);
	MyShader outliningShader(projectDir, vertFile, fragFileOutlining);
	MyShader grassShader(projectDir, vertFileGrass, fragFileGrass);

	//----------------------------------------
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	grassShader.use();
	/**
		ע�⣬����������ı�Ե��ʱ��OpenGL��Ա�Ե��ֵ��������һ���ظ���ֵ���в�ֵ����Ϊ���ǽ����Ļ��Ʒ�ʽ����Ϊ��GL_REPEAT��
		��ͨ����û����ģ�������������ʹ����͸��ֵ������ͼ��Ķ���������ײ���Ե�Ĵ�ɫֵ���в�ֵ�������Ľ����һ����͸������ɫ�߿�
		����ܻῴ������������������ı��Ρ�
		Ҫ����������ÿ����alpha�����ʱ���뽫����Ļ��Ʒ�ʽ����ΪGL_CLAMP_TO_EDGE��
	*/
	int tex3Grass = textureLoad2(projectDir, img3, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, 
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	grassShader.setInt("texture1", 0);

	//---------------------------
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//-----------------------------
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	int tex1Cube = textureLoad(projectDir, img1);
	int tex2Floor = textureLoad(projectDir, img2);
	//int tex3Grass = textureLoad(projectDir, img3);

	myshader.use();
	myshader.setInt("texture1", 0);


	//---------------------------------------------------------------------------------------------

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilMask(0x00);  //�������������ģ�建��
		//render---------------------------------------------------------------------------------

		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		glm::mat4 model(1.0);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), 
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

		//��
		grassShader.use();
		glBindVertexArray(grassVAO);
		glBindTexture(GL_TEXTURE_2D, tex3Grass);
		glActiveTexture(GL_TEXTURE0);

		grassShader.setMat4("view", view);
		grassShader.setMat4("projection", projection);
		for (size_t j = 0; j < grassPosition.size(); j++) {
			for (size_t i = 0; i < 8; i++)		{  //ͬһ��λ�û�8���ݣ��Ե���ʽ��
				model = glm::mat4(1.0);
				//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 2.0f));
				model = glm::translate(model, grassPosition[j]);
				model = glm::rotate(model, glm::radians(i * 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				grassShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		//��Դ
		lightShader.use();
		glBindVertexArray(cubeVAO);
		
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);

		model = glm::mat4(1.0);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1));
		lightShader.setMat4("model", model);
		lightShader.setVec3("lightColor", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		myshader.use();

		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);

		//�����
		myshader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
		myshader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
		myshader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
		myshader.setVec3(("dirlight.direction"), -0.2f, -1.0f, -0.3f);
		myshader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

		//1�����Դ
		myshader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);
		myshader.setVec3(("pointlight.ambient"), pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		myshader.setVec3(("pointlight.diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		myshader.setVec3(("pointlight.specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);

		myshader.setFloat(("pointlight.constant"), pointLightColors[4].x);
		myshader.setFloat(("pointlight.linear"), pointLightColors[4].y);
		myshader.setFloat(("pointlight.quadratic"), pointLightColors[4].z);

		//1���۹�
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

		//viewPos
		myshader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);


		//----------------------------------
		//�ذ�
		glBindVertexArray(floorVAO);
		glBindTexture(GL_TEXTURE_2D, tex2Floor);
		glActiveTexture(GL_TEXTURE0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.001f, 0.0f));
		myshader.setMat4("model", model);
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//  ������ ----------------
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, tex1Cube);
		glActiveTexture(GL_TEXTURE0);
		//------------------------------------------------���������廭����ǽ��
		for (size_t j = 0; j < 5; j++) {
			for (size_t i = 0; i < 10; i++) {
				glm::mat4 model2(1.0f);
				model2 = glm::translate(model2, glm::vec3(4.9f, j + 0.0f, i - 4.5f));
				model2 = glm::scale(model2, glm::vec3(0.2f, 1.0f, 1.0f));
				myshader.setMat4("model", model2);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		for (size_t j = 0; j < 5; j++) {
			for (size_t i = 0; i < 10; i++) {
				glm::mat4 model2(1.0f);
				model2 = glm::translate(model2, glm::vec3(-4.9f, j + 0.0f, i - 4.5f));
				model2 = glm::scale(model2, glm::vec3(0.2f, 1.0f, 1.0f));
				myshader.setMat4("model", model2);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		for (size_t j = 0; j < 5; j++) {
			for (size_t i = 0; i < 10; i++) {
				glm::mat4 model2(1.0f);
				model2 = glm::translate(model2, glm::vec3(i - 4.49f, j + 0.0f, -4.91f));
				model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 0.2f));
				myshader.setMat4("model", model2);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		//---------------ֻ����Ҫ����������������ʹ��ģ�建��--------
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilMask(0xff);

		//����
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//����2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//����3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//-----------------------ʹ�ò�ͬ��Shader��������
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outliningShader.use();
		glBindVertexArray(cubeVAO);

		outliningShader.setMat4("view", view);
		outliningShader.setMat4("projection", projection);

		glm::vec3 scaler(1.01f, 1.01f, 1.01f);  //�Ŵ�һСȦ
		//����1����
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, scaler);

		outliningShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//����2����
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
		model = glm::scale(model, scaler);
		outliningShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//����3����
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, scaler);
		outliningShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilMask(0xff);
		glEnable(GL_DEPTH_TEST);

		//render-------------------------------------------------------------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}