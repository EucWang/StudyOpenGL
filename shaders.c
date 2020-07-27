#include "shaders.h"
#include <stdio.h>

//Ƭ����ɫ���������Ǽ�������������ɫ�����
//���� ��ɫ����id�� ���С��0�� Сʱ����ʧ��
int makeFragmentShader(char* fragShaderSource) {
	unsigned int fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    //����Ƭ����ɫ��
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);  //����Ƭ����ɫ����Դ��
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
int makeVertexShader(char* vertexShaderSource) {
	unsigned int vertexShader;

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

	if (!success) {  //����ʧ��
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  //��ȡ����֮��Ĵ�����Ϣ��־
		printf("%s%s\n", "ERROR:SHADER::VERTEX::COMPILATION_FAILED", infoLog); //����̨���
		return -1;
	}

	return vertexShader;
}


//�����������յ���ɫ���������
//������ɫ���������id�� ���С��0�� Сʱ����ʧ��
int makeShaderProgram(char * vertexShaderSource, char * fragShaderSource) {
	unsigned int shaderProgram;

	//glCreateProgram��������һ�����򣬲������´�����������ID���á�
	shaderProgram = glCreateProgram();

	int mVertexShader = makeVertexShader(vertexShaderSource);
	int mFragmentShader = makeFragmentShader(fragShaderSource);

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
