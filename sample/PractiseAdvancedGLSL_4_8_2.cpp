#include "PractiseAdvancedGLSL_4_8_2.h"


static double deltaTime;
static float lastFrame;
static double lastX = DEFAULT_SCREEN_WIDTH / 2, lastY = DEFAULT_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

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

static void buffer_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


/**
* Ƭ����ɫ�� ��
* ʹ�� gl_FragCoord.x/.y ����Ƭ���ڴ����е�λ����Ӧ�ò�ͬ����ɫ
**/
int PractiseAdvancedGLSL_4_8_2::practise(string projectDir) {

	GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Advanced GLSL gl_FragCoord",
		buffer_window_callback);
	if (window == NULL) { return -1; }

	//---------prepare
	MyShader myshader(projectDir.c_str(), vertFile, fragFile);
	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, cubePosition, sizeof(cubePosition), 3);

	//---------prepare done

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		//-------------render
		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT,
			0.1f, 100.0f);

		myshader.use();
		glBindVertexArray(VAO);

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));


		//����Ƭ����ɫ���е� gl_FragCoord.x/.y ��ָ����ʾ��ͬ����ɫ
		myshader.setVec3("ourColor1", glm::vec3(1.0f, 0.4f, 0.4f));
		myshader.setVec3("ourColor2", glm::vec3(0.4f, 1.0f, 0.4f));
		myshader.setVec3("ourColor3", glm::vec3(0.4f, 0.4f, 1.0f));
		myshader.setVec3("ourColor4", glm::vec3(1.0f, 0.4f, 1.0f));

		myshader.setMat4("model", model);
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//-------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	myshader.deleteProgram();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 1;
}

/**
* Ƭ����ɫ�� ��
* ʹ�� gl_FrontFacing
* �� �������ڲ����ⲿʹ�ò�ͬ������
* ע�⣬����㿪�������޳�����Ϳ����������ڲ������ˣ�����������ʹ��gl_FrontFacing��û�������ˡ�
*/
int PractiseAdvancedGLSL_4_8_2::practise2(string projectDir) {

	GLFWwindow* window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Advanced GLSL gl_FrontFacing",
		buffer_window_callback);
	if (window == NULL) { return -1; }

	//---------prepare
	MyShader myshader(projectDir.c_str(), vertFile2, fragFile2);
	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, cubePosition, sizeof(cubePosition), 3,
		cubeTexCoords, sizeof(cubeTexCoords), 2);

	//-----  Uniform �������
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	int tex1 = RenderUtil::textureLoad2D(projectDir, imgFileMarble2);
	int tex2 = RenderUtil::textureLoad2D(projectDir, imgFileMarbleInside);

	myshader.use();
	myshader.setInt("texture_diffuse1", 0);
	myshader.setInt("texture_inside", 1);

	//---------prepare done

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window)) {

		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);

		//-------------render
		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT,
			0.1f, 100.0f);

		myshader.use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2);

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		myshader.setMat4("model", model);
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//-------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	myshader.deleteProgram();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 1;
}


/**
* gl_FragDepth��������������ǿ���ʹ����������ɫ��������Ƭ�ε����ֵ��
* �Լ��������ֵ��һ���ܴ��ȱ�㣬ֻҪ������Ƭ����ɫ���ж�gl_FragDepth����д�룬
* OpenGL�ͻᣨ����Ȳ���С�������۵��������������е���ǰ��Ȳ���(Early Depth Testing)��
* �������õ�ԭ���ǣ�OpenGL�޷���Ƭ����ɫ������֮ǰ��֪Ƭ�ν�ӵ�е����ֵ����ΪƬ����ɫ�����ܻ���ȫ�޸�������ֵ��
* 
* ��OpenGL 4.2�������Կ��Զ����߽���һ���ĵ��ͣ���Ƭ����ɫ���Ķ���ʹ���������(Depth Condition)��������gl_FragDepth������
*
* layout (depth_<condition>) out float gl_FragDepth;
* condition����Ϊ�����ֵ��
* ------------------------------------------------------------------------
*	����			| ����
* ------------------------------------------------------------------------
*	any			| Ĭ��ֵ����ǰ��Ȳ����ǽ��õģ������ʧ�ܶ�����
*	greater		| ��ֻ�������ֵ��gl_FragCoord.z����
*	less		| ��ֻ�������ֵ��gl_FragCoord.z��С
*	unchanged	| �����Ҫд��gl_FragDepth���㽫ֻ��д��gl_FragCoord.z��ֵ
* ------------------------------------------------------------------------
* 
* ͨ���������������Ϊgreater����less��OpenGL���ܼ�����ֻ��д��ȵ�ǰƬ�����ֵ������߸�С��ֵ�ˡ�
* ע���������ֻ��OpenGL 4.2�汾�����ϲ��ṩ��
*/
int PractiseAdvancedGLSL_4_8_2::practise3(string projectDir) {

}