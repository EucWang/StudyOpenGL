#include "../include/shaders.h"
#include "../include/shaderSource.h"
#include "../include/list.h"
#include "../include/fileUtil.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
* @param vertexPath       ������ɫ�����ļ�·��
* @param fragmentPath  Ƭ�β�ֵ�����ļ�·��
* @shaderId  : int *  ���������
* @return bool  �Ƿ񴴽��ɹ�
*/
bool createShaderProgram(char* vertexPath, char* fragmentPath, int * shaderId) {
	char* vertexStr;
	char* fragStr;
	bool readVertexSuccess = readStrFromFile(vertexPath, &vertexStr);
	bool readFragSuccess = readStrFromFile(fragmentPath, &fragStr);
	if (!readVertexSuccess && !readFragSuccess) {
		return false;
	}

	if (vertexStr == NULL) { printf("%s%s\n", "failed to read from vertexPath file, file path is ", vertexPath);	}

	if (fragStr == NULL) { printf("%s%s\n", "failed to read from fragmentPath file, file path is ", fragmentPath);	}

	int tShader = makeShaderProgram(vertexStr, fragStr);
	if (tShader < 0) { return false;	}

	*shaderId = tShader;

	return true;
}


//Ƭ����ɫ���������Ǽ�������������ɫ�����
//���� ��ɫ����id�� ���С��0�� Сʱ����ʧ��
static int makeFragmentShader(const char* tfragShaderSource) {
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
/**
* @param tvertexShaderSource  ��������� ������ɫ���ĳ�����ַ���
* @param tfragShaderSource  ��������� Ƭ����ɫ���ĳ����ַ���
* @return int  ���ش�����shaderProgram��idֵ
*/
int makeShaderProgram(const char* tvertexShaderSource, const char* tfragShaderSource) {
	if (tvertexShaderSource == NULL && tfragShaderSource == NULL)	{ return -1; }
	
	unsigned int shaderProgram;
	int mVertexShader = -1; 
	int mFragmentShader = -1;

	//glCreateProgram��������һ�����򣬲������´�����������ID���á�
	shaderProgram = glCreateProgram();
	if (tvertexShaderSource != NULL){
		mVertexShader = makeVertexShader(tvertexShaderSource);
		if (mVertexShader < 0) { return -1;	 }
		//��֮ǰ�������ɫ�����ӵ����������
		glAttachShader(shaderProgram, mVertexShader);
	}

	if (tfragShaderSource != NULL)	{
		mFragmentShader = makeFragmentShader(tfragShaderSource);
		if (mFragmentShader < 0) { return -1; }
		//��֮ǰ�������ɫ�����ӵ����������
		glAttachShader(shaderProgram, mFragmentShader);
	}

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

	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	if (mVertexShader != -1)	{ glDeleteShader(mVertexShader);	}
	if (mFragmentShader != -1){ glDeleteShader(mFragmentShader); }

	return shaderProgram;
}