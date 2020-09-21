#include "PractiseFrameBuffers_4_5.h"
 
static double deltaTime;
static double lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static float cubeVertices[] = {
	// Back face
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // Bottom-left
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f, // bottom-right         
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f, // top-left
	// Front face
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top-right
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top-right
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f, // top-left
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom-left
	// Left face
	-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		 1.0f, 0.0f, // top-right
	-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-right
	// Right face
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-left
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // top-right         
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-left
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f, // top-left
	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, // top-right
	// Top face
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // top-left
	 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f, // top-right     
	 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // bottom-right
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // top-left
	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f  // bottom-left        
};

static float quadVertices[] = {
	// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
};

static float planeVertices[] = {
	// (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
	//this will cause the floor texture to repeat)
	// positions          // texture Coords
	 5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 2.0f
};

static Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));

static void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

static void mouse_move_callback(GLFWwindow* window, double posX, double posY) {
	if (isMouseFirstIn) {
		isMouseFirstIn = false;
		lastX = posX;
		lastY = posY;
	}

	double offsetX = posX - lastX;
	double offsetY = lastY - posY;

	lastX = posX;
	lastY = posY;

	camera.ProcessMouseMovement(offsetX, offsetY);
}

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.ProcessMouseScroll(offsetY);
}

static void make_texture_attach(unsigned int  * tex, int index = 0);

static void make_render_buffer_obj_attach(unsigned int  * rbo);

int PractiseFrameBuffers_4_5::practise(const char * projectDir) {
	std::cout << "PractiseFrameBuffers_4_5.practise()" << std::endl;
	//---------------------------------------------------------------------
	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "practise frame buffer");
	if (window == NULL) { return -1; }

	MyShader shader(projectDir, vertFile, fragFile);  //��֡����д���shader
	//MyShader screenshader(projectDir, vertFileScreen, fragFileScreen);   //��ͨ��û�д������Ļshader
	//MyShader screenReverseColorShader(projectDir, vertFileScreen, fragFileReverseColor);   //��ת��ɫ����Ļ��shader
	MyShader screenGrayShader(projectDir, vertFileScreen, fragFileGray);   //����ɫ��λ�Ҷȵ���Ļshader

	GLuint quadVAO, quadVBO;
	makeVAOVBO(&quadVAO, &quadVBO, quadVertices, sizeof(quadVertices), 4);
	GLuint cubeVAO, cubeVBO;
	makeVAOVBO(&cubeVAO, &cubeVBO, cubeVertices, sizeof(cubeVertices), 8);
	GLuint planeVAO, planeVBO;
	makeVAOVBO(&planeVAO, &planeVBO, planeVertices, sizeof(planeVertices), 8);

	int cubTexture;
	if ((cubTexture = textureLoad(projectDir, imgContainer)) <= 0) { return -1; }
	int planeTexture;
	if ((planeTexture = textureLoad(projectDir, imgPlane)) <= 0) { return -1; }

	shader.use();
	shader.setInt("texture1", 0);
	//screenshader.use();
	//screenshader.setInt("screenTexture", 0);
	//screenReverseColorShader.use();
	//screenReverseColorShader.setInt("screenTexture", 0);
	screenGrayShader.use();
	screenGrayShader.setInt("screenTexture", 0);

	//---------------------------------------------------------------------
	unsigned int framebuffer;   //һ��֡�������
	glGenFramebuffers(1, &framebuffer);  //����һ��֡�������
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  //��֡������� �� GL_FRAMEBUFFER
	unsigned int texColorBuffer;  //һ���յ�����ͼ��
	make_texture_attach(&texColorBuffer);
	//���Ӵ���Ⱥ�ģ�建�������
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH,
	//	SMALL_SCREEN_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
	//	GL_TEXTURE_2D, texture, 0);
	
	unsigned int rbo;//��Ⱦ������󸽼�
	make_render_buffer_obj_attach(&rbo);
	//���֡�����Ƿ�����
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
		return -1;
	}
	//���֡����, ����Ĭ�ϵĴ��ڻ���, �Ա�������������������Ⱦ
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//---------------------------------------------------------------------

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		double curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		processInput(window);

		//--------------------------render to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom),
			SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		//��Ⱦ2������
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubTexture);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, (float)curFrame * glm::radians(10.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, 1.0f));
		model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(34.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		shader.setMat4("model", model2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(1.0f, 0.0f, -1.0f));
		model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(12.0f), glm::vec3(0.0f, 1.2f, 0.0f));
		shader.setMat4("model", model2);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//��Ⱦ�ذ�
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ������Ȼ���, ��Ϊ��Ļ�Ǳ�׼�Ķ�ά����,����Ҫ��Ȼ���
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  //���֡�����

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //�����Ļ��ɫ
		glClear(GL_COLOR_BUFFER_BIT);         //�����ɫ����
		//--------------------------render to framebuffer done

		//------------------render framebuffer to default screen 
		//screenshader.use();
		//screenReverseColorShader.use();
		screenGrayShader.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);  //��֡�������ɵ��������texColorBufferд�뵽��Ļ֡������
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//------------------render framebuffer to default screen  done

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//---------------------------------------------------------------------
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	//������е�֡�������֮��, ɾ����֡����
	glDeleteFramebuffers(1, &framebuffer);
	glfwTerminate();
	//---------------------------------------------------------------------
}

/// <summary>
/// 
/// �����ɫ����/��Ȼ���,��ɫ����Attachment
/// ������һ���ڴ�λ�ã����ܹ���Ϊ֡�����һ�����壬���Խ�������Ϊһ��ͼ�񡣵�����һ��������ʱ������������ѡ�
/// �������Ⱦ�������(Renderbuffer Object)��
///
/// ������,����һ�������ӵ�֡�����ʱ�����е���Ⱦָ���д�뵽��������У���������һ����ͨ����ɫ/��Ȼ�ģ�建��һ����
///	ʹ��������ŵ��ǣ�������Ⱦ�����Ľ�����ᱻ������һ������ͼ���У�����֮���������ɫ���кܷ����ʹ������
/// </summary>
/// <param name="texture"> out </param>
void make_texture_attach(unsigned int * texColorBuffer, int index) {
	glGenTextures(1, texColorBuffer);   
	glBindTexture(GL_TEXTURE_2D, *texColorBuffer);
	//��Ҫ��������ǣ����ǽ�ά������Ϊ����Ļ��С�������ⲻ�Ǳ���ģ����������Ǹ������data����������NULL��
	//��������������ǽ����������ڴ��û�����������������������������Ⱦ��֡����֮�������С�
	//ͬ��ע�����ǲ������Ļ��Ʒ�ʽ��༶��Զ���������ڴ��������¶�������Ҫ���ǡ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);  //��հ�

	//Ȼ�����������õ������ӵ�֡������, ��͸�������ɫ����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D,
		*texColorBuffer, 0);

	std::cout << "make_texture_attach : texture = " << *texColorBuffer << std::endl;
}

/// <summary>
/// ����һ����Ⱦ�������, ����Ⱥ�ģ�建���
/// </summary>
/// <param name="rbo"></param>
void make_render_buffer_obj_attach(unsigned int * rbo) {
	glGenRenderbuffers(1, rbo);   //������Ⱦ�������
	glBindRenderbuffer(GL_RENDERBUFFER, *rbo);  //����Ⱦ�������

	//����һ����Ⱥ�ģ����Ⱦ�������
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH, SMALL_SCREEN_WIDTH);

	glBindRenderbuffer(GL_RENDERBUFFER, 0); //�������

	//���������Ⱦ�������
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *rbo);

}