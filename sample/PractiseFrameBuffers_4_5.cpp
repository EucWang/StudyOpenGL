#include "PractiseFrameBuffers_4_5.h"
 
static double deltaTime;
static double lastFrame;
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

static float quadVertices[] = {
	// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
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

static void make_texture_attach(unsigned int  * tex, int index = 0);

static void make_render_buffer_obj_attach(unsigned int  * rbo);

int PractiseFrameBuffers_4_5::practise(const char * projectDir) {
	std::cout << "PractiseFrameBuffers_4_5.practise()" << std::endl;
	//---------------------------------------------------------------------
	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "practise frame buffer");
	if (window == NULL) { return -1; }

	MyShader shader(projectDir, vertFile, fragFile);  //往帧缓冲写入的shader
	//MyShader screenshader(projectDir, vertFileScreen, fragFileScreen);   //普通的没有处理的屏幕shader
	//MyShader screenReverseColorShader(projectDir, vertFileScreen, fragFileReverseColor);   //反转颜色的屏幕的shader
	MyShader screenGrayShader(projectDir, vertFileScreen, fragFileGray);   //将颜色置位灰度的屏幕shader

	GLuint quadVAO, quadVBO;
	makeVAOVBO(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 4);
	GLuint cubeVAO, cubeVBO;
	makeVAOVBO(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);
	GLuint planeVAO, planeVBO;
	makeVAOVBO(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	int cubTexture;
	if ((cubTexture = textureLoad(projectDir, imgContainer)) <= 0) { return -1; }
	int planeTexture;
	if ((planeTexture = textureLoad(projectDir, imgPlane)) <= 0) { return -1; }

	shader.use();
	shader.setInt("texture1", 0);
	//screenshader.use();
	//screenshader.setInt("screenTexture", 0);
	//screenReverseColorShader.use();
	//screenReverseColorShader.setInt("screenTexture", 0);
	screenGrayShader.use();
	screenGrayShader.setInt("screenTexture", 0);

	//---------------------------------------------------------------------
	unsigned int framebuffer;   //一个帧缓冲对象
	glGenFramebuffers(1, &framebuffer);  //创建一个帧缓冲对象
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  //绑定帧缓冲对象 到 GL_FRAMEBUFFER
	unsigned int texColorBuffer;  //一个空的纹理图像
	make_texture_attach(&texColorBuffer);
	//附加带深度和模板缓冲的纹理
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH,
	//	SMALL_SCREEN_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
	//	GL_TEXTURE_2D, texture, 0);
	
	unsigned int rbo;//渲染缓冲对象附件
	make_render_buffer_obj_attach(&rbo);
	//检查帧缓冲是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
		return -1;
	}
	//解绑帧缓冲, 激活默认的窗口缓冲, 以便让主窗口能正常的渲染
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//---------------------------------------------------------------------

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//--------------------------render to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		//渲染2个箱子
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubTexture);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, (float)curFrame * glm::radians(10.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, 1.0f));
		model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(34.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		shader.setMat4("model", model2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(1.0f, 0.0f, -1.0f));
		model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(12.0f), glm::vec3(0.0f, 1.2f, 0.0f));
		shader.setMat4("model", model2);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//渲染地板
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 禁用深度缓冲, 因为屏幕是标准的二维像素,不需要深度缓冲
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  //解除帧缓冲绑定

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //清空屏幕颜色
		glClear(GL_COLOR_BUFFER_BIT);         //清空颜色缓冲
		//--------------------------render to framebuffer done

		//------------------render framebuffer to default screen 
		//screenshader.use();
		//screenReverseColorShader.use();
		screenGrayShader.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);  //将帧缓冲生成的纹理对象texColorBuffer写入到屏幕帧缓冲中
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//------------------render framebuffer to default screen  done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//---------------------------------------------------------------------
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	//完成所有的帧缓冲操作之后, 删除此帧缓冲
	glDeleteFramebuffers(1, &framebuffer);
	glfwTerminate();
	//---------------------------------------------------------------------
}

/// <summary>
/// 
/// 添加颜色缓冲/深度缓冲,颜色附件Attachment
/// 附件是一个内存位置，它能够作为帧缓冲的一个缓冲，可以将它想象为一个图像。当创建一个附件的时候我们有两个选项：
/// 纹理或渲染缓冲对象(Renderbuffer Object)。
///
/// 纹理附件,当把一个纹理附加到帧缓冲的时候，所有的渲染指令将会写入到这个纹理中，就想它是一个普通的颜色/深度或模板缓冲一样。
///	使用纹理的优点是，所有渲染操作的结果将会被储存在一个纹理图像中，我们之后可以在着色器中很方便地使用它。
/// </summary>
/// <param name="texture"> out </param>
void make_texture_attach(unsigned int * texColorBuffer, int index) {
	glGenTextures(1, texColorBuffer);   
	glBindTexture(GL_TEXTURE_2D, *texColorBuffer);
	//主要的区别就是，我们将维度设置为了屏幕大小（尽管这不是必须的），并且我们给纹理的data参数传递了NULL。
	//对于这个纹理，我们仅仅分配了内存而没有填充它。填充这个纹理将会在我们渲染到帧缓冲之后来进行。
	//同样注意我们并不关心环绕方式或多级渐远纹理，我们在大多数情况下都不会需要它们。
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);  //清空绑定

	//然后把这个创建好的纹理附加到帧缓冲上, 这就附加了颜色缓冲
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D,
		*texColorBuffer, 0);

	std::cout << "make_texture_attach : texture = " << *texColorBuffer << std::endl;
}

/// <summary>
/// 生成一个渲染缓冲对象, 带深度和模板缓冲的
/// </summary>
/// <param name="rbo"></param>
void make_render_buffer_obj_attach(unsigned int * rbo) {
	glGenRenderbuffers(1, rbo);   //创建渲染缓冲对象
	glBindRenderbuffer(GL_RENDERBUFFER, *rbo);  //绑定渲染缓冲对象

	//创建一个深度和模板渲染缓冲对象
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH, SMALL_SCREEN_WIDTH);

	glBindRenderbuffer(GL_RENDERBUFFER, 0); //清理掉绑定

	//附加这个渲染缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *rbo);

}