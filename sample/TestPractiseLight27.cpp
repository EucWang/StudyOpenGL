#include "../include/TestPractiseLight27.h"

static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));

static glm::vec3 lightPos(0.0f, 10.0f, 3.0f);

static const float lightVertices[] = {
	// positions    
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};


//----------------沙漠场景------------------------------
//定向光的4个数据
static glm::vec3 dirLightArgs[4] = {
    glm::vec3(0.3f, 0.24f, 0.14f),  //ambient
    glm::vec3(0.7f, 0.42f, 0.26f),     //diffuse
    glm::vec3(0.5f, 0.5f, 0.5f),     //specular
    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
};

//点光源的6个数据
static glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.6f, 0.0f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
    glm::vec3(1.0f, 0.0f, 0.0f),  //第二个点光源的 (ambient, diffuse, specular)  红色
    glm::vec3(1.0f, 1.0, 0.0),    //第三个点光源的 (ambient, diffuse, specular)  黄色
    glm::vec3(0.2f, 0.2f, 1.0f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色

    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
};

//聚光的6个数据
static glm::vec3 spotlightArgs[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
    glm::vec3(0.8f, 0.8f, 0.8f),     //diffuse
    glm::vec3(0.8f, 0.8f, 0.8f),     //specular
    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
};

static glm::vec3 clearColor(0.75f, 0.52f, 0.3f);   //背景颜色

static glm::vec2 spotlight_cutoff(12.5f, 13.0f);  //聚光的切角和外切角

//--------------------------------------------------------------------------//


//----------------工厂场景------------------------------>>>>>>>>>>>>>>>>>>>>>>>>>>
////定向光的4个数据
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


int TestPractiseLight27::practiseLight27(const char * projectDir) {

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "draw model practise");
	if (window == NULL)	{		
		glfwTerminate();		
		return -1;	
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glEnable(GL_DEPTH_TEST);

	MyShader ourShader(projectDir, vertextFile, fragFile);

	MyShader lightShader(projectDir, lightVertFile, lightFragFile);

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	char* modelObjPath;
	if (!getChildPath(&modelObjPath, projectDir, modelFile)) { return -1; }
	std::cout << "modelObjPath : " << modelObjPath << std::endl;
	Model ourModel(modelObjPath);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
	
		processInput(window);

		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ourShader.use();

		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);

		projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SMALL_SCREEN_WIDTH / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		///////////

		//定向光
		ourShader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
		ourShader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
		ourShader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
		ourShader.setVec3(("dirlight.direction"), -0.2f, -1.0f, -0.3f);
		ourShader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);
		
		//1个点光源
		ourShader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);
		ourShader.setVec3(("pointlight.ambient"),  pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		ourShader.setVec3(("pointlight.diffuse"),  pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		ourShader.setVec3(("pointlight.specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);

		ourShader.setFloat(("pointlight.constant"), pointLightColors[4].x);
		ourShader.setFloat(("pointlight.linear"), pointLightColors[4].y);
		ourShader.setFloat(("pointlight.quadratic"), pointLightColors[4].z);

		//1个聚光
		ourShader.setVec3(("spotlight.ambient"), spotlightArgs[0].x, spotlightArgs[0].y, spotlightArgs[0].z);
		ourShader.setVec3(("spotlight.diffuse"), spotlightArgs[1].x, spotlightArgs[1].y, spotlightArgs[1].z);
		ourShader.setVec3(("spotlight.specular"), spotlightArgs[2].x, spotlightArgs[2].y, spotlightArgs[2].z);
		ourShader.setVec3(("spotlight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
		ourShader.setVec3(("spotlight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);

		ourShader.setFloat(("spotlight.cutoff"), glm::cos(glm::radians(spotlight_cutoff.x)));
		ourShader.setFloat(("spotlight.outCutoff"), glm::cos(glm::radians(spotlight_cutoff.y)));
		ourShader.setFloat(("spotlight.constant"), spotlightArgs[3].x);
		ourShader.setFloat(("spotlight.linear"), spotlightArgs[3].y);
		ourShader.setFloat(("spotlight.quadratic"), spotlightArgs[3].z);


		//viewPos

		ourModel.draw(&ourShader);

		//-------------------------------------------------------------------------------
		lightShader.use();
		glBindVertexArray(VAO);

		glm::mat4 model1(1.0f);
		model1 = glm::translate(model1, lightPos);
		model1 = glm::scale(model1, glm::vec3(0.1f, 0.1f, 0.1f));
		lightShader.setMat4("model", model1);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		lightShader.setVec3("ourColor", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}
