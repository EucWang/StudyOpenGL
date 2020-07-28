#include "shaders.h"
#include "test.h"
#include <stdio.h>

const char* vertexShaderSource3 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

static unsigned int VBO, VAO, EBO;

static void prepareData();

static void render(int shaderProgram);

static void onDestroy(int shaderProgram);

int drawUnfillRectangle() {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "DRAW UNFILL RECTANGLE", NULL, NULL);
	if (window == NULL)	{
		printf("%s\n", "create glfwWindow failed. exit");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		printf("%s\n", "Failed to Initialize glad.");
		return -1;
	}

	int shaderProgram = makeShaderProgram(vertexShaderSource3, fragmentShaderSource3);
	if (shaderProgram < 0)	{
		return -1;
	}

	prepareData();

	while (!glfwWindowShouldClose(window))	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	onDestroy(shaderProgram);
	glfwTerminate();

	return 1;
}


void prepareData() {
	float vertices[] = {
		0.5f, 0.5f, 0.0f,  //top right
		0.5f, -0.5f, 0.0f,   //bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f, 0.5f, 0.0f  //top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	//一个顶点数组对象会储存以下这些内容：
	//  glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
	//	通过glVertexAttribPointer设置的顶点属性配置。
	//	通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//先绑定VAO， 从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//链接顶点属性
	//使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的。
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  //允许解绑VBO， 但是不要解绑EBO
	glBindVertexArray(0);       //解绑VAO，上面配置的VBO，EBO都间接的绑定到VAO中了，绘制时再重新绑定即可

	//线框模式(Wireframe Mode)
	//第一个参数表示我们打算将其应用到所有的三角形的正面和背面，第二个参数告诉我们用线来绘制。
	//后的绘制调用会一直以线框模式绘制三角形，
	//直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式。
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void render(int shaderProgram) {

	glUseProgram(shaderProgram);  //使用着色器程序
	glBindVertexArray(VAO);      //重新绑定VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //画三角形
}

void onDestroy(int shaderProgram) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}