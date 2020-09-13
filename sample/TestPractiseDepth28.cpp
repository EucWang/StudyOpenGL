#include "TestPractiseDepth28.h"

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

//----------------��������------------------------------>>>>>>>>>>>>>>>>>>>>>>>>>>
//������4������
//static glm::vec3 dirLightArgs[4] = {
//    glm::vec3(0.05f, 0.05f, 0.1f),  //ambient
//    glm::vec3(0.2f, 0.2f, 0.7f),     //diffuse
//    glm::vec3(0.7f, 0.7f, 0.7f),     //specular
//    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
//};
//
////���Դ��6������
//static glm::vec3 pointLightColors[] = {
//    glm::vec3(0.2f, 0.2f, 0.6f),  //��һ�����Դ�� (ambient, diffuse, specular)  ����ɫ
//    glm::vec3(0.3f, 0.3f, 0.7f),  //�ڶ������Դ�� (ambient, diffuse, specular)  ��ɫ
//    glm::vec3(0.0f, 0.0f, 0.3f),   //���������Դ�� (ambient, diffuse, specular)  ��ɫ
//    glm::vec3(0.4f, 0.4f, 0.4f),  //���ĸ����Դ�� (ambient, diffuse, specular)  ��ɫ
//
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
////�۹��6������
//static glm::vec3 spotlightArgs[] = {
//    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
//    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
//    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
//static glm::vec3 clearColor(0.1f, 0.1f, 0.1f);   //������ɫ
//
//static glm::vec2 spotlight_cutoff(10.0f, 12.5f);  //�۹���нǺ����н�

//---------------------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<

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
static unsigned int boxVAO, boxVBO;
static unsigned int floorVAO, floorVBO;
 
static int tex1, tex2;

/// <summary>
/// ׼������
/// </summary>
/// <param name="practise"></param>
/// <param name="projectDir"></param>
static void prepare(TestPractiseDepth28 * practise, const char* projectDir, MyShader& myshader, MyShader& lightShader, MyShader& outliningShader);

/// <summary>
/// ��Ⱦÿһ֡
/// </summary>
/// <param name="practise"></param>
/// <param name="curFrame"></param>
static void render(TestPractiseDepth28* practise, double curFrame, MyShader& myshader, MyShader& lightShader, MyShader& outliningShader);

int TestPractiseDepth28::practiseDepth28(const char * projectDir) {
	
	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "draw scene with more depth params");
	if (window == NULL) { return -1; }

	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);   //  ������Ȳ���
	glDepthFunc(GL_LESS);      // ������Ⱥ���

	glEnable(GL_STENCIL_TEST);    //����ģ�����
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);   //��ģ����Ժ���Ȳ��Զ�ͨ��ʱ�� ��ģ��ֵ����ΪglStencilFunc�������õ�refֵ���ڶ���������
	
	MyShader myshader(projectDir, vertFile, fragFile);
	MyShader lightShader(projectDir, vertFile, fragFileLight);
	MyShader outliningShader(projectDir, vertFile, fragFileOutlining);

	prepare(this, projectDir, myshader, lightShader, outliningShader);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		render(this, curFrame, myshader, lightShader, outliningShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}


void prepare(TestPractiseDepth28 * practise, const char* projectDir, MyShader& myshader, MyShader& lightShader, MyShader& outliningShader) {

	//------------------------------------------------------

	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);

	//------------------------------------------------

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

	glBindVertexArray(0);

	myshader.use();
	tex1 = textureLoad2(projectDir, practise->img1, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	tex2 = textureLoad(projectDir, practise->img2);
	// shader configuration
	// --------------------
	myshader.setInt("texture1", 0);
	lightShader.use();
	lightShader.setInt("texture1", 0);

	outliningShader.use();
	outliningShader.setInt("texture1", 0);

	///--------------
}


