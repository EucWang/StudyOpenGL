#include "PractiseAntiAliasing_4_11.h"

/// <summary>
/// 普通场景, 离屏渲染, 而不是采用opengl默认的颜色缓冲的抗锯齿
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseAntiAliasing_4_11::practise0(string projectDir) {
	const char *title = "render with anti aliasing in custom frame buffer"; 
	WindowHelper windowHelper(title, Camera(glm::vec3(0.0f, 3.0f, 10.0f)), 4);
	windowHelper.create();

	MyShader myshader(projectDir.c_str(), vertFile0, fragFile0);

	GLuint VAO, VBO;
	RenderUtil::makeVertexArrayAndBuffer(&VAO, &VBO, cubeVertices, sizeof(cubeVertices), 8);

	//--------uniform buffer
	glUniformBlockBinding(myshader.id,
		glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

	//--------uniform buffer done

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(windowHelper.getWindow())) {
		//windowHelper.calcDeltaTime();
		//windowHelper.getProcessInput()(windowHelper.getWindow());
		windowHelper.calcProcessInput();
	
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = windowHelper.getCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(windowHelper.getCamera().Zoom),
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//------render
		myshader.use();
		glBindVertexArray(VAO);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		myshader.setMat4("model", model);
		myshader.setVec3("ourColor", glm::vec3(0.5f, 0.6f, 0.7f));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//});
		//------render done

		glfwSwapBuffers(windowHelper.getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();
	myshader.deleteProgram();
}


/// <summary>
/// 通过草地来查看抗锯齿效果
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>R
int PractiseAntiAliasing_4_11::practise(string projectDir) { 
	const char* title = "anti aliasing"; 
	WindowHelper windowHelper(title, Camera(glm::vec3(0.0f, 3.0f, 3.0f)), 4);
	windowHelper.create();

	std::cout << "prepare data." << std::endl;
	//-------------prepare data
	MyShader myshader(projectDir.c_str(), vertFile, fragFile);
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	int texPlane = RenderUtil::textureLoad2D(projectDir, imgFilePlane);

	MyShader shaderGrass(projectDir.c_str(), vertFileGrass, fragFileGrass);
	shaderGrass.use();
	shaderGrass.setInt("texture_diffuse1", 0);

	GLuint grassVAO, grassVBO;
	RenderUtil::makeVertexArrayAndBuffer(&grassVAO, &grassVBO, grassVertices, sizeof(grassVertices), 5);

	int texGrass = RenderUtil::textureLoad2D(projectDir, imgFileGrass, 
		GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	//---prepare uniform buffer
	glUniformBlockBinding(myshader.id,
		glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	glUniformBlockBinding(shaderGrass.id,
		glGetUniformBlockIndex(shaderGrass.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
	//---prepare uniform buffer done 

	srand(glfwGetTime());
	glm::mat4* modelMatrices;
	float offset = 4.5f;
	int amount = 512;
	modelMatrices = new glm::mat4[amount];
	for (int i = 0; i < amount; i++) {
		//随机位移位置
		float x = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//随机沿着y轴旋转角度
		float rot = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//在 [0.5 , 2.5f] 之间随机缩放系数
		float scaleX = (rand() % 200) / 100.0f + 0.5;
		float scaleY = (rand() % 200) / 100.0f + 0.5;
		glm::mat4 model(1.0f);
		//因为缩放了,所以草不一定在地上,在y轴上也有一个位移量
		model = glm::translate(model, glm::vec3(x, 0.5f* (scaleY - 1), z));
		model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0));////2. 缩放, 
		model = glm::rotate(model, rot * glm::radians(360.0f), glm::vec3(0.0f, 10.0f, 0.0f)); //有轴旋转
		modelMatrices[i] = model;
	}

	//--- prepare instancing array
	int sizeVec4 = sizeof(glm::vec4);
	GLuint instancingVBO;
	glBindVertexArray(grassVAO);
	glGenBuffers(1, &instancingVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancingVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, )
	glBufferData(GL_ARRAY_BUFFER, amount * 4 * sizeVec4, &modelMatrices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(sizeVec4));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(2 *sizeVec4));
	
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(3 *sizeVec4));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//--- prepare instancing array done

	//-------------prepare data done
	std::cout << "prepare data done." << std::endl;

	glEnable(GL_DEPTH_TEST);  //开启深度缓冲
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);  //开启透明颜色混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE); //开启多重采样缓冲,抗锯齿

	while (!glfwWindowShouldClose(windowHelper.getWindow())) {
		//windowHelper.calcDeltaTime();
		//windowHelper.getProcessInput()(windowHelper.getWindow());
		windowHelper.calcProcessInput();

		//-------------render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = windowHelper.getCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(windowHelper.getCamera().Zoom),
			DEFAULT_SCREEN_WIDTH * 1.0f / DEFAULT_SCREEN_HEIGHT, 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		myshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texPlane);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		shaderGrass.use();
		glBindVertexArray(grassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texGrass);
		//for (int i = 0; i < amount; i++) {
		//	shaderGrass.setMat4("model", modelMatrices[i]);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, amount);

		//-------------render done

		glfwSwapBuffers(windowHelper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &grassVBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &grassVAO);
	myshader.deleteProgram();
	shaderGrass.deleteProgram();

	return 1;
}


/// <summary>
///  离屏多重采样缓冲
///  帧缓冲
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PractiseAntiAliasing_4_11::practise1(string projectDir) {
	WindowHelper window("sample of frame buffer and custom anti aliasing");
	window.create();


	MyShader myshader(projectDir.c_str(), vertFile3, fragFile3);
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);

	GLuint cubeVAO, cubeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);

	int texCube = RenderUtil::textureLoad2D(projectDir, imgFileCube);

	//--------uniform buffer
	glUniformBlockBinding(myshader.id, glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
	//--------uniform buffer done

	MyShader screenshader(projectDir.c_str(), vertFileScreen, fragFileScreen);
	GLuint screenVAO, screenVBO;
	RenderUtil::makeVertexArrayAndBuffer(&screenVAO, &screenVBO, quadVertices, sizeof(quadVertices), 4);
	screenshader.use();
	screenshader.setInt("texture_screen", 0);
	//---------frame buffer 帧缓冲
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//-------给帧缓冲附加上渲染输出的纹理
	unsigned int texFrameBuffer;   //给frame buffer提供渲染输出的空白纹理
	glGenTextures(1, &texFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, texFrameBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.getScreenWidth(), window.getScreenHeight(),
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texFrameBuffer, 0); //绑定framebuffer和texture
	//------- 给帧缓冲附加上渲染缓冲
	unsigned int renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.getScreenWidth(), window.getScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//---------
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {  //检查帧缓冲是否创建成功
		std::cout << "ERROR:FRAMEBUFFER::CREATE FAILED." << std::endl;
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//---------frame buffer done

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window.getWindow())) {
		window.calcProcessInput();

		//---------- render to frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myshader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texCube);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = window.getCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(window.getCamera().Zoom), 
			window.getScreenWidth() * 1.0f / window.getScreenHeight(), 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//------------- render to frame buffer done

		//------- render to screen
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenshader.use();
		glBindVertexArray(screenVAO);
		glBindTexture(GL_TEXTURE_2D, texFrameBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//------- render to screen

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}
	glfwTerminate();
	myshader.deleteProgram();
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &UBO);

	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &renderBuffer);
	glDeleteTextures(1, &texFrameBuffer);
	screenshader.deleteProgram();
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenVBO);
}