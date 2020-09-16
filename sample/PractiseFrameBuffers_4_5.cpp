#include "PractiseFrameBuffers_4_5.h"
 

static double deltaTime;
static float lastFrame;
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

static void make_texture_attach(unsigned int &tex);

int PractiseFrameBuffers_4_5::practise(const char * projectDir) {
	std::cout << "PractiseFrameBuffers_4_5.practise()" << std::endl;

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int texture;
	make_texture_attach(&texture);
	//Ȼ�����������õ������ӵ�֡������, ��͸�������ɫ����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		texture, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH,
		SMALL_SCREEN_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);


	//����Ƿ�֡��������
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAMEBUFFER:ERROR:NOT_COMPLETE" << std::endl;
		return -1;
	}


	//����Ĭ�ϵĴ��ڻ���, �Ա�������������������Ⱦ
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//������е�֡�������֮��, ɾ����֡����
	glDeleteFramebuffers(1, &fbo);
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
void make_texture_attach(unsigned int * texture) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	//��Ҫ��������ǣ����ǽ�ά������Ϊ����Ļ��С�������ⲻ�Ǳ���ģ����������Ǹ������data����������NULL��
	//��������������ǽ����������ڴ��û�����������������������������Ⱦ��֡����֮�������С�
	//ͬ��ע�����ǲ������Ļ��Ʒ�ʽ��༶��Զ���������ڴ��������¶�������Ҫ���ǡ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << "make_texture_attach : texture = " << *texture << std::endl;
}