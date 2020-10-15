#include "NormalMap.h"

static GLuint wallVAO, wallVBO;
static GLuint cubeVAO, cubeVBO;
static int tex_wall, tex_wall_normal;

int NormalMap::practise(const char* projectDir) {
	std::cout << "NormalMap::practise()" << std::endl;

	WindowHelper helper("Normal Mapping", Camera(glm::vec3(0.5f, 0.3f, 2.0f)), 0);
	helper.create();

	MyShader myshader(projectDir, vertFile, fragFile);
	MyShader lightShader(projectDir, vertFileLight, fragFileLight);

	myshader.use();
	myshader.setInt("texture_diffuse1", 0);
	myshader.setInt("normal_map", 1);

	tex_wall = RenderUtil::textureLoad2D(projectDir, imgFileBrickwall, false);
	tex_wall_normal = RenderUtil::textureLoad2D(projectDir, imgFileBrickwallNormal, false);

	RenderUtil::makeVertexArrayAndBuffer(&wallVAO, &wallVBO, wallVertices, sizeof(wallVertices), 8);

	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		//------------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myshader.use();
		glBindVertexArray(wallVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_wall);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_wall_normal);

		glm::mat4 view = helper.getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

		glm::mat4 model(1.0f);

		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		myshader.setMat4("model", model);

		myshader.setVec3("lightPos", lightPos);
		myshader.setVec3("viewPos", helper.getCamera().Position);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//----------------------------
		lightShader.use();
		glBindVertexArray(cubeVAO);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);

		model = glm::mat4(1.0);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.03));
		lightShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		//------------------

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}