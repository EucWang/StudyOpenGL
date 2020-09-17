#include "PractiseFrameBuffer_kernel_4_5.h"

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

static float glassVertices[] = {
	//positions		  //normal       	//texture Coords
	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,
	0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f,  1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,

	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,
	-0.5f, -0.5f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f,  0.0f,
};

static float grassVertices[] = {
	//positions				//texture Coords
	-0.5f, 0.5f, 0.0f,		0.0f,  1.0f,
	0.5f, 0.5f, 0.0f,		1.0f,  1.0f,
	0.5f, -0.5f, 0.0f,		1.0f,  0.0f,

	-0.5f, 0.5f, 0.0f,		0.0f,  1.0f,
	0.5f, -0.5f, 0.0f,		1.0f,  0.0f,
	-0.5f, -0.5f, 0.0f,		0.0f,  0.0f,
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

int PractiseFrameBuffer_kernel_4_5::practise(const char* projectDir) {

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "draw frame buffer with kernel");
	if (window == NULL) { return -1; }

	//MyShader shaderScreen(projectDir, vertFileScreen, fragFileScreen);
	MyShader shaderScreen(projectDir, vertFileScreen, fragFileScreen2);
	MyShader shaderFrame(projectDir, vertFileFrame, fragFileFrame);
	MyShader shaderGrass(projectDir, vertFileGrass, fragFileGrass);

	GLuint cubeVAO, cubeVBO, planeVAO, planeVBO, quadVAO, quadVBO, glassVAO, glassVBO, grassVAO, grassVBO;
	makeVAOVBO(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);
	makeVAOVBO(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);
	makeVAOVBO(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 4);
	makeVAOVBO(&glassVAO, &glassVBO, glassVertices, sizeof(glassVertices), 8);
	makeVAOVBO(&grassVAO, &grassVBO, grassVertices, sizeof(grassVertices), 5);

	int texContainer = textureLoad(projectDir, imageFileContainer);
	int texPlane = textureLoad(projectDir, imageFilePlane);
	int texGlass = textureLoad(projectDir, imageFileGlass);
	int texGrass = textureLoad(projectDir, imageFileGrass);
	shaderScreen.use();
	shaderScreen.setInt("screenTexture", 0);
	shaderFrame.use();
	shaderFrame.setInt("texture_diffuse", 0);
	shaderGrass.use();
	shaderGrass.setInt("texture1", 0);

	//-----------------prepare framebuffer
	unsigned int framebufferObj;
	glGenFramebuffers(1, &framebufferObj);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferObj);  //绑定帧缓冲
	//------------  纹理对象(用于接收帧缓冲的输出,以及作为屏幕缓冲的输入)
	unsigned int texFrameBuffer;    
	glGenTextures(1, &texFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, texFrameBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texFrameBuffer, 0);//绑定纹理到准缓冲上
	//-----------   深度和模板渲染缓冲对象
	unsigned int renderBufferObj;  
	glGenRenderbuffers(1, &renderBufferObj);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObj);
	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT);//创建一个深度和模板渲染缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObj);//绑定深度和模板渲染缓冲对象到帧缓冲上
	//------------   检查帧缓冲是否完整, 如果不完整则不能使用
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: FrameBuffer is not completed!" << std::endl;
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);        //解除纹理绑定
	glBindRenderbuffer(GL_RENDERBUFFER, 0);  //解除渲染缓冲绑定
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  //解除帧缓冲绑定
	//-----------------prepare framebuffer done


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE); 只在需要面剔除的对象渲染之前绘制,不需要面剔除的对象渲染时不需要绘制
	glEnable(GL_BLEND);  //允许颜色混淆
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //根据透明度做混淆

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		processInput(window);

		//-------------------------------------render
		//------   render to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObj);  //使用帧缓冲
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		shaderFrame.use();

		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT,
			0.1f, 100.0f);
		shaderFrame.setMat4("view", view);
		shaderFrame.setMat4("projection", projection);
		//渲染地面
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texPlane);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		shaderFrame.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//渲染箱子
		glEnable(GL_CULL_FACE);
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, texContainer);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)curFrame * glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shaderFrame.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//渲染草
		glDisable(GL_CULL_FACE);//禁用面剔除
		shaderGrass.use();
		glBindVertexArray(grassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texGrass);
		shaderGrass.setMat4("view", view);
		shaderGrass.setMat4("projection", projection);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 1.8f));
		shaderGrass.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//渲染玻璃
		glDisable(GL_CULL_FACE);//禁用面剔除
		shaderFrame.use();
		glBindVertexArray(glassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texGlass);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 2.0f));
		shaderFrame.setMat4("model", model);
		shaderFrame.setMat4("view", view);
		shaderFrame.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 结束帧缓冲
		//------   render to framebuffer done

		//------   render to screen
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderScreen.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texFrameBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//------   render to screen done

		//-------------------------------------render done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &quadVBO);
	shaderFrame.deleteProgram();
	shaderScreen.deleteProgram();

	glDeleteFramebuffers(1, &framebufferObj);
	glfwTerminate();
}