#include "Bloom.h"



int Bloom::practise(const char* projectDir) {

	std::cout << "Bloom::practise£¨£©" << std::endl;

	WindowHelper helper("Bloom", Camera(glm::vec3(0.0f, 0.0f, 5.0f)), 0);
	helper.create();

	MyShader planeShader(projectDir, vertFile, fragFile);
	planeShader.use();
	planeShader.setInt("texture_diffuse", 0);

	int texWood = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);

	GLuint planeVAO, planeVBO;
	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();
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
		planeShader.setMat4("model", model);

		planeShader.setVec3("viewPos", helper.getCamera().Position);
		planeShader.setVec3("lightPos", lightPos);
		planeShader.setVec3("lightColor", lightColor);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//-----render done --------
		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}