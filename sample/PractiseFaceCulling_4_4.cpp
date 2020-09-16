#include "PractiseFaceCulling_4_4.h"

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

int PractiseFaceCulling_4_4::practise(const char* projectDir) {

	GLFWwindow  * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "practise to draw with face culling.");
	if (window == NULL) { return -1; }

	MyShader cubeshader(projectDir, vertFile, fragFile);
	MyShader grassshader(projectDir, grassVertFile, grassFragFile);
	GLuint cubeVAO=0;
	makeVAOVBO(&cubeVAO, cubeVertices, sizeof(cubeVertices), 8);

	GLuint planeVAO;
	makeVAOVBO(&planeVAO, planeVertices, sizeof(planeVertices), 8);

	GLuint grassVAO;
	makeVAOVBO(&grassVAO, grassVertices, sizeof(grassVertices), 5);

	cubeshader.use();
 	int tex1 = textureLoad(projectDir, imgFileCube);
	int tex2 = textureLoad(projectDir, imgFilePlane);
	int tex3 = textureLoad(projectDir, imgFileGrass);

	cubeshader.setInt("texture_diffuse1", 0);
	grassshader.setInt("texture_diffuse1", 0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-------------------------render
		cubeshader.use();

		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		glm::mat4 model(1.0);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);
		cubeshader.setMat4("view", view);
		cubeshader.setMat4("projection", projection);


		glEnable(GL_CULL_FACE); //开启面剔除
		glCullFace(GL_FRONT);  //GL_BACK：只剔除背向面。GL_FRONT：只剔除正向面。GL_FRONT_AND_BACK：剔除正向面和背向面。
		glFrontFace(GL_CW);  //默认值是GL_CCW，它代表的是逆时针的环绕顺序，另一个选项是GL_CW，它（显然）代表的是顺时针顺序。
		//-------------  立方体
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)curFrame * glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cubeshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisable(GL_CULL_FACE);  //关闭面剔除
		//-------------  地面
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, tex2);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.003f, 0.0f));
		cubeshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//----------------- 草
		grassshader.use();
		glBindVertexArray(grassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex3);
		grassshader.setMat4("view", view);
		grassshader.setMat4("projection", projection);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		grassshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -0.16f, 1.13f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		grassshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -0.16f, 0.87f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		grassshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -0.36f, 0.82f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		grassshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//-------------------------render

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}

