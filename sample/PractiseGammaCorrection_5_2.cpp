#include "PractiseGammaCorrection_5_2.h"


int PractiseGammaCorrection_5_2::practise(const char* projectDir) {
	WindowHelper helper("Gamma Correction 5.2", Camera(glm::vec3(0.0f, 1.5f, 3.0f)), 0);
	helper.create();

	//----prepare data
	MyShader lightshader(projectDir, vertFileLight, fragFileLight);
	GLuint lightVAO, lightVBO;
	RenderUtil::makeVertexArrayAndBuffer(&lightVAO, &lightVBO, cubeVertices, sizeof(cubeVertices), 8);

	MyShader planeshader(projectDir, vertFile, fragFile);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	GLuint texturePlane = RenderUtil::textureLoad2D(projectDir, imgFileWood);
	GLuint texturePlaneSpecular = RenderUtil::textureLoad2D(projectDir, imgFileWoodSpecular);
	planeshader.use();
	planeshader.setInt("texture_diffuse1", 0);
	planeshader.setInt("texture_specular11", 1);

	//------- Uniform buffer
	GLuint planeUBO;
	glGenBuffers(1, &planeUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, planeUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUniformBlockBinding(planeshader.id, glGetUniformBlockIndex(planeshader.id, "Matrices"), 0);
	glUniformBlockBinding(lightshader.id, glGetUniformBlockIndex(lightshader.id, "Matrices1"), 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, planeUBO, 0, 2 * sizeof(glm::mat4));
	//------- Uniform buffer done

	MyShader screenshader(projectDir, vertFileScreen, fragFileScreen);

	GLuint screenVAO, screenVBO;
	RenderUtil::makeVertexArrayAndBuffer(&screenVAO, &screenVBO, quadVertices, sizeof(quadVertices), 4);
	screenshader.use();
	screenshader.setInt("texture_screen", 0);

	//----prepare data done

	GLuint customFrameBuffer, customTexture, customRenderBuffer;
	RenderUtil::makeFramebuffer(&customFrameBuffer, &customTexture, &customRenderBuffer,
		helper.getScreenWidth(), helper.getScreenHeight(), 4);
	GLuint intermediatedFrameBuffer, intermediatedTexture;
	RenderUtil::makeFramebuffer(&intermediatedFrameBuffer, &intermediatedTexture, NULL,
		helper.getScreenWidth(), helper.getScreenHeight(), 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		//--render to custom frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, customFrameBuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//- render

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = helper.getCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, planeUBO);    //set uniform buffer
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//lightshader.use();
		//glBindVertexArray(lightVAO);
		//glm::mat4 model3(1.0f);
		//model3 = glm::translate(model3, lightPos);
		//model3 = glm::scale(model3, glm::vec3(0.1f, 0.1f, 0.1f));
		//lightshader.setMat4("model", model3);
		//lightshader.setVec3("lightColor", pointLightColors[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		planeshader.use();
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePlane);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturePlaneSpecular);
		///////////
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

		planeshader.setVec3("viewPos", helper.getCamera().Front.x, helper.getCamera().Front.y, helper.getCamera().Front.z);
		planeshader.setFloat("shininess", 64.0f);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		planeshader.setMat4("model", model);
		planeshader.setVec3("texture_specular", glm::vec3(0.5f, 0.5f, 0.5f));

		planeshader.setBool("useSpot", helper.switchByClickKeyV());
		planeshader.setBool("usePoint", helper.switchByClickKeyB());
		planeshader.setBool("useBlinn", helper.switchByClickKeyN());
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//- render done
		
		RenderUtil::blitFrameBuffer(&customFrameBuffer, &intermediatedFrameBuffer,
			helper.getScreenWidth(), helper.getScreenHeight());
		
		//--render to custom frame buffer done

		//-- render to default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenshader.use();
		glBindVertexArray(screenVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, intermediatedTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//-- render to default frame buffer done

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}