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

//----------------工厂场景------------------------------>>>>>>>>>>>>>>>>>>>>>>>>>>
//定向光的4个数据
//static glm::vec3 dirLightArgs[4] = {
//    glm::vec3(0.05f, 0.05f, 0.1f),  //ambient
//    glm::vec3(0.2f, 0.2f, 0.7f),     //diffuse
//    glm::vec3(0.7f, 0.7f, 0.7f),     //specular
//    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
//};
//
////点光源的6个数据
//static glm::vec3 pointLightColors[] = {
//    glm::vec3(0.2f, 0.2f, 0.6f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
//    glm::vec3(0.3f, 0.3f, 0.7f),  //第二个点光源的 (ambient, diffuse, specular)  红色
//    glm::vec3(0.0f, 0.0f, 0.3f),   //第三个点光源的 (ambient, diffuse, specular)  黄色
//    glm::vec3(0.4f, 0.4f, 0.4f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色
//
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
////聚光的6个数据
//static glm::vec3 spotlightArgs[] = {
//    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
//    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
//    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
//static glm::vec3 clearColor(0.1f, 0.1f, 0.1f);   //背景颜色
//
//static glm::vec2 spotlight_cutoff(10.0f, 12.5f);  //聚光的切角和外切角

//---------------------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<

//----------------化学实验室场景------------------------------------------>>>>>>>>>>>>
//定向光的4个数据
static glm::vec3 dirLightArgs[4] = {
    glm::vec3(0.5f, 0.5f, 0.5f),  //ambient
    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
};

//点光源的6个数据
static glm::vec3 pointLightColors[] = {
    glm::vec3(0.4f, 0.7f, 0.1f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
    glm::vec3(0.4f, 0.7f, 0.1f),  //第二个点光源的 (ambient, diffuse, specular)  红色
    glm::vec3(0.4f, 0.7f, 0.1f),   //第三个点光源的 (ambient, diffuse, specular)  黄色
    glm::vec3(0.4f, 0.7f, 0.1f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色

    glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
};

//聚光的6个数据
static glm::vec3 spotlightArgs[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
    glm::vec3(0.0f, 1.0f, 0.0f),     //diffuse
    glm::vec3(0.0f, 1.0f, 0.0f),     //specular
    glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
};

static glm::vec3 clearColor(0.9f, 0.9f, 0.9f);   //背景颜色

static glm::vec2 spotlight_cutoff(7.0f, 10.0f);  //聚光的切角和外切角

//-----------------------------------------------------------------<<<<<<<<<<<<<<
static unsigned int boxVAO, boxVBO;
static unsigned int floorVAO, floorVBO;
 
static int tex1, tex2;

/// <summary>
/// 准备数据
/// </summary>
/// <param name="practise"></param>
/// <param name="projectDir"></param>
static void prepare(TestPractiseDepth28 * practise, const char* projectDir, MyShader& myshader, MyShader& lightShader, MyShader& outliningShader);

/// <summary>
/// 渲染每一帧
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

	glEnable(GL_DEPTH_TEST);   //  允许深度测试
	glDepthFunc(GL_LESS);      // 允许深度函数

	glEnable(GL_STENCIL_TEST);    //允许模板测试
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);   //在模板测试和深度测试都通过时， 将模板值设置为glStencilFunc函数设置的ref值（第二个参数）
	
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
	//-----------------------------------------  先画光源

	glStencilMask(0x00);     //正常的画光源，地板，墙面， （掩码为0，则写入缓冲的都是0）---------<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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

	//定向光
	myshader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
	myshader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
	myshader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
	myshader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

	//1个点光源
	myshader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);

	myshader.setVec3(("pointlight.ambient"), pointLightColors[3].x * 0.1f, pointLightColors[3].y * 0.1f, pointLightColors[3].z * 0.1f);
	myshader.setVec3(("pointlight.diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	myshader.setVec3(("pointlight.specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);

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

	//--------------------------------------- 画地板
	glBindVertexArray(floorVAO);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glm::mat4 model3(1.0);
	//地板沿y轴向下移动0.001，防止出现防止深度冲突
	model3 = glm::translate(model3, glm::vec3(0.0f, -0.001f, 0.0f));
	myshader.setMat4("model", model3);
	myshader.setMat4("view", view);
	myshader.setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//------------------------地板和立方体使用了不同的纹理图片---------------------------

	glBindVertexArray(boxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);

	//------------------------------------------------利用立方体画三个墙面
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
	// 我们将模板缓冲清除为0，对箱子中所有绘制的片段，将模板值更新为1
	// 通过使用GL_ALWAYS模板测试函数，我们保证了箱子的每个片段都会将模板缓冲的模板值更新为1。
	// 因为片段永远会通过模板测试，在绘制片段的地方，模板缓冲会被更新为参考值。
	glStencilFunc(GL_ALWAYS, 1, 0xff);    //写入模板缓冲的值如果为1，则通过，正常绘制
	glStencilMask(0xff);   //掩码设置为1，则写入模板缓冲的都是本来需要设置的值
	myshader.use();
	//------------------------------------------------ 画第一个立方体
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	myshader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//------------------------------------------------ 画第二个立方体
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	myshader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//-------------------------------------------------------------------------------------------
	//现在模板缓冲在箱子被绘制的地方都更新为1了，我们将要绘制放大的箱子，但这次要禁用模板缓冲的写入
	// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
	//渲染通过:  现在绘制轻微放大版本的立方体, 这次禁用蒙版写入
	// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn,  
	// 因为现在模板缓冲里全是1了. 缓冲里全是1的部分已经绘制了
	// thus only drawing the objects' size differences, making it look like borders.
	// 因此只需要绘制立方体不同大小的部分, 使其看起来像边框
	// -----------------------------------------------------------------------------------------------------------------------------
	glStencilFunc(GL_NOTEQUAL, 1, 0xff);		//屏蔽掉等于1的模板部分， 而放大部分的立方体超出部分为被0填充，则放大部分的才会被画出来，这样就会画出物体的轮廓了    
	glStencilMask(0x00);						// 模板掩码设置为0， 则写入模板掩码中的都是0
	glDisable(GL_DEPTH_TEST);					//禁用深度测试, 因为禁用深度测试了,则glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 走第二个GL_KEEP,保持1的部分不变,只更新新写入的部分
	float scale = 1.05f;						//对物体放大的位数
	outliningShader.use();
	glBindVertexArray(boxVAO);
	//------------------------------------------------ 画第一个立方体的轮廓
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	outliningShader.setMat4("model", model);
	outliningShader.setMat4("view", view);
	outliningShader.setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//------------------------------------------------ 画第二个立方体的轮廓
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	outliningShader.setMat4("model", model);
	outliningShader.setMat4("view", view);
	outliningShader.setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//----------
	glBindVertexArray(0);  //解除对VAO的绑定
	glStencilMask(0xff);   //模板缓冲写入该写入的
	glStencilFunc(GL_ALWAYS, 0, 0xff);  // 统统允许绘制,但是用0来比较
	glEnable(GL_DEPTH_TEST);   //允许深度测试
}

/**
*  为物体创建轮廓的步骤如下：
		1. 在绘制（需要添加轮廓的）物体之前，将模板函数设置为GL_ALWAYS，每当物体的片段被渲染时，将模板缓冲更新为1。
		2. 渲染物体。
		3. 禁用模板写入以及深度测试。
		4. 将每个物体缩放一点点。
		5. 使用一个不同的片段着色器，输出一个单独的（边框）颜色。
		6. 再次绘制物体，但只在它们片段的模板值不等于1时才绘制。
		7. 再次启用模板写入和深度测试。
**/