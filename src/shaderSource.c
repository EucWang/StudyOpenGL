#ifndef SHADER_SOURCE_H_
#define SHADER_SOURCE_H_

#include "shaders.h"
#include "shaderSource.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LineMaxLen 2048
#define KeyMaxLen 64

struct ShaderProgram* createShaderProgram(const char* vertexPath, const char* fragmentPath) {
	
}

static char* readStrFromFile(const char* filePath) {
	FILE* file = NULL;

	char filebuf[1024 * 16] = { 0 };
	char lineBuf[LineMaxLen];

	file = fopen(filePath, "r");
	if (file == NULL) {
		return NULL;
	}
	char* pTmp;
	List  list;
	list_init(&list, NULL);
	while (!feof(file)) {
		memset(lineBuf, 0, sizeof(lineBuf));  //���buf
		pTmp = fgets(lineBuf, LineMaxLen, file);
		if (pTmp == NULL) {
			break;
		}

		int len = strlen(lineBuf);
		char* str = malloc((len + 1) + sizeof(char));
		memset(str, 0, (len + 1) + sizeof(char));
		strcpy(str, lineBuf);
		list_add(&list, str);
	}

	int listSize = list_size(&list);
	if (listSize <= 0)	{
		return NULL;
	}

	//char* tmp;
	//list_resetIterator(&list);
	//while (list_hasNext(&list)) {
		//list_moveToNext(&list);
		//list_iterator(vertices, (void**)(&tmp));
		//TODO
	//}
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
static int makeShaderProgram(const char* tvertexShaderSource, const char* tfragShaderSource) {
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

#endif // !SHADER_SOURCE_H_
