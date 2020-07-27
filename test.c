#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//����ͬ��Ҳϣ���ܹ���GLFW��ʵ��һЩ������ƣ������ͨ��ʹ��GLFW�ļ������뺯������ɡ�
//���ǽ���ʹ��GLFW��glfwGetKey����������Ҫһ�������Լ�һ��������Ϊ���롣
void processInput(GLFWwindow* window);

//ѭ���е��Զ�����Ⱦ
void render(GLFWwindow * window);

//���봴��������ɫ��
int makeVertexShader();

//���봴��Ƭ����ɫ��
int makeFragmentShader();

int linkVertexAttribute();


//��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
//���Ҫʹ�øղű������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������Ȼ������Ⱦ�����ʱ�򼤻������ɫ������
//�Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ�á�
int makeShaderProgram();

//��ʽ��ͼ
int someOpenGlFunctionThatDrawsOurTriangle();


unsigned int VBO;
unsigned int VAO;

int main() {
	//printf("%s\n", "Hello glad.");
	//return 0;

	glfwInit();  //��ʼ��GLFW

	//ʹ��glfwWindowHint����������GLFW
	// glfwWindowHint�����ĵ�һ����������ѡ������ƣ�
	// ���ǿ��ԴӺܶ���GLFW_��ͷ��ö��ֵ��ѡ��
	// �ڶ�����������һ�����ͣ������������ѡ���ֵ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //���ǽ����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// ������ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile),
	// ��ȷ����GLFW������Ҫʹ�ú���ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ���û�������Ѳ�����Ҫ�����������ԣ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//���ʹ�õ���Mac OS Xϵͳ���㻹��Ҫ���������д��뵽��ĳ�ʼ����������Щ���ò���������
#ifdef __APPLE__
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ����������
	// ������������ʾ������ڵ����ƣ����⣩��
	// ��������ʹ��"LearnOpenGL"����Ȼ��Ҳ����ʹ����ϲ�������ơ�
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("%s\n", "Failed to create GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("%s\n", "Failed to initialize GLAD");
		return -1;
	}

	//�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, 800, 600);
	//ע������ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ��������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int shaderProgram = makeShaderProgram();
	if (shaderProgram < 0) {
		return -1;
	}
	linkVertexAttribute(shaderProgram);


	//��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ�������С�
	//glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	while (!glfwWindowShouldClose(window)){
		//����
		processInput(window);

		// ��Ⱦָ��
		//render(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //������
		glClear(GL_COLOR_BUFFER_BIT);   

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��
		// ������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(window);


		//glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬��
		//�����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	//����û��Ƿ����˷��ؼ�(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
		//ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊ true�ķ����ر�GLFW
		glfwSetWindowShouldClose(window, true); 
	}
}


//���Ǳ�������Ⱦǰָ��OpenGL����ν��Ͷ������ݡ�
//������ɫ����������ָ���κ��Զ�������Ϊ��ʽ�����롣
//��ʹ����к�ǿ������Ե�ͬʱ��������ȷ��ζ�����Ǳ����ֶ�ָ���������ݵ���һ�����ֶ�Ӧ������ɫ������һ���������ԡ�
//λ�����ݱ�����Ϊ32λ��4�ֽڣ�����ֵ��
//ÿ��λ�ð���3��������ֵ��
//����3��ֵ֮��û�п�϶��������ֵ�����⼸��ֵ�������н�������(Tightly Packed)��
//�����е�һ��ֵ�ڻ��忪ʼ��λ�á�
int linkVertexAttribute(unsigned int shaderProgram) {

	//ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ���
	//��һ������ָ������Ҫ���õĶ������ԡ�
	//���ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0��
	//��Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
	//�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	//����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
	//���ĸ������Ƿ�ϣ�����ݱ���׼��(Normalize)�������ΪGL_TRUE�����ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
	//�������������(Stride)����ʾ�������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
    //Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶��
	//��������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
	//һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ����������ӣ���ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
	//������������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//���������Ѿ�������OpenGL����ν��Ͷ������ݣ�
	//��������Ӧ��ʹ��glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	//��������Ĭ���ǽ��õġ�


	//���ж������Ѿ����ú��ˣ�
	//����ʹ��һ�����㻺����󽫶������ݳ�ʼ���������У�������һ�������һ��Ƭ����ɫ����
	//��������OpenGL��ΰѶ����������ӵ�������ɫ���Ķ��������ϡ�
	//��OpenGL�л���һ�����壬���������������

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);  //ʹ��glGenBuffers������һ������ID����һ��VBO����

	//0 ���ƶ������鵽�����й�Openglʹ��
	glBindVertexArray(VAO);

	//ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	//����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//����glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	//glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	//��һ��������Ŀ�껺�������,���㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ����
	//�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	//����������������ϣ�����͵�ʵ�����ݡ�
	//���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
	//GL_STATIC_DRAW �����ݲ���򼸺�����ı�;GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶ�;GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���������Ѿ��Ѷ������ݴ������Կ����ڴ��У���VBO������㻺��������

	 
	//1 ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//2. ��������Ⱦһ������ʱҪʹ����ɫ������
	//glUseProgram(shaderProgram);

	//3. ��������
	//someOpenGlFunctionThatDrawsOurTriangle();

	return 1;
}