void render(TestPractiseDepth28* practise , double curFrame, MyShader& myshader, MyShader& lightShader, MyShader& outliningShader) {

	//-----------------------------------------
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);

	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.Zoom),
		SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);
	//-----------------------------------------  �Ȼ���Դ

	glStencilMask(0x00);     //�����Ļ���Դ���ذ壬ǽ�棬 ������Ϊ0����д�뻺��Ķ���0��---------<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	lightShader.use();
	glBindVertexArray(boxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);

	lightShader.setMat4("view", view);
	lightShader.setMat4("projection", projection);

	glm::mat4 model4(1.0f);
	model4 = glm::translate(model4, lightPos);
	model4 = glm::rotate(model4, (float)(curFrame)*glm::radians(10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model4 = glm::scale(model4, glm::vec3(0.1f, 0.1f, 0.1f));
	lightShader.setMat4("model", model4);

	lightShader.setVec3("lightColor", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	//------------------------------------------------
	myshader.use();

	//------------
	myshader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

	//�����
	myshader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
	myshader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
	myshader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
	myshader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

	//1�����Դ
	myshader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);

	myshader.setVec3(("pointlight.ambient"), pointLightColors[3].x * 0.1f, pointLightColors[3].y * 0.1f, pointLightColors[3].z * 0.1f);
	myshader.setVec3(("pointlight.diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	myshader.setVec3(("pointlight.specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);

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

	//--------------------------------------- ���ذ�
	glBindVertexArray(floorVAO);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glm::mat4 model3(1.0);
	//�ذ���y�������ƶ�0.001����ֹ���ַ�ֹ��ȳ�ͻ
	model3 = glm::translate(model3, glm::vec3(0.0f, -0.001f, 0.0f));
	myshader.setMat4("model", model3);
	myshader.setMat4("view", view);
	myshader.setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//------------------------�ذ��������ʹ���˲�ͬ������ͼƬ---------------------------

	glBindVertexArray(boxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);

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

	//-------------------------------------------------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ���ǽ�ģ�建�����Ϊ0�������������л��Ƶ�Ƭ�Σ���ģ��ֵ����Ϊ1
	// ͨ��ʹ��GL_ALWAYSģ����Ժ��������Ǳ�֤�����ӵ�ÿ��Ƭ�ζ��Ὣģ�建���ģ��ֵ����Ϊ1��
	// ��ΪƬ����Զ��ͨ��ģ����ԣ��ڻ���Ƭ�εĵط���ģ�建��ᱻ����Ϊ�ο�ֵ��
	glStencilFunc(GL_ALWAYS, 1, 0xff);    //д��ģ�建���ֵ���Ϊ1����ͨ������������
	glStencilMask(0xff);   //��������Ϊ1����д��ģ�建��Ķ��Ǳ�����Ҫ���õ�ֵ
	myshader.use();
	//------------------------------------------------ ����һ��������
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	myshader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//------------------------------------------------ ���ڶ���������
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	myshader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//-------------------------------------------------------------------------------------------
	//����ģ�建�������ӱ����Ƶĵط�������Ϊ1�ˣ����ǽ�Ҫ���ƷŴ�����ӣ������Ҫ����ģ�建���д��
	// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
	//��Ⱦͨ��:  ���ڻ�����΢�Ŵ�汾��������, ��ν����ɰ�д��
	// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn,  
	// ��Ϊ����ģ�建����ȫ��1��. ������ȫ��1�Ĳ����Ѿ�������
	// thus only drawing the objects' size differences, making it look like borders.
	// ���ֻ��Ҫ���������岻ͬ��С�Ĳ���, ʹ�俴������߿�
	// -----------------------------------------------------------------------------------------------------------------------------
	glStencilFunc(GL_NOTEQUAL, 1, 0xff);		//���ε�����1��ģ�岿�֣� ���Ŵ󲿷ֵ������峬������Ϊ��0��䣬��Ŵ󲿷ֵĲŻᱻ�������������ͻử�������������    
	glStencilMask(0x00);						// ģ����������Ϊ0�� ��д��ģ�������еĶ���0
	glDisable(GL_DEPTH_TEST);					//������Ȳ���, ��Ϊ������Ȳ�����,��glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); �ߵڶ���GL_KEEP,����1�Ĳ��ֲ���,ֻ������д��Ĳ���
	float scale = 1.05f;						//������Ŵ��λ��
	outliningShader.use();
	glBindVertexArray(boxVAO);
	//------------------------------------------------ ����һ�������������
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	outliningShader.setMat4("model", model);
	outliningShader.setMat4("view", view);
	outliningShader.setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//------------------------------------------------ ���ڶ��������������
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	outliningShader.setMat4("model", model);
	outliningShader.setMat4("view", view);
	outliningShader.setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//----------
	glBindVertexArray(0);  //�����VAO�İ�
	glStencilMask(0xff);   //ģ�建��д���д���
	glStencilFunc(GL_ALWAYS, 0, 0xff);  // ͳͳ�������,������0���Ƚ�
	glEnable(GL_DEPTH_TEST);   //������Ȳ���
}

/**
*  Ϊ���崴�������Ĳ������£�
		1. �ڻ��ƣ���Ҫ��������ģ�����֮ǰ����ģ�庯������ΪGL_ALWAYS��ÿ�������Ƭ�α���Ⱦʱ����ģ�建�����Ϊ1��
		2. ��Ⱦ���塣
		3. ����ģ��д���Լ���Ȳ��ԡ�
		4. ��ÿ����������һ��㡣
		5. ʹ��һ����ͬ��Ƭ����ɫ�������һ�������ģ��߿���ɫ��
		6. �ٴλ������壬��ֻ������Ƭ�ε�ģ��ֵ������1ʱ�Ż��ơ�
		7. �ٴ�����ģ��д�����Ȳ��ԡ�
**/