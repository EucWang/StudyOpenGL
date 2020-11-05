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
		myshader->use();
		glm::mat4 view = helper->getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(
									glm::radians(helper->getCamera().Zoom),
			helper->getScreenWidth() * 1.0f/ helper->getScreenHeight(), 0.1f, 100.0f);

		myshader->setMat4("view", view);
		myshader->setMat4("projection", projection);

		glm::mat4 model(1.0f);
		for(int row=0; row < nrRows; row ++) {
			myshader->setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumn; col++) {
				float tRoughness = glm::clamp((float)col / (float)nrColumn, 0.05f, 1.0f);
				myshader->setFloat("roughness", tRoughness);

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumn / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f));
				myshader->setMat4("model", model);
				glBindVertexArray(sphereVAO);
				glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
			}
		}

		for (unsigned int i = 0; i < sizeof(lightPos) / sizeof(lightPos[0]); i++) {
			myshader->setVec3("lightPos[" + std::to_string(i) + "]", lightPos[i]);
			myshader->setVec3("lightColor[" + std::to_string(i) + "]", lightColor[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			myshader->setMat4("model", model);
			glBindVertexArray(sphereVAO);
			glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
		}
		
		//----------------------------------------------------

		glfwSwapBuffers(helper->getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}

void PractisePBRLighting2::createSphereData() {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
			
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
		
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; y++) {
		if (!oddRow) {
			for (unsigned int x = 0; x <= X_SEGMENTS; x++) {
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else {
			for (int x = X_SEGMENTS; x >= 0; --x) {
				indices.push_back((y+1)*(X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	sphereIndexCount = indices.size();

	std::vector<float> data;
	for (unsigned int i = 0; i < positions.size(); ++i) {
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);

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

	glGenVertexArrays(1, &sphereVAO);
	unsigned int vbo, ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],GL_STATIC_DRAW);

	float stride = 8 * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
}