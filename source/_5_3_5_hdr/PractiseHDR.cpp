#include "PractiseHDR.h"

static float exposure = 1.0f;

int PractiseHDR::practise(const char* projectDir) {
	std::cout << "PractiseHDR::practise() run" << std::endl;
	WindowHelper helper("HDR Demo", Camera(glm::vec3(0.0f, 0.0f, 5.0f)), 0);
	helper.create();

	GLuint hdrFrameBuffer;
	GLuint hdrTexture;
	GLuint hdrRenderBuffer;
	glGenTextures(1, &hdrTexture);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, helper.getScreenWidth(), helper.getScreenHeight(), 
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &hdrRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, helper.getScreenWidth(), helper.getScreenHeight());

	glGenFramebuffers(1, &hdrFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRenderBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	MyShader myshader(projectDir, vertFile, fragFile);
	myshader.use();
	myshader.setInt("texture_diffuse", 0);

	MyShader quadShader(projectDir, vertFileQuad, fragFileQuad);
	quadShader.use();
	quadShader.setInt("hdrBuffer", 0);

	int texWood = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);

	// lighting info
	// -------------
	// positions
	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
	lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, cubeVertices, sizeof(cubeVertices), 8);

	GLuint quadVAO, quadVBO;
	RenderUtil::makeVertexArrayAndBuffer(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 5);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = helper.getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

		myshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texWood);

		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		//myshader.setVec3("lightPos", lightPos);
		for (unsigned int i = 0; i < lightPositions.size(); i++) {
			myshader.setVec3("lights[" + std::to_string(i) + "].lightPos", lightPositions[i]);
			myshader.setVec3("lights[" + std::to_string(i) + "].lightColor", lightColors[i]);
		}
		myshader.setVec3("viewPos", helper.getCamera().Position);

		//render tunnel
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
		myshader.setMat4("model", model);
		myshader.setInt("inverse_normals", helper.switchByClickKeyB());

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		quadShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		quadShader.setBool("hdr", helper.switchByClickKeyN());
		quadShader.setFloat("exposure", exposure);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	std::cout << "HDR run done." << std::endl;
	return 1;
}