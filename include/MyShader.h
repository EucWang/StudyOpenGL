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
	
	/// <summary>
	/// 构建shader, 有2个着色器: 顶点着色器, 片段着色器
	/// </summary>
	/// <param name="vertexSubPath">顶点着色器的全路径</param>
	/// <param name="fragmentSubPath">片段着色器的全路径</param>
	MyShader(const GLchar* vertexPath, const GLchar* fragmentPath);

	/// <summary>
	/// 构建shader, 有2个着色器: 顶点着色器, 片段着色器
	/// </summary>
	/// <param name="projectDir">着色器所在的父级目录, 2个着色器都在这个目录下</param>
	/// <param name="vertexSubPath">顶点着色器的子路径</param>
	/// <param name="fragmentSubPath">片段着色器的子路径</param>
	MyShader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath);

	/// <summary>
	/// 构建shader, 有3个着色器: 顶点着色器, 片段着色器, 几何着色器
	/// </summary>
	/// <param name="projectDir">着色器所在的父级目录, 3个着色器都在这个目录下</param>
	/// <param name="vertexSubPath">顶点着色器的子路径</param>
	/// <param name="fragmentSubPath">片段着色器的子路径</param>
	/// <param name="geometrySubPath">几何着色器的子路径</param>
	MyShader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath, const char* geometrySubPath);

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
	void setVec3(const std::string& name, glm::vec3 &value) const;
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

	/// <summary>
	///
	/// </summary>
	/// <param name="vShaderCode"></param>
	/// <param name="fShaderCode"></param>
	/// <param name="gShaderCode"></param>
	/// <returns></returns>
	bool compileShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);

	/// <summary>
	/// 判断对着色器的编译是否成功 或者检查 着色器和Shader的链接是否成功, 根据isCompile字段来区分
	/// </summary>
	/// <param name="shaderId">创建的着色器的id</param>
	/// <param name="type">着色器的类型, 1: 定点着色器; 2:几何着色器; 3:片段着色器</param>
	/// <param name="isCompile">true: 表示检查的是着色器的编译; false: 表示检查的是Shader和所有着色器的链接</param>
	/// <returns>成功返回true,否则返回false</returns>
	bool checkCompileOrLink(unsigned int shaderId, int type, bool isCompile=true);
	//string getVertexCode(const char* vertexPath);
	//string getFragmentCode(const char* fragmentPath);
};

#endif // !SHADER_H_

