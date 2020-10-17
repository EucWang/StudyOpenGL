#include "NormalMap2.h"

int NormalMap2::practise(const char* projectDir) {
	std::cout << "NormalMap2::practise()" << std::endl;

	WindowHelper helper("Normal Mapping 2", Camera(glm::vec3(0.0f, 1.0f, 3.0f)), 0);
	helper.create();
	//-------------------- prepare data

	MyShader myshader(projectDir, vertFile, fragFile);
	myshader.use();
	myshader.setInt("texture_diffuse", 0);
	myshader.setInt("texture_normal", 1);

	//------------------ 手工计算切线和福切线
	glm::vec3 edge1 = pos2 - pos1; 
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent1(0.0f);
	glm::vec3 bitangent1(0.0f);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	//tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	//第二个三角形
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	
	glm::vec3 tangent2(0.0f);
	glm::vec3 bitangent2(0.0f);
	
	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	float quadVertices[] = {
		pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};
	//------------------ 手工计算切线和福切线 done

	GLuint wallVAO, wallVBO;
	//RenderUtil::makeVertexArrayAndBuffer(&wallVAO, &wallVBO, wallVertices, sizeof(wallVertices), 8);
	glGenVertexArrays(1, &wallVAO);
	glGenBuffers(1, &wallVBO);

	glBindVertexArray(wallVAO);
	glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

	int texBrickWall = RenderUtil::textureLoad2D(projectDir, imgFileBrickWall, false);
	int texBrickWallNormal = RenderUtil::textureLoad2D(projectDir, imgFileBrickWallNormal, false);

	GLuint lightVAO, lightVBO;
	RenderUtil::makeVertexArrayAndBuffer(&lightVAO, &lightVBO, cubeVertices, sizeof(cubeVertices), 8);

	MyShader lightShader(projectDir, vertFileLight, fragFileLight);

	//-------------------- prepare data done
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//lightPos.y = cos(glfwGetTime()) * 0.7f;
		//lightPos.z = sin(glfwGetTime()) * 0.7f;

		glm::mat4 view = helper.getCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);
		myshader.use();	
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		myshader.setVec3("viewPos", helper.getCamera().Position);
		myshader.setVec3("lightPos", lightPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texBrickWall);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texBrickWallNormal);
		glBindVertexArray(wallVAO);
		render(myshader);

		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		glBindVertexArray(lightVAO);

		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.02));
		lightShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}

void NormalMap2::render(MyShader &shader) {
	shader.use();

	glm::mat4 model(1.0f);
	//model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// rotate the quad to show normal mapping from multiple directions
	//model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 6);     //第一面墙

	//移动到右边，旋转90度
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);    //第二面墙
	//
	// //移动到左边， 旋转-90度
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);     //第三面墙
}
