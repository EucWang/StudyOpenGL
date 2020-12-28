
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
	RenderUtil::glCheckError();
	std::cout << "glCheckError() done." << std::endl;
	while (!glfwWindowShouldClose(helper->getWindow())) {
		helper->calcProcessInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(helper->getCamera().Zoom),
			helper->getScreenWidth() * 1.0f / helper->getScreenHeight(), 0.1f, 100.0f);
		glm::mat4 view = helper->getCamera().GetViewMatrix();
		glm::mat4 model(1.0f);

		planeshader->use();

		planeshader->setMat4("view", view);
		planeshader->setMat4("projection", projection);
		planeshader->setVec3("cameraPos", helper->getCamera().Position);
		planeshader->setVec3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		planeshader->setFloat("ao", 1.0f);
		planeshader->setBool("useGamma", helper->switchByClickKeyB());

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texPlane);
		//glBindVertexArray(planeVAO);
		//planeshader->setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//------------------------------------------------------

		for (int row = 0; row < nrRows; row++) {
			planeshader->setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumn; col++) {
				float param = (float)col / (float)nrColumn;
				planeshader->setFloat("roughness", glm::clamp(param, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumn / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				planeshader->setMat4("model", model);
				renderSphere();
			}
		}

		//光线
		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		//for (unsigned int i = 0; i < sizeof(lightPos) / sizeof(lightPos[0]); i++) {
		for (unsigned int i = 0; i < 4; i++) {
			//glm::vec3 newPos = lightPos[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			glm::vec3 newPos = lightPos[i];
			planeshader->setVec3("lightPos[" + std::to_string(i) + "]", newPos);

			planeshader->setVec3("lightColor[" + std::to_string(i) + "]", lightColor[i]);
		
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			planeshader->setMat4("model", model);
			renderSphere();
		}


		glfwSwapBuffers(helper->getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}


void PractisePBR::renderSphere() {

	if (sphereVAO == 0) {
		glGenVertexArrays(1, &sphereVAO);
		glGenBuffers(1, &sphereVBO);
		glGenBuffers(1, &sphereEBO);

		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<unsigned int> indices;

		//遍历生成圆球的球面的点,法线, 纹理 数据
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
				float xSegment = (float)x / X_SEGMENTS;
				float ySegment = (float)y / Y_SEGMENTS;

				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				pos.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;  //奇数行
		// 对点进行排列
		for (unsigned int y = 0; y < Y_SEGMENTS; y++) {
			if (!oddRow) {
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			} else {
				for (int x = X_SEGMENTS; x >= 0; --x) {
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back( y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		//把数据综合到一起
		std::vector<float> data;
		for (unsigned int i = 0; i < pos.size(); i++) {
			data.push_back(pos[i].x);
			data.push_back(pos[i].y);
			data.push_back(pos[i].z);
			if (normals.size() > 0) {
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0) {
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}

		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		float stride = (3 + 3 + 2) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
		
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}