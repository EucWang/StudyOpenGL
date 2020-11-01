
#include "PractisePBR.h"
int PractisePBR::practise(const char* projectDir) {
	std::cout << "PractisePBR::practise()" << std::endl;

	helper = new WindowHelper("Practise PBR", Camera(glm::vec3(0.0f, 1.0f, 5.0f)), 0);
	helper->create();

	planeshader = new MyShader(projectDir, vertFile, fragFile);
	planeshader->use();
	planeshader->setInt("texture_diffuse", 0);

	RenderUtil::makeVertexArrayAndBuffer(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	int texPlane = RenderUtil::textureLoad2D(projectDir, imgFilePlane, false);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper->getWindow())) {
		helper->calcProcessInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(helper->getCamera().Zoom),
			helper->getScreenWidth() * 1.0f / helper->getScreenHeight(), 0.1f, 100.0f);
		glm::mat4 view = helper->getCamera().GetViewMatrix();
		glm::mat4 model(1.0f);

		planeshader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texPlane);
		glBindVertexArray(planeVAO);

		planeshader->setMat4("view", view);
		planeshader->setMat4("projection", projection);
		planeshader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(helper->getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}