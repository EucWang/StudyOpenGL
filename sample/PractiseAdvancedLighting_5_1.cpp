#include "PractiseAdvancedLighting_5_1.h"

int PractiseAdvancedLighting_5_1::practise(const char * projectDir) {
	const char* title = "advanced lighting";
	WindowHelper helper(title, Camera(glm::vec3(1.0f, 1.5f, 0.0f)), 0);
	helper.create();

	MyShader myshader(projectDir, vertFile, fragFile);
	myshader.use();
	myshader.setInt("texture_diffuse1", 0);
	myshader.setInt("texture_specular1", 1);

	MyShader planeshader(projectDir, vertFilePlane, fragFilePlane);
	planeshader.use();
	planeshader.setInt("texture_diffuse1", 0);

	MyShader lightShader(projectDir, vertFile, fragFileLight);

	MyShader screenshader(projectDir, vertFileScreen, fragFileScreen);
	screenshader.use();
	screenshader.setInt("texture_screen", 0);
	
	GLuint cubeVAO, cubeVBO, planeVAO, planeVBO, screenVAO, screenVBO;
	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);
	RenderUtil::makeVertexArrayAndBuffer(&screenVAO, &screenVBO, quadVertices, sizeof(quadVertices), 4);

	GLuint texCube = RenderUtil::textureLoad2D(projectDir, imgFileCube);
	GLuint texCubeSpecular = RenderUtil::textureLoad2D(projectDir, imgFileCubeSpecular);
	GLuint texPlane = RenderUtil::textureLoad2D(projectDir, imgFilePlane);

	GLuint msFBO, msTexFBO, msRenderFBO;
	RenderUtil::makeFramebuffer(&msFBO, &msTexFBO, &msRenderFBO,
		helper.getScreenWidth(), helper.getScreenHeight(), 4);

	GLuint intermediateFBO, intermediateTexFBO;
	RenderUtil::makeFramebuffer(&intermediateFBO, &intermediateTexFBO, NULL,
		helper.getScreenWidth(), helper.getScreenHeight(), 0);

	glUniformBlockBinding(myshader.id, glGetUniformBlockIndex(myshader.id, "Matrices"), 0);
	glUniformBlockBinding(lightShader.id, glGetUniformBlockIndex(lightShader.id, "Matrices"), 0);
	glUniformBlockBinding(planeshader.id, glGetUniformBlockIndex(planeshader.id, "Matrices"), 0);
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		// render to  multi samples' frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myshader.use();
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = helper.getCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);
		// uniform 缓冲
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		// render cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texCube);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texCubeSpecular);
		glBindVertexArray(cubeVAO);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 1.0f));
		myshader.setMat4("model", model);
		bool spot = helper.switchByClickKeyV();
		std::cout << "spot : " << spot << std::endl;
		myshader.setBool("spot", spot);

		//定向光
		myshader.setVec3(("dirlight.ambient"), dirLightArgs[0]);
		myshader.setVec3(("dirlight.diffuse"), dirLightArgs[1]);
		myshader.setVec3(("dirlight.specular"), dirLightArgs[2]);
		//myshader.setVec3(("dirlight.direction"), -0.2f, -1.0f, -0.3f);
		myshader.setVec3(("dirlight.direction"), dirLightArgs[3]);

		//1个点光源
		myshader.setVec3(("pointlight.position"), lightPos);
		myshader.setVec3(("pointlight.ambient"), pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		myshader.setVec3(("pointlight.diffuse"), pointLightColors[0]);
		myshader.setVec3(("pointlight.specular"), pointLightColors[0]);
		myshader.setFloat(("pointlight.constant"), pointLightColors[4].x);
		myshader.setFloat(("pointlight.linear"), pointLightColors[4].y);
		myshader.setFloat(("pointlight.quadratic"), pointLightColors[4].z);

		//1个聚光
		myshader.setVec3(("spotlight.ambient"), spotlightArgs[0]);
		myshader.setVec3(("spotlight.diffuse"), spotlightArgs[1]);
		myshader.setVec3(("spotlight.specular"), spotlightArgs[2]);
		myshader.setVec3(("spotlight.position"), helper.getCamera().Position);
		myshader.setVec3(("spotlight.direction"), helper.getCamera().Front);
		myshader.setFloat(("spotlight.cutoff"), glm::cos(glm::radians(spotlight_cutoff.x)));
		myshader.setFloat(("spotlight.outCutoff"), glm::cos(glm::radians(spotlight_cutoff.y)));
		myshader.setFloat(("spotlight.constant"), spotlightArgs[3].x);
		myshader.setFloat(("spotlight.linear"), spotlightArgs[3].y);
		myshader.setFloat(("spotlight.quadratic"), spotlightArgs[3].z);

		//viewPos
		myshader.setVec3("viewPos", helper.getCamera().Position);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//render plane
		planeshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texPlane);
		glm::mat4 model1(1.0f);
		model1 = glm::translate(model1, glm::vec3(0.0f, -0.004f, 0.0f));
		planeshader.setMat4("model", model1);

		//根据按键切换冯氏光照效果/Blinn-Phong 两种光照
		bool isBlinn = helper.switchByClickKeyB();
		std::cout << "isBlinn = " << isBlinn << std::endl; 
		planeshader.setBool("blinn", isBlinn);
		planeshader.setBool("spot", helper.switchByClickKeyV());
		planeshader.setBool("usePointLight", helper.switchByClickKeyN());

		//定向光
		planeshader.setVec3(("dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
		planeshader.setVec3(("dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
		planeshader.setVec3(("dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
		planeshader.setVec3(("dirlight.direction"), -0.2f, -1.0f, -0.3f);
		planeshader.setVec3(("dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

		//1个点光源
		planeshader.setVec3(("pointlight.position"), lightPos.x, lightPos.y, lightPos.z);
		planeshader.setVec3(("pointlight.ambient"), pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		planeshader.setVec3(("pointlight.diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		planeshader.setVec3(("pointlight.specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		planeshader.setFloat(("pointlight.constant"), pointLightColors[4].x);
		planeshader.setFloat(("pointlight.linear"), pointLightColors[4].y);
		planeshader.setFloat(("pointlight.quadratic"), pointLightColors[4].z);

		//1个聚光
		planeshader.setVec3(("spotlight.ambient"), spotlightArgs[0].x, spotlightArgs[0].y, spotlightArgs[0].z);
		planeshader.setVec3(("spotlight.diffuse"), spotlightArgs[1].x, spotlightArgs[1].y, spotlightArgs[1].z);
		planeshader.setVec3(("spotlight.specular"), spotlightArgs[2].x, spotlightArgs[2].y, spotlightArgs[2].z);
		planeshader.setVec3(("spotlight.position"), helper.getCamera().Position.x, helper.getCamera().Position.y, helper.getCamera().Position.z);
		planeshader.setVec3(("spotlight.direction"), helper.getCamera().Front.x, helper.getCamera().Front.y, helper.getCamera().Front.z);
		planeshader.setFloat(("spotlight.cutoff"), glm::cos(glm::radians(spotlight_cutoff.x)));
		planeshader.setFloat(("spotlight.outCutoff"), glm::cos(glm::radians(spotlight_cutoff.y)));
		planeshader.setFloat(("spotlight.constant"), spotlightArgs[3].x);
		planeshader.setFloat(("spotlight.linear"), spotlightArgs[3].y);
		planeshader.setFloat(("spotlight.quadratic"), spotlightArgs[3].z);

		//viewPos
		planeshader.setVec3("viewPos", helper.getCamera().Position.x, helper.getCamera().Position.y, helper.getCamera().Position.z);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//render light
		lightShader.use();
		glBindVertexArray(cubeVAO);
		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, lightPos);
		model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
		lightShader.setMat4("model", model2);
		lightShader.setVec3("lightColor", pointLightColors[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//转换到普通的非多重采样的2D帧缓冲
		RenderUtil::blitFrameBuffer(&msFBO, &intermediateFBO, helper.getScreenWidth(), helper.getScreenHeight());

		//------- render to default screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenshader.use();
		glBindVertexArray(screenVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, intermediateTexFBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &screenVBO);

	glfwTerminate();
	return 1;
}
