#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {
public:
	unsigned int id;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec3(const std::string &name, glm::vec3 ) const;

private:
	bool compileShader(const char* vShaderCode, const char* fShaderCode);
	bool getVertexCode(const char **vertexCode, const char * vertexPath);
	bool getFragmentCode(const char** targetFragmentCode, const char* fragmentPath);
};

#endif // !SHADER_H_
