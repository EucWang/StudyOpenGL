#include "RenderShadow.h"

const int SHADOW_WIDTH = 1024;
const int SHADOW_HEIGHT = 1024;
const float near_plane = 1.0f;
const float far_plane = 7.5f;

static GLuint planeVAO, planeVBO;
static GLuint cubeVAO, cubeVBO;
static GLuint quadVAO, quadVBO;

int RenderShadow::practise(const char * projectDir) {
	std::cout << "RenderShadow::practise() runing..." << std::endl;

	WindowHelper helper("Render Shadow", Camera(glm::vec3(0.0f, 3.0f, 2.0f)), 0);
	helper.create();

	MyShader shadowShader(projectDir, vertFileShadowMap, fragFileShadowMap);
	//MyShader testShadowShader(projectDir, vertFileTestShadowMap, fragFileTestShadowMap);
	MyShader shader(projectDir, vertFileRenderShadow, fragFileRenderShadow);

	//testShadowShader.use();
	//testShadowShader.setInt("depthMap", 0);
	shader.use();
	shader.setInt("diffuse_texture", 0);
	shader.setInt("shadow_map", 1);

	int textureWood = RenderUtil::textureLoad2D(projectDir, imgWood, true);

	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);
	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);
	RenderUtil::makeVertexArrayAndBuffer(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 5);

	//--------------- shadow depth frame buffer
	GLuint shadowFrameBuffer, textureShadow;
	glGenFramebuffers(1, &shadowFrameBuffer);

	glGenTextures(1, &textureShadow);
	glBindTexture(GL_TEXTURE_2D, textureShadow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
		0, GL_DEPTH_COMPONENT,  GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureShadow, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//--------------- shadow depth frame buffer done

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();
		//-------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		shadowShader.use();
		shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//glBindVertexArray(planeVAO);
		//glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		//shadowShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		//glBindVertexArray(cubeVAO);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f));
		//shadowShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.0f, 1.5f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.5f));
		//shadowShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		//model = glm::scale(model, glm::vec3(0.25f));
		//shadowShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		renderScene(shadowShader);
		//-------------
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, helper.getScreenWidth(), helper.getScreenHeight());
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//testShadowShader.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureShadow);
		//glBindVertexArray(quadVAO);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWood);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureShadow);

		glm::mat4 view = helper.getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", helper.getCamera().Front);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		//shader.setMat4("model", model);
		//glBindVertexArray(planeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f));
		//shader.setMat4("model", model);
		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.0f, 1.5f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.5f));
		//shader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		//model = glm::scale(model, glm::vec3(0.25f));
		//shader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		renderScene(shader);
		//-------------

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}

void RenderShadow::renderScene(MyShader& shader) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 1.5f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
	model = glm::scale(model, glm::vec3(0.25f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}