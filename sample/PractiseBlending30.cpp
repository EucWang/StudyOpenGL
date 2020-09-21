#include "PractiseBlending30.h"

#include <iostream>
#include <map>
using namespace std;


static double deltaTime;
static double lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

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

static float glassVertices[] = {
	//positions		  //normal       	//texture Coords
	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,
	0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f,  1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,

	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,
	-0.5f, -0.5f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f,  0.0f,
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

int PractiseBlending30::practise(const char* projectDir) {

	GLFWwindow * window  = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Practise Blending 30.");
	if (window == NULL) { return -1; }

	MyShader myshader(projectDir, vertFile, fragFile);

	char* modelFilePath;
	if (!getChildPath(&modelFilePath, projectDir, objFileBackpack)) { return -1; }

	Model backpackModel(modelFilePath);

	myshader.use();
	myshader.setInt("texture_diffuse1", 0);

	int texFloor =  textureLoad(projectDir, imageFileFloor);
	int texCube = textureLoad(projectDir, imageFileCube);
	int texRedGlass = textureLoad(projectDir, imageFileTransparentRed);

	//---------------------------
	GLuint floorVAO, floorVBO;

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

	//---------------------------
	GLuint cubeVAO, cubeVBO;
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

	//-----------------------------------------
	GLuint redglassVAO, redglassVBO;

	glGenVertexArrays(1, &redglassVAO);
	glGenBuffers(1, &redglassVBO);
	glBindVertexArray(redglassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, redglassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glassVertices), glassVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//--------------------------- 5个窗户的位置
	const int windowSize = 5;
	glm::vec3 windows[windowSize] = {
		  glm::vec3(1.0f, 0.0f, 1.55f),
		  glm::vec3(1.0f, 0.0f, 2.55f),
		  glm::vec3(1.5f, 0.0f, 2.3f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
	std::map<float, glm::vec3> sorted;  //1个容器,渲染时可以根据观察者视角到窗户的距离进行排序

	//---------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);  //开启混合(透明颜色的混合)
	//我们需要使用源颜色向量的alpha作为源因子，使用1−alpha作为目标因子。
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------
		//render()
		myshader.use();

		glm::mat4 model(1.0);
		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);

		//----------------------------   背包
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

		myshader.setMat4("model", model);
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);

		backpackModel.draw(&myshader); 

		//----------------------------  地面
		glBindVertexArray(floorVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texFloor);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.001f, 0.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//----------------------------  立方体
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texCube);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f,0.0f, 1.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//----------------------- 玻璃
		glBindVertexArray(redglassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texRedGlass);

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.55f));
		//myshader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		////----------------------- 玻璃2 , 透过第二个玻璃看第三个玻璃,会发现第三个玻璃看不到了
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 2.55f));
		//myshader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		////----------------------- 玻璃3
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(1.5f, 0.0f, 2.3f));
		//myshader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//******注意*****
		//对应有透明混合的物体之间, 不要打乱顺序
		//需要最先绘制最远的物体，最后绘制最近的物体。
		//不然, 只能 根据观察者视角,对透明物体进行排序渲染 (这个也不能百分百的解决这个问题)
		//-------------------------------------
		//更高级的技术还有次序无关透明度(Order Independent Transparency, OIT)，但这超出本教程的范围了。
		//现在，你还是必须要普通地混合你的物体，
		//但如果你很小心，并且知道目前方法的限制的话，你仍然能够获得一个比较不错的混合实现。
		sorted.clear(); //清空
		for (size_t i = 0; i < windowSize; i++) {
			float distance = glm::length(camera.Position - windows[i]);  //获得距离
			sorted[distance] = windows[i];  //填入数据
		}
		//以逆序(从远到近)的从map中遍历获取值
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin();
			it != sorted.rend(); ++it) {
			model = glm::mat4(1.0);
			model = glm::translate(model, it->second);
			myshader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			std::cout << "the distance from camera to window is : " << it->first << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}