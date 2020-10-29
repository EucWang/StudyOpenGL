#include "DeferredShading.h"


static const float linear = 0.7;
static const float quadratic = 1.8;


int DeferredShading::practise(const char* projectDir) {
	std::cout << "DeferredShading::practise()" << std::endl;
	WindowHelper helper("Deferred Shading", Camera(glm::vec3(1.0f, 1.5f, 8.0f)), 0);
	helper.create();

	MyShader quadShader(projectDir, vertFileQuad, fragFileQuad);
	quadShader.use();
	quadShader.setInt("texture_diffuse", 0);

	GLuint quadVAO, quadVBO;
	RenderUtil::makeVertexArrayAndBuffer(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 5);

	MyShader gbufferShader(projectDir, vertFileGBuffer, fragFileGBuffer);
	gbufferShader.use();
	//gbufferShader.setInt("texture_diffuse1", 0);
	//gbufferShader.setInt("texture_specular1", 1);

	//MyShader shader(projectDir, vertFile, fragFile);
	//shader.use();
	//shader.setInt("texture_diffuse", 0);

	MyShader gbufferShaderLighting(projectDir, vertFileGBufferShading, fragFileGBufferShading);
	gbufferShaderLighting.use();
	gbufferShaderLighting.setInt("gPosition", 0);
	gbufferShaderLighting.setInt("gNormal", 1);
	gbufferShaderLighting.setInt("gAlbedoSpec", 2);

	// lighting info
   // -------------
	const unsigned int NR_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}


	GLuint VAO, VBO;
	GLuint tex;

	Model nanoModel(projectDir, modelFileNano);

	RenderUtil::makeVertexArrayAndBuffer(&VAO, &VBO, planeVertices, sizeof(planeVertices), 8);
	tex = RenderUtil::textureLoad2D(projectDir, imgFileWood, false);

	GLuint gFrameBuffer;
	GLuint gTexture[3];  //gPosition, gNormal, gColorSpec;
	glGenFramebuffers(1, &gFrameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	glGenTextures(4, gTexture);
	for (size_t i = 0; i < 3; i++){
		glBindTexture(GL_TEXTURE_2D, gTexture[i]);
		//我们使用RGB纹理来储存位置和法线的数据，因为每个对象只有三个分量；
		//但是我们将颜色和镜面强度数据合并到一起，存储到一个单独的RGBA纹理里面，这样我们就不需要声明一个额外的颜色缓冲纹理了。
		GLuint format1 = 0; 
		if (i < 2) { format1 = GL_RGBA16F; 
		} else { format1 = GL_RGBA; }
		glTexImage2D(GL_TEXTURE_2D, 0, format1, helper.getScreenWidth(), helper.getScreenHeight(), 0,
			GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gTexture[i], 0);
	}

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	GLuint gRenderBuffer;  //帧缓冲的渲染缓冲
	glGenRenderbuffers(1, &gRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, helper.getScreenWidth(), helper.getScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gRenderBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
			glm::mat4 view = helper.getCamera().GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
				helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);

			glm::mat4 model(1.0f);

			gbufferShader.use();
			gbufferShader.setMat4("view", view);
			gbufferShader.setMat4("projection", projection);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) { 
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(-3.0f + 3.0f * j, -0.5f, -3.0f * i));
					model = glm::scale(model, glm::vec3(0.2f));
					gbufferShader.setMat4("model", model);
					nanoModel.draw(&gbufferShader);
				}
			}
		 
			//------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		gbufferShaderLighting.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gTexture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gTexture[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gTexture[2]);
		for (unsigned int i = 0; i < lightPositions.size(); i++) {
			gbufferShaderLighting.setVec3("lights[" + std::to_string(i) + "].pos", lightPositions[i]);
			gbufferShaderLighting.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
			// update attenuation parameters and calculate radius
			gbufferShaderLighting.setFloat("lights[" + std::to_string(i) + "].linear", linear);
			gbufferShaderLighting.setFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);
		}
		gbufferShaderLighting.setVec3("viewPos", helper.getCamera().Position);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, helper.getScreenWidth(), helper.getScreenHeight(),
			0, 0, helper.getScreenWidth(), helper.getScreenHeight(),
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDisable(GL_DEPTH_TEST);
		//
		//glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//
		//quadShader.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, gTexture[2]);
		//glBindVertexArray(quadVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}