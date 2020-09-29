#include "PractiseGammaCorrection_5_2_2.h"

int PractiseGammaCorrection_5_2_2::practise(const char * projectDir) {
	
	WindowHelper helper("gamma correction sample2", Camera(glm::vec3(0.0f, 3.0f, 5.0f)), 0);
	helper.create();

	MyShader lightshader(projectDir, vertFileLight, fragFileLight);
	GLuint lightVAO, lightVBO;
	RenderUtil::makeVertexArrayAndBuffer(&lightVAO, &lightVBO, cubeVertices, sizeof(cubeVertices), 8);

	MyShader shader(projectDir, vertFile, fragFile);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	int texWood = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);
	int texWoodSpecular = RenderUtil::textureLoad2D(projectDir, imgFileWoodSpecular, false);
	int texWoodGammaCorrected = RenderUtil::textureLoad2D(projectDir, imgFileWood, true);

	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUniformBlockBinding(shader.id, glGetUniformBlockIndex(shader.id, "Matrices"), 0);
	glUniformBlockBinding(lightshader.id, glGetUniformBlockIndex(lightshader.id, "Matrices"), 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

	shader.use();
	shader.setInt("floorTexture", 0);
	shader.setInt("floorTextureSpecular", 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();
	
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 projection(1.0f);
		glm::mat4 view(1.0f);
		view = helper.getCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		shader.use();
		glBindVertexArray(planeVAO);
		bool gammaEnable = helper.switchByClickKeyB();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gammaEnable ? texWoodGammaCorrected : texWood);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texWoodSpecular);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.004f, 0.0f));
		shader.setMat4("model", model);

		shader.setVec3("viewPos", helper.getCamera().Position);
		shader.setBool("gamma", gammaEnable);
		
		shader.setVec3("attenuation_args", glm::vec3(1.0f, 0.14f, 0.07f));

		glUniform3fv(glGetUniformLocation(shader.id, "lightPosition"), 4, &lightPositions[0][0]);
		glUniform3fv(glGetUniformLocation(shader.id, "lightColors"), 4, &lightColors[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//--------------------
		lightshader.use();
		glBindVertexArray(lightVAO);

		for (int i = 0; i < 4; i++) {
			glm::mat4 model3(1.0f);
			model3 = glm::translate(model3, lightPositions[i]);
			model3 = glm::scale(model3, glm::vec3(0.1f, 0.1f, 0.1f));
			lightshader.setMat4("model", model3);
			lightshader.setVec3("lightColor", glm::vec3(1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}


	glfwTerminate();
	return 1;
}