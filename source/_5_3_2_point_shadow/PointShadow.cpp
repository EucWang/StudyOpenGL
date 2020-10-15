#include "PointShadow.h"

static const GLuint SHADOW_WIDTH = 1024;
static const GLuint SHADOW_HEIGHT = 1024;

static GLuint cubeVAO, cubeVBO;

static float near_plane = 1.0f;
static float far_plane = 25.0f;

static int texWood;
static int texWall;

static GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;

/// <summary>
/// 点光阴影
/// </summary>
/// <param name="projectDir"></param>
/// <returns></returns>
int PointShadow::practise(const char * projectDir) {
	std::cout << "PointShadow::practise() running...." << std::endl;

	WindowHelper helper("point shadow", Camera(glm::vec3(0.0f, 0.0f, 3.0f)), 0);
	helper.create();

	//prepare data

	//-------depth cube map and frame buffer
	GLuint depth_cube_map;  //立方体深度贴图
	glGenTextures(1, &depth_cube_map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube_map);
	for (int i = 0; i < 6; i++)	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	GLuint shadow_depth_frame_buffer;  //将立方体深度贴图绑定到帧缓冲上
	glGenFramebuffers(1, &shadow_depth_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_depth_frame_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cube_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//-------depth cube map and frame buffer done

	MyShader shaderDepth(projectDir, vertFileShadow, fragFileShadow, geomFileShadow);
	RenderUtil::makeVertexArrayAndBuffer(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);

	MyShader shaderRender(projectDir, vertFile, fragFile);
	MyShader shaderLight(projectDir, vertFileLight, fragFileLight);

	texWood = RenderUtil::textureLoad2D(projectDir, imgFileWood, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texWall = RenderUtil::textureLoad2D(projectDir, imgFileWall, false);

	shaderRender.use();
	shaderRender.setInt("diffuse_texture", 0);
	shaderRender.setInt("depth_map", 1);

	//prepare data done

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(helper.getWindow())) {
		helper.calcProcessInput();
		//----- render

		lightPos.z = sin(glfwGetTime() * 0.5) * 3.0f;  //move light position over time
	
		//shadow_matrices[6]  lightPos   far_plane  model
			//这里glm::perspective的视野参数，设置为90度。90度我们才能保证视野足够大到可以合适地填满立方体贴图的一个面，
		//立方体贴图的所有面都能与其他面在边缘对齐
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);
		//因为投影矩阵在每个方向上并不会改变，我们可以在6个变换矩阵中重复使用。
		//创建了6个视图矩阵，把它们乘以投影矩阵，来得到6个不同的光空间变换矩阵。glm::lookAt的target参数是它注视的立方体贴图的面的一个方向。
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos,
			lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)));
		
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
 
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadow_depth_frame_buffer);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		//configure shader and matrices
		shaderDepth.use();
		for (int i = 0; i < 6; i++) {
			shaderDepth.setMat4("shadow_matrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		}
		shaderDepth.setFloat("far_plane", far_plane);
		shaderDepth.setVec3("lightPos", lightPos);

		//render scene
		renderScene(shaderDepth);

		//-------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, helper.getScreenWidth(), helper.getScreenHeight());
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(helper.getCamera().Zoom),
			helper.getScreenWidth() * 1.0f / helper.getScreenHeight(), 0.1f, 100.0f);
		glm::mat4 view = helper.getCamera().GetViewMatrix();

		shaderLight.use();
		glBindVertexArray(cubeVAO);
		shaderLight.setMat4("view", view);
		shaderLight.setMat4("projection", projection);
		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.05f));
		shaderLight.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shaderRender.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texWood);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube_map);

		shaderRender.setMat4("view", view);
		shaderRender.setMat4("projection", projection);
		shaderRender.setVec3("lightPos", lightPos);
		shaderRender.setVec3("viewPos", helper.getCamera().Position);
		shaderRender.setFloat("far_plane", far_plane);
		shaderRender.setBool("hasShadow", helper.switchByClickKeyB());

		//render scene
		renderScene(shaderRender, true);

		//----- render done
		glfwSwapBuffers(helper.getWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}


void PointShadow::renderScene(const MyShader& shader, bool scene) {
	glBindVertexArray(cubeVAO);
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(5.0f));

	glDisable(GL_CULL_FACE);
	shader.setBool("reverse_normals", true);
	shader.setMat4("model", model);
	if (scene){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texWall);
	}
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glEnable(GL_CULL_FACE);
	shader.setBool("reverse_normals", false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texWood);
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}