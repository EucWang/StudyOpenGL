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
* 片段着色器 中
* 使用 gl_FragCoord.x/.y 根据片段在窗口中的位置来应用不同的颜色
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


		//根据片段着色器中的 gl_FragCoord.x/.y 来指定显示不同的颜色
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
* 片段着色器 中
* 使用 gl_FrontFacing
* 在 正方体内部和外部使用不同的纹理
* 注意，如果你开启了面剔除，你就看不到箱子内部的面了，所以现在再使用gl_FrontFacing就没有意义了。
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

	//-------------------- Uniform缓冲矩阵 
	glUniformBlockBinding(myshader.id,
		glGetUniformBlockIndex(myshader.id, "Matrices"), 0);  //将箱子顶点着色器中的Uniform对象绑定到指定的绑定点0上

	glUniformBlockBinding(planeshader.id,
		glGetUniformBlockIndex(planeshader.id, "Matrices2"), 0); //将地面的顶点着色器中的Uniform缓冲对象绑定到指定的绑定点0上

	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));  //及那个Uniform缓冲对象也绑定到指定的绑定点0上
	//-------------------- Uniform缓冲矩阵 done

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

		//--------视角、视窗矩阵
		glm::mat4 view(1.0);
		glm::mat4 projection(1.0);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT,
			0.1f, 100.0f);
		//--------视角、视窗矩阵 done

		//----------更新Uniform缓冲对象中的数据
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//----------更新Uniform缓冲对象中的数据 done

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

		//---------------- 渲染地面
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
* gl_FragDepth的输出变量，我们可以使用它来在着色器内设置片段的深度值。
* 自己设置深度值有一个很大的缺点，只要我们在片段着色器中对gl_FragDepth进行写入，
* OpenGL就会（像深度测试小节中讨论的那样）禁用所有的提前深度测试(Early Depth Testing)。
* 它被禁用的原因是，OpenGL无法在片段着色器运行之前得知片段将拥有的深度值，因为片段着色器可能会完全修改这个深度值。
* 
* 从OpenGL 4.2起，我们仍可以对两者进行一定的调和，在片段着色器的顶部使用深度条件(Depth Condition)重新声明gl_FragDepth变量：
*
* layout (depth_<condition>) out float gl_FragDepth;
* condition可以为下面的值：
* ------------------------------------------------------------------------
*	条件			| 描述
* ------------------------------------------------------------------------
*	any			| 默认值。提前深度测试是禁用的，你会损失很多性能
*	greater		| 你只能让深度值比gl_FragCoord.z更大
*	less		| 你只能让深度值比gl_FragCoord.z更小
*	unchanged	| 如果你要写入gl_FragDepth，你将只能写入gl_FragCoord.z的值
* ------------------------------------------------------------------------
* 
* 通过将深度条件设置为greater或者less，OpenGL就能假设你只会写入比当前片段深度值更大或者更小的值了。
* 注意这个特性只在OpenGL 4.2版本或以上才提供。
*/
int PractiseAdvancedGLSL_4_8_2::practise3(string projectDir) {

}