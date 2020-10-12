#include "ShadowMapping.h"

#define SHADOW_WIDTH  1024
#define SHADOW_HEIGHT 1024

GLuint planeVAO, planeVBO;
GLuint cubeVAO, cubeVBO;
GLuint quadVAO, quadVBO;

GLfloat near_plane = 1.0f;
GLfloat far_plane = 7.5;

int ShadowMapping::practise(const char * projectDir) {
	std::cout << "ShadowMapping::practise()" << std::endl;

	WindowHelper helper("shadow mapping demo", Camera(glm::vec3(0.0f, 3.0f, 2.0f)), 0);
	helper.create();

	MyShader depthShader(projectDir, vertFile, fragFile);
	MyShader screenShader(projectDir, vertFileScreen, fragFileScreen);
	screenShader.use();
	screenShader.setInt("depthMap", 0);

	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);
	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);
	RenderUtil::makeVertexArrayAndBuffer(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 5);
	
	//------------------------- shadow depth frame buffer
	GLuint shadowFrameBuffer;
	glGenFramebuffers(1, &shadowFrameBuffer);

	GLuint textureShadow;
	glGenTextures(1, &textureShadow);
	glBindTexture(GL_TEXTURE_2D, textureShadow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowFrameBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//------------------------- shadow depth frame buffer done

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();
		//------------------------

		//1. 首先 渲染深度阴影贴图
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//1.0 configure shader and matrices
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//1.1 render plane
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		depthShader.setMat4("model", model);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//1.2 render cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		depthShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		depthShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25f));
		depthShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
		depthShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//2. 像 往常一样渲染场景, 但是这次试用深度阴影贴图
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, helper.getScreenWidth(), helper.getScreenHeight());
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		screenShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureShadow);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//------------------------
		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}


	glfwTerminate();
	return 1;
}