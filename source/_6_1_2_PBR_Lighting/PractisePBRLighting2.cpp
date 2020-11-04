#include "PractisePBRLighting2.h"


int PractisePBRLighting2::practise(const char * projectDir) {
	std::cout << "PractisePBRLighting2::practise()" << std::endl;

	helper = new WindowHelper("Practise PBR 2",
		Camera(glm::vec3(0.0f, 1.0f, 5.0f)), 0);
	helper->create();

	myshader = new MyShader(projectDir, vertFile, fragFile);

	int textAlbedo = RenderUtil::textureLoad2D(projectDir, imgFileAlbedo, false);
	int texAo = RenderUtil::textureLoad2D(projectDir, imgFileAo, false);
	int texMetallic = RenderUtil::textureLoad2D(projectDir, imgFileMetallic, false);
	int texRoughness = RenderUtil::textureLoad2D(projectDir, imgFileRoughness, false);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper->getWindow())) {
		helper->calcProcessInput();
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//----------------------------------------------------
		//----------------------------------------------------

		glfwSwapBuffers(helper->getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}