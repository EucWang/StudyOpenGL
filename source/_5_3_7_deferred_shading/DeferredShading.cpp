#include "DeferredShading.h"


int DeferredShading::practise(const char* projectDir) {
	std::cout << "DeferredShading::practise()" << std::endl;

	WindowHelper helper("Deferred Shading", Camera(glm::vec3(0.0f, 0.0f, 3.0f)), 0);
	helper.create();

	MyShader shader(projectDir, vertFile, fragFile);
	shader.use();
	shader.setInt("texture_diffuse", 0);

	GLuint VAO, VBO;
	GLuint tex;

	RenderUtil::makeVertexArrayAndBuffer(&VAO, &VBO, planeVertices, sizeof(planeVertices), 8);
	tex = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glm::mat4 view = helper.getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

		glm::mat4 model(1.0f);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}