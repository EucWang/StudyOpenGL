#include "../include/MyShader.h"
#include "../include/shaderSource.h"
#include "../include/fileUtil.h"

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

MyShader::MyShader(const GLchar* vertexPath, const GLchar* fragPath) {
	// 1. 从文件路径中获取顶点/片段着色器
	char* vShaderCode;
	char* fShaderCode;
	//读取顶点着色器文件中的文本资源
	if (!readStrFromFile(vertexPath, &vShaderCode)) { return; }
	//读取片段着色器文件中的文本资源
	if (!readStrFromFile(fragPath, &fShaderCode)) { return; }

	id = -1;
	// 2. 编译着色器
	if (!compileShader(vShaderCode, fShaderCode)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
}

MyShader::MyShader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath, const char * geometrySubPath) {
	char* vertexPath;
	char* fragPath;
	char* geomPath;

	if (!getChildPath(&vertexPath, projectDir, vertexSubPath)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
	if (!getChildPath(&fragPath, projectDir, fragmentSubPath)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
	if (!getChildPath(&geomPath, projectDir, geometrySubPath)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
	char* vShaderCode;
	char* fShaderCode;
	char* gShaderCode;
	//读取顶点着色器文件中的文本资源
	if (!readStrFromFile(vertexPath, &vShaderCode)) { return; }
	//读取片段着色器文件中的文本资源
	if (!readStrFromFile(fragPath, &fShaderCode)) { return; }
	//读取几何着色器文件中的文本资源
	if (!readStrFromFile(geomPath, &gShaderCode)) { return; }

	id = -1;
	// 2. 编译着色器
	if (!compileShader(vShaderCode, fShaderCode, gShaderCode)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}

	free(vertexPath);
	free(fragPath);
	free(geomPath);
}

MyShader::MyShader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath) {
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
	char* vShaderCode;
	char* fShaderCode;
	//读取顶点着色器文件中的文本资源
	if (!readStrFromFile(vertexPath, &vShaderCode)) { return; }
	//读取片段着色器文件中的文本资源
	if (!readStrFromFile(fragPath, &fShaderCode)) { return; }
	 
	id = -1;
	// 2. 编译着色器
	if (!compileShader(vShaderCode, fShaderCode)) {
		std::cout << "Shader constructor() failed." << std::endl;
		return;
	}
	
	free(vertexPath);
	free(fragPath);
}

void MyShader::use() {
	glUseProgram(id);
}

// ------------------------------------------------------------------------
void MyShader::setBool(const std::string& name, bool value)  const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void MyShader::setInt(const std::string& name, int value)const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void MyShader::setFloat(const std::string& name, float value)const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void MyShader::setVec3(const char* name, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}
// ------------------------------------------------------------------------
void MyShader::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}
// ------------------------------------------------------------------------
void MyShader::setVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
// ------------------------------------------------------------------------
void MyShader::setVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void MyShader::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void MyShader::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void MyShader::setVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void MyShader::setVec4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void MyShader::setMat2(const std::string& name, const glm::mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void MyShader::setMat3(const std::string& name, const glm::mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void MyShader::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool MyShader::compileShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode) {
	unsigned int vertex, fragment, geometry;

	//定点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (!checkCompileOrLink(vertex, 1)) { return false; }

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (!checkCompileOrLink(fragment, 3)) { return false; }

	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	if (!checkCompileOrLink(geometry, 2)) { return false; }

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glAttachShader(id, geometry);
	glLinkProgram(id);
	if (!checkCompileOrLink(id, 0, false)) { return false; }

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return true;
}

bool MyShader::compileShader(const char* vShaderCode, const char* fShaderCode) {
	unsigned int vertex, fragment;

	//int success;
	//char infoLog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// 打印编译错误（如果有的话）
	//glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(vertex, 512, NULL, infoLog);
	//	std::cout << "ERROR:SHADER:VERTEX:COMPILATION_FAILED." << infoLog << std::endl;
	//	return false;
	//}
	if (!checkCompileOrLink(vertex, 1, true)) { return false; }

	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// 打印编译错误（如果有的话）
	//glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(fragment, 512, NULL, infoLog);
	//	std::cout << "ERROR:SHADER:FRAGMENT:COMPILATION_FAILED." << infoLog << std::endl;
	//	return false;
	//
	//}
	if (!checkCompileOrLink(fragment, 3, true)) { return false; }

	//总的着色器程序, 链接
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	//glGetProgramiv(id, GL_LINK_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(id, 512, NULL, infoLog);
	//	std::cout << "ERROR:SHADER:PROGRAM:LINKING_FAILED." << infoLog << std::endl;
	//	return false;
	//}
	if (!checkCompileOrLink(id, 0, false)) { return false; }

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

/// <summary>
/// 判断对着色器的编译是否成功, 或者检查 着色器和Shader的链接是否成功, 根据isCompile字段来区分
/// </summary>
/// <param name="shaderId">创建的着色器的id 或者就是Shader的id</param>
/// <param name="type">着色器的类型, 1: 定点着色器; 2:几何着色器; 3:片段着色器; 0:Shader的编译</param>
/// <param name="isCompile">true: 表示检查的是着色器的编译; false: 表示检查的是Shader和所有着色器的链接</param>
/// <returns>成功返回true,否则返回false</returns>
bool MyShader::checkCompileOrLink(unsigned int shaderId, int type, bool isCompile) {
	int success;
	char infoLog[512];

	string typeName;
	switch (type) {
	case 0:     typeName = "PROCESS_LINK"; break;
	case 1:		typeName = "VERTEX";	break;
	case 2:		typeName = "FRAGMENT";	break;
	case 3:		typeName = "GEOMETRY";	break;
	default:	break;
	}
	string errorType;
	if(isCompile) {
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		errorType = "COMPILATION_FAILED";
	}	else {
		glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
		errorType = "LINK_FAILED";
	}
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:" << typeName << ":" << errorType 
			<< "\nerror info is : \n" << infoLog << std::endl;
		return false;
	}
	return true;
}

//string MyShader::getCode(const char* vertexPath) {
//
//	std::string vertexCode;          //解析获得的顶点着色器文本代码
//	std::ifstream vShaderFile;
//	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//	try {
//		vShaderFile.open(vertexPath);
//		// 读取文件的缓冲内容到数据流中
//		std::stringstream vShaderStream;
//		vShaderStream << vShaderFile.rdbuf();
//		// 关闭文件处理器
//		vShaderFile.close();
//
//		// 转换数据流到string
//		vertexCode = vShaderStream.str();
//	}
//	catch (std::ifstream::failure e) {
//		std::cout << "ERROR::SHADER::VERTEX_FILE_NOT_SUCCESFULLY_READ" << ", filePath = " << vertexPath << std::endl;
//		return NULL;
//	}
//
//	// 转换成c语言的字符串
//
//	return vertexCode;
//}

//string MyShader::getFragmentCode(const char* fragmentPath) {
//
//	std::string fragmentCode;		//解析获得的颜色着色器文本代码
//	std::ifstream fShaderFile;
//	// 保证ifstream对象可以抛出异常：
//	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//	try {
//		fShaderFile.open(fragmentPath);
//		// 读取文件的缓冲内容到数据流中
//		std::stringstream fShaderStream;
//		fShaderStream << fShaderFile.rdbuf();
//		// 关闭文件处理器
//		fShaderFile.close();
//
//		// 转换数据流到string
//		fragmentCode = fShaderStream.str();
//	}
//	catch (std::ifstream::failure e) {
//		std::cout << "ERROR::SHADER::FRAGMENT_FILE_NOT_SUCCESFULLY_READ" << ", file path = " << fragmentPath << std::endl;
//		return NULL;
//	}
//
//	// 转换成c语言的字符串
//	return fragmentCode;
//}

