#include <../shader.h>
#include <../shaderSource.h>
#include "../include/fileUtil.h"

 
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	const char* vShaderCode;
	const char* fShaderCode;
	const char* vPath = vertexPath;
	const char* fPath = fragmentPath;
	//��ȡ������ɫ���ļ��е��ı���Դ
	if (!getVertexCode(&vShaderCode, vPath)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
	//��ȡƬ����ɫ���ļ��е��ı���Դ
	if (!getFragmentCode(&fShaderCode, fPath)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}

	// 2. ������ɫ��
	if (!compileShader(vShaderCode, fShaderCode)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
}

Shader::Shader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath) {
		char* vertexPath;
		char* fragPath;
		if (!getChildPath(&vertexPath, projectDir, vertexSubPath)) {
			std::cout << "Shader constructor() failed." << std::endl;
			return;
		}
		if (!getChildPath(&fragPath, projectDir, fragmentSubPath)) {
			std::cout << "Shader constructor() failed." << std::endl;
			return;
		}
		const char* vShaderCode;
		const char* fShaderCode;
		//��ȡ������ɫ���ļ��е��ı���Դ
		if (!getVertexCode(&vShaderCode, vertexPath)) {
			std::cout << "Shader constructor() failed." << std::endl;
			return;
		}
		//��ȡƬ����ɫ���ļ��е��ı���Դ
		if (!getFragmentCode(&fShaderCode, fragPath)) {
			std::cout << "Shader constructor() failed." << std::endl;
			return;
		}

		// 2. ������ɫ��
		if (!compileShader(vShaderCode, fShaderCode)) {
			std::cout << "Shader constructor() failed." << std::endl;
			return;
		}

		free(vertexPath);
		free(fragPath);
	}

void Shader::use() {
		glUseProgram(id);
	}

void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}

void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
	
void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}
	
void Shader::setVec3(const std::string& name, glm::vec3 value) const {
		glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
	}
 
bool Shader::getFragmentCode(const char** targetFragmentCode, const char* fragmentPath) {

	std::string fragmentCode;		//������õ���ɫ��ɫ���ı�����
	std::ifstream fShaderFile;
	// ��֤ifstream��������׳��쳣��
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		fShaderFile.open(fragmentPath);
		// ��ȡ�ļ��Ļ������ݵ���������
		std::stringstream vShaderStream, fShaderStream;
		fShaderStream << fShaderStream.rdbuf();
		// �ر��ļ�������
		fShaderFile.close();

		// ת����������string
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FRAGMENT_FILE_NOT_SUCCESFULLY_READ" << ", file path = " << fragmentPath << std::endl;
		return false;
	}

	// ת����c���Ե��ַ���
	const char* fShaderCode = fragmentCode.c_str();
	*targetFragmentCode = fShaderCode;
	return true;
}

bool Shader::getVertexCode(const char** targetVertexCode, const char* vertexPath) {

	std::string vertexCode;          //������õĶ�����ɫ���ı�����
	std::ifstream vShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		// ��ȡ�ļ��Ļ������ݵ���������
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();

		// ת����������string
		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::VERTEX_FILE_NOT_SUCCESFULLY_READ" << ", filePath = " << vertexPath << std::endl;
		return false;
	}

	// ת����c���Ե��ַ���
	const char* vShaderCode = vertexCode.c_str();
	*targetVertexCode = vShaderCode;
	return true;
}

bool Shader::compileShader(const char * vShaderCode, const char * fShaderCode) {
	unsigned int vertex, fragment;

	int success;
	char infoLog[512];

	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// ��ӡ�����������еĻ���
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:VERTEX:COMPILATION_FAILED." << infoLog << std::endl;
		return false;
	}

	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// ��ӡ�����������еĻ���
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:FRAGMENT:COMPILATION_FAILED." << infoLog << std::endl;
		return false;
	}

	//�ܵ���ɫ������, ����
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:PROGRAM:LINKING_FAILED." << infoLog << std::endl;
		return false;
	}

	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}
