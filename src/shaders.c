#include "shaders.h"
#include <stdio.h>

GLFWwindow* createGLWindow(int width, int height, const char* title) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	GLFWwindow* window = glfwCreateWindow(width, height,  title, NULL, NULL);
	if (window == NULL) {
		printf("%s\n", "Failed to create GLFW window");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("%s\n", "Failed to initialize GLAD.");
		return NULL;
	}

	//�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, width, height);
	//ע������ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ�������������� 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

//Ƭ����ɫ���������Ǽ�������������ɫ�����
//���� ��ɫ����id�� ���С��0�� Сʱ����ʧ��
int makeFragmentShader(const char* tfragShaderSource) {
	unsigned int fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    //����Ƭ����ɫ��
	glShaderSource(fragmentShader, 1, &tfragShaderSource, NULL);  //����Ƭ����ɫ����Դ��
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

//���봴����ɫ��
//���� ��ɫ����id�� ���С��0�� Сʱ����ʧ��
int makeVertexShader(const char* tvertexShaderSource) {
	unsigned int vertexShader;

	//���룬����������ɫ��
	//����Ҫ��������ɫ�������Բ�����ʽ�ṩ��glCreateShader,
	//�����������ڴ���һ��������ɫ�������ݵĲ�����GL_VERTEX_SHADER��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ�������
	//��һ��������Ҫ�������ɫ������
	//�ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ��
	//�����������Ƕ�����ɫ��������Դ��
	glShaderSource(vertexShader, 1, &tvertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//����ڵ���glCompileShader������Ƿ�ɹ���
	int success;        //�Ƿ����ɹ���״̬
	char infoLog[512];  //������Ϣ������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //��ȡ����֮���״̬

	if (!success) {  //����ʧ��
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  //��ȡ����֮��Ĵ�����Ϣ��־
		printf("%s%s\n", "ERROR:SHADER::VERTEX::COMPILATION_FAILED", infoLog); //����̨���
		return -1;
	}

	return vertexShader;
}

//�����������յ���ɫ���������
//������ɫ���������id�� ���С��0�� Сʱ����ʧ��
int makeShaderProgram(const char * tvertexShaderSource, const char * tfragShaderSource) {
	unsigned int shaderProgram;

	//glCreateProgram��������һ�����򣬲������´�����������ID���á�
	shaderProgram = glCreateProgram();

	int mVertexShader = makeVertexShader(tvertexShaderSource);
	int mFragmentShader = makeFragmentShader(tfragShaderSource);

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
	//glUseProgram(shaderProgram);  

	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);

	return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	//����û��Ƿ����˷��ؼ�(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊ true�ķ����ر�GLFW
		glfwSetWindowShouldClose(window, true);
	}
}
