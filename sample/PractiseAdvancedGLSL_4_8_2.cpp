#include "PractiseAdvancedGLSL_4_8_2.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

static double deltaTime;
static double lastFrame;
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

	GLFWwindow* window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		"Advanced GLSL gl_FrontFacing", buffer_window_callback);
	if (window == NULL) { return -1; }

	//---------prepare
	MyShader myshader(projectDir.c_str(), vertFile2, fragFile2);
	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayFromSubData(&VAO, &VBO, 
		cubePosition, sizeof(cubePosition), 3,
		cubeTexCoords, sizeof(cubeTexCoords), 2);

	int tex1 = RenderUtil::textureLoad2D(projectDir, imgFileMarble2);
	int tex2 = RenderUtil::textureLoad2D(projectDir, imgFileMarbleInside);
	int tex3 = RenderUtil::textureLoad2D(projectDir, imgFilePlane);

	myshader.use();
	myshader.setInt("texture_diffuse1", 0);
	myshader.setInt("texture_inside", 1);

	//-------------------
	MyShader planeshader(projectDir.c_str(), vertFilePlane, fragFilePlane);
	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayFromSubData(&planeVAO, &planeVBO,
		planePosition, sizeof(planePosition), 3,
		planeTexCoords, sizeof(planeTexCoords), 2);

	planeshader.use();
	planeshader.setInt("texture_diffuse1", 0);

	//-------------------- Uniform������� 
	glUniformBlockBinding(myshader.id,
		glGetUniformBlockIndex(myshader.id, "Matrices"), 0);  //�����Ӷ�����ɫ���е�Uniform����󶨵�ָ���İ󶨵�0��

	glUniformBlockBinding(planeshader.id,
		glGetUniformBlockIndex(planeshader.id, "Matrices2"), 0); //������Ķ�����ɫ���е�Uniform�������󶨵�ָ���İ󶨵�0��

	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));  //���Ǹ�Uniform�������Ҳ�󶨵�ָ���İ󶨵�0��
	//-------------------- Uniform������� done

	//---------prepare done

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	while (!glfwWindowShouldClose(window)) {

		float curFrame = (float)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);

		//-------------render

		//--------�ӽǡ��Ӵ�����
		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT,
			0.1f, 100.0f);
		//--------�ӽǡ��Ӵ����� done

		//----------����Uniform��������е�����
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//----------����Uniform��������е����� done

		myshader.use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2);

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		myshader.setMat4("model", model);
		//myshader.setMat4("view", view);
		//myshader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//---------------- ��Ⱦ����
		planeshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex3);

		glm::mat4 model2(1.0);
		model2 = glm::translate(model2, glm::vec3(0.0f, -0.005f, 0.0f));
		planeshader.setMat4("model", model2);
		glDrawArrays(GL_TRIANGLES, 0, 6);


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