//�����������յ���ɫ���������
//������ɫ���������id�� ���С��0�� Сʱ����ʧ��
int makeShaderProgram() {
	unsigned int shaderProgram;

	//glCreateProgram��������һ�����򣬲������´�����������ID���á�
	shaderProgram = glCreateProgram();

	int mVertexShader = makeVertexShader();
	int mFragmentShader = makeFragmentShader();

	if (mVertexShader < 0 || mFragmentShader < 0) {
		return -1;
	}

	//��֮ǰ�������ɫ�����ӵ����������
	glAttachShader(shaderProgram, mVertexShader);
	glAttachShader(shaderProgram, mFragmentShader);

	//��glLinkProgram��������
	glLinkProgram(shaderProgram);


	//����ڵ���glLinkProgram������Ƿ�ɹ���
	int success;        //�Ƿ����ɹ���״̬
	char infoLog[512];  //������Ϣ������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //��ȡ����֮���״̬
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s%s\n", "ERROR:SHADER::PROGRAM::LINK_FAILED", infoLog); //����̨���
		return -1;
	}

	//����glUseProgram�������øմ����ĳ��������Ϊ���Ĳ������Լ�������������
	//��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������Ҳ����֮ǰд����ɫ��)�ˡ�
	glUseProgram(shaderProgram);  

	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);

	return shaderProgram;
}


//���봴����ɫ��
//���� ��ɫ����id�� ���С��0�� Сʱ����ʧ��
int makeVertexShader() {
	unsigned int vertexShader;

	char* vertexShaderSource = "#version 330 core\n"   //�汾���� OpenGl3.3 core����ģʽ
		"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) �趨�����������λ��ֵ��in �ؼ��֣� vec3 ����3d���ꣻ aPos ���������
		"void main() {\n"                                  //������C���Ե�main����
		"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position����  Ԥ����ı�����vec4���͵ģ� ��main�������Ĭ�Ͻ�gl_Position��ֵ��Ϊ������ɫ�������
		"}";

	//���룬����������ɫ��
	//����Ҫ��������ɫ�������Բ�����ʽ�ṩ��glCreateShader,
	//�����������ڴ���һ��������ɫ�������ݵĲ�����GL_VERTEX_SHADER��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ�������
	//��һ��������Ҫ�������ɫ������
	//�ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ��
	//�����������Ƕ�����ɫ��������Դ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//����ڵ���glCompileShader������Ƿ�ɹ���
	int success;        //�Ƿ����ɹ���״̬
	char infoLog[512];  //������Ϣ������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //��ȡ����֮���״̬

	if (!success){  //����ʧ��
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  //��ȡ����֮��Ĵ�����Ϣ��־
		printf("%s%s\n", "ERROR:SHADER::VERTEX::COMPILATION_FAILED", infoLog); //����̨���
		return -1;
	}

	return vertexShader;
}

//Ƭ����ɫ���������Ǽ�������������ɫ�����
//���� ��ɫ����id�� ���С��0�� Сʱ����ʧ��
int makeFragmentShader() {
	unsigned int fragmentShader; 
	
	char* fragmentShaderSource = "#version 330 core\n"     //�汾���� OpenGl3.3 core����ģʽ
		"out vec4 FragColor;"            //Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ��
		"void main(){"                     //������C���Ե�main����
		"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"   //���ǽ�һ��alphaֵΪ1.0(1.0������ȫ��͸��)���ٻ�ɫ��vec4��ֵ����ɫ�����
		"}";

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    //����Ƭ����ɫ��
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  //����Ƭ����ɫ����Դ��
	glCompileShader(fragmentShader);                      //����Ƭ����ɫ��

	//����ڵ���glCompileShader������Ƿ�ɹ���
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("%s%s\n", "ERROR:SHADER:FRAGMENT:COMPILATION_FAILED", infoLog);
		return -1;
	}
	return fragmentShader;
}