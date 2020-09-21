#ifndef MYSHADER_H_
#define MYSHADER_H_

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

#include "shaderSource.h"
#include "fileUtil.h"

class MyShader {

public:
	unsigned int id;
	
	MyShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	MyShader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath);

	void use();
	void deleteProgram() { glDeleteProgram(id); id = -1; }
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value)  const;
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value)const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value)const;
	// ------------------------------------------------------------------------
	void setVec3(const char* name, glm::vec3 value) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, glm::vec3 value) const;
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const;
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	bool compileShader(const char* vShaderCode, const char* fShaderCode);

	//string getVertexCode(const char* vertexPath);
	//string getFragmentCode(const char* fragmentPath);
};

#endif // !SHADER_H_

