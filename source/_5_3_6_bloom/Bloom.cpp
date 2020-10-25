#include "Bloom.h"

static float exposure = 1.0f;

int Bloom::practise(const char* projectDir) {

	std::cout << "Bloom::practise（）" << std::endl;

	WindowHelper helper("Bloom", Camera(glm::vec3(0.0f, 0.0f, 5.0f)), 0);
	helper.create();

	// lighting info
	// -------------
	// positions
	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
	lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
	lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

	MyShader quadShader(projectDir, vertFileQuad, fragFileQuad);
	quadShader.use();
	quadShader.setInt("texture_bloom", 0);
	quadShader.setInt("texture_bloomblur", 1);

	MyShader blurShader(projectDir, vertFileQuad, fragFileBlur);
	blurShader.use();
	blurShader.setInt("texture_diffuse", 0);

	GLuint quadVAO, quadVBO;
	RenderUtil::makeVertexArrayAndBuffer(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 5);

	MyShader planeShader(projectDir, vertFile, fragFile);
	planeShader.use();
	planeShader.setInt("texture_diffuse", 0);

	int texWood = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);
	int texContainer = RenderUtil::textureLoad2D(projectDir, imgFileContainer2, false);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	MyShader lightShader(projectDir, vertFileLight, fragFileLight);

	GLuint cubeVAO, cubeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);

	GLuint bloomFramebuffer;
	GLuint bloomTexture[2];
	GLuint bloomRenderBuffer;

	glGenFramebuffers(1, &bloomFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFramebuffer);
	glGenTextures(2, bloomTexture);
	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, bloomTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, helper.getScreenWidth(),
			helper.getScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, bloomTexture[i], 0);
	}
 	
	glGenRenderbuffers(1, &bloomRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, bloomRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, helper.getScreenWidth(), helper.getScreenHeight());

	GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);  //显式告知OpenGL我们正在通过glDrawBuffers渲染到多个颜色缓冲，否则OpenGL只会渲染到帧缓冲的第一个颜色附件
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, bloomRenderBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "create framebuffer is not complete!" << std::endl;
	}

	GLuint pingpongFBO[2];
	GLuint pingpongTexture[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongTexture);
	for (unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);

		glBindTexture(GL_TEXTURE_2D, pingpongTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, helper.getScreenWidth(),
			helper.getScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTexture[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "create pingpong framebuffer is not complete." << std::endl;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, bloomFramebuffer);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//-----render -------------

		glm::mat4 view = helper.getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth()*1.0f/helper.getScreenHeight(), 0.1f, 100.0f);

		planeShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texWood);

		planeShader.setMat4("view", view);
		planeShader.setMat4("projection", projection);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		planeShader.setMat4("model", model);
		planeShader.setVec3("viewPos", helper.getCamera().Position);

		for (int i = 0; i < 4; i++) {
			planeShader.setVec3("lights[" + std::to_string(i) + "].pos", lightPositions[i]);
			planeShader.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
		}

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//-----------reder cube
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainer);

		// create one large cube that acts as the floor
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		//model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
		//planeShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// then create multiple cubes as the scenery
		//glBindTexture(GL_TEXTURE_2D, texContainer);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		planeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		planeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		planeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.25));
		planeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		planeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		planeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//-----------reder cube done

		lightShader.use();
		glBindVertexArray(cubeVAO);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		//lightShader.setVec3("lightColor", lightColor);

		for (unsigned int i = 0; i < lightPositions.size(); i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(lightPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			lightShader.setMat4("model", model);
			lightShader.setVec3("lightColor", lightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
 
		//------------------------------------------------------------------
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 10;
		blurShader.use();
		//得到一个HDR纹理后，我们用提取出来的亮区纹理填充一个帧缓冲，然后对其模糊处理10次（5次垂直5次水平）：
		//重复这个步骤10次，亮区图像就进行一个重复5次的高斯模糊了。这样我们可以对任意图像进行任意次模糊处理；高斯模糊循环次数越多，模糊的强度越大。
		for (size_t i = 0; i < amount; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			blurShader.setInt("horizontal", horizontal);
			GLuint tex;
			if (first_iteration) {
				//第一次，只有高亮区域的纹理
				tex = bloomTexture[1]; 
			} else {
				//第二次模糊渲染时， 用的是模糊处理高亮区域之后的结果，也就是第一个pingpongTexture，
				tex = pingpongTexture[!horizontal]; 
			}
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			horizontal = !horizontal;
			if (first_iteration) { first_iteration = false; }
		}
		
		//------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quadShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bloomTexture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongTexture[!horizontal]);
		glBindVertexArray(quadVAO);
		bool bloom = helper.switchByClickKeyB();
		bool use_gamma = helper.switchByClickKeyN();
		quadShader.setBool("bloom", bloom);
		quadShader.setFloat("exposure", exposure);
		quadShader.setBool("use_gamma", use_gamma);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
		//-----render done --------
		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}