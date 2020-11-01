#include "PractiseSSAO.h"

/// <summary>
/// 对随机数生成的数值， 让核心样本靠近圆点分布，
/// 这是一个家属插值函数
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="f"></param>
/// <returns></returns>
static float lerp(GLfloat a, GLfloat b, GLfloat f) {
	return a + f * (b - a);
}

/// <summary>
/// 创建SSAO 需要的 核心法线半球
/// 随机核心转动的噪点向量
/// 以及包含随机旋转向量的4X4纹理，
/// </summary>
void PractiseSSAO::makeSSAOKernel() {
	//------------------------------------------------------------------------------------
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); //随机浮点数0.0 - 1.0范围 
	std::default_random_engine generator;  //默认的随机数引擎

	for (GLuint i = 0; i < 64; i++) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		GLfloat scale = GLfloat(i) / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);//让随机样本值更加靠近原点分布
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	//随机核心转动
	//创建一个4X4朝向切线空间平面法线的随机旋转向量数组
	for (GLuint i = 0; i < 16; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	//创建一个包含随机旋转向量的4X4纹理， 设定他的封装方法为GL_REPEAT
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

/// <summary>
///  在SSAO阶段和光照阶段之间， 我们还要进行模糊SSAO纹理的处理，创建一个帧缓冲对象来存储模糊结果
/// </summary>
void PractiseSSAO::makeSSAOBlurFrameBuffer() {
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoBlurTex);
	glBindTexture(GL_TEXTURE_2D, ssaoBlurTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, helper->getScreenWidth(), helper->getScreenHeight(),
		0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "make SSAO Blur Frame buffer is not complete." << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
///创建一个SSAO的Framebuffer，只需要红色纹理分量,因为环境遮蔽的结果是一个灰度值
/// </summary>
void PractiseSSAO::makeSSAOFrameBuffer() {
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glGenTextures(1, &ssaoTex);
	glBindTexture(GL_TEXTURE_2D, ssaoTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, helper->getScreenWidth(), helper->getScreenHeight(),
		0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "make SSAO Frame buffer is not complete." << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
///  创建G FrameBuffer
/// </summary>
void PractiseSSAO::makeGFrameBuffer() {

	GLuint grenderbuffer;
	glGenFramebuffers(1, &gframebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gframebuffer);

	glGenTextures(1, &gTexPositionDepth);
	glBindTexture(GL_TEXTURE_2D, gTexPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, helper->getScreenWidth(), helper->getScreenHeight(),
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gTexPositionDepth, 0);

	glGenTextures(1, &gTexNormal);
	glBindTexture(GL_TEXTURE_2D, gTexNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, helper->getScreenWidth(), helper->getScreenHeight(),
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gTexNormal, 0);

	glGenTextures(1, &gTexAlbdoSpec);
	glBindTexture(GL_TEXTURE_2D, gTexAlbdoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, helper->getScreenWidth(), helper->getScreenHeight(),
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gTexAlbdoSpec, 0);
	GLuint attachments[] = { GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &grenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, grenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, helper->getScreenWidth(), helper->getScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, grenderbuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "gframe buffer is not completed." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int PractiseSSAO::practise(const char* projectDir) {
	helper = new WindowHelper("SSAO Demo", Camera(glm::vec3(0.0f, 2.0f, 5.0f)), 0);
	helper->create();

	Model nanoModel(projectDir, modelFileNano, false);

	MyShader myshader(projectDir, vertFile, fragFile);
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);
	myshader.setInt("texture_specular1", 1);

	MyShader gbufferShader(projectDir, vertFileGBuffer, fragFileGBuffer);

	MyShader quadShader(projectDir, vertFileQuad, fragFileQuad); 
	quadShader.use();
	quadShader.setInt("texture_g_pos_depth", 0);
	quadShader.setInt("texture_g_normal", 1);
	quadShader.setInt("texture_g_noise", 2);

	MyShader blurShader(projectDir, vertFileBlur, fragFileBlur);
	blurShader.use();
	blurShader.setInt("texture_ssaoInput", 0);

	MyShader finalLightShader(projectDir, vertFileLight, fragFileLight);
	finalLightShader.use();
	finalLightShader.setInt("texture_gPosDepth", 0);
	finalLightShader.setInt("texture_gNormal", 1);
	finalLightShader.setInt("texture_gAlbedo", 2);
	finalLightShader.setInt("texture_ssao", 3);

	int texContainer = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);
	int texContainerSpec = RenderUtil::textureLoad2D(projectDir, imgFileWoodSpec, false);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	GLuint containerVAO, containerVBO;
	RenderUtil::makeVertexArrayAndBuffer(&containerVAO, &containerVBO, cubeVertices, sizeof(cubeVertices), 8);

	GLuint quadVAO, quadVBO;
	RenderUtil::makeVertexArrayAndBuffer(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 5);

	makeGFrameBuffer();  //创建G FrameBuffer
	makeSSAOFrameBuffer();  
	makeSSAOBlurFrameBuffer();
	makeSSAOKernel();
	
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper->getWindow())) {
		helper->calcProcessInput();


		//step1, g framebuffer :  render scene's geometry/color data into gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, gframebuffer);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = helper->getCamera().GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(helper->getCamera().Zoom),
				helper->getScreenWidth() * 1.0f / helper->getScreenHeight(), 0.1f, 50.0f);
			glm::mat4 model(1.0f);

			myshader.use();
			myshader.setMat4("view", view);
			myshader.setMat4("projection", projection);

			//加载模型
			myshader.setBool("invertedNormals", 0); //
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
			model = glm::scale(model, glm::vec3(0.5f));
			myshader.setMat4("model", model);
			nanoModel.draw(&myshader);

			//加载一个盒子
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -5.0f));
			myshader.setMat4("model", model);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texContainer);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texContainerSpec);
			glBindVertexArray(containerVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
			//myshader.setMat4("model", model);
			//myshader.setBool("invertedNormals", 1); //反转法线
			//glBindVertexArray(containerVAO);
			//glDrawArrays(GL_TRIANGLES, 0, 36);

			
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//step2.  ssao : generate ssao texture
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			quadShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gTexPositionDepth);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gTexNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);

			for (unsigned int i = 0; i < 64; i++) {
				quadShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
			}
			quadShader.setMat4("projection", projection);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//step3 blur ssao and remove noise
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			blurShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ssaoTex);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//step4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		finalLightShader.use();

		//视角空间的光源位置
		glm::vec3 lightPosView = helper->getCamera().GetViewMatrix() * glm::vec4(lightPos, 1.0);
		finalLightShader.setVec3("light.pos", lightPosView);
		finalLightShader.setVec3("light.color", lightColor);
		// Update attenuation parameters
		const float linear = 0.09;
		const float quadratic = 0.032;
		finalLightShader.setFloat("light.Linear", linear);
		finalLightShader.setFloat("light.Quadratic", quadratic);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gTexPositionDepth);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gTexNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gTexAlbdoSpec);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, ssaoBlurTex);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(helper->getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}
