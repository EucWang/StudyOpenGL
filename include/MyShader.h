#ifndef MYSHADER_H_
#define MYSHADER_H_

#include <glad/glad.h> // ����glad����ȡ���еı���OpenGLͷ�ļ�
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
	/// ����shader, ��2����ɫ��: ������ɫ��, Ƭ����ɫ��
	/// </summary>
	/// <param name="vertexSubPath">������ɫ����ȫ·��</param>
	/// <param name="fragmentSubPath">Ƭ����ɫ����ȫ·��</param>
	MyShader(const GLchar* vertexPath, const GLchar* fragmentPath);

	/// <summary>
	/// ����shader, ��2����ɫ��: ������ɫ��, Ƭ����ɫ��
	/// </summary>
	/// <param name="projectDir">��ɫ�����ڵĸ���Ŀ¼, 2����ɫ���������Ŀ¼��</param>
	/// <param name="vertexSubPath">������ɫ������·��</param>
	/// <param name="fragmentSubPath">Ƭ����ɫ������·��</param>
	MyShader(const char* projectDir, const char* vertexSubPath, const char* fragmentSubPath);

	/// <summary>
	/// ����shader, ��3����ɫ��: ������ɫ��, Ƭ����ɫ��, ������ɫ��
	/// </summary>
	/// <param name="projectDir">��ɫ�����ڵĸ���Ŀ¼, 3����ɫ���������Ŀ¼��</param>
	/// <param name="vertexSubPath">������ɫ������·��</param>
	/// <param name="fragmentSubPath">Ƭ����ɫ������·��</param>
	/// <param name="geometrySubPath">������ɫ������·��</param>
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
	/// �ж϶���ɫ���ı����Ƿ�ɹ� ���߼�� ��ɫ����Shader�������Ƿ�ɹ�, ����isCompile�ֶ�������
	/// </summary>
	/// <param name="shaderId">��������ɫ����id</param>
	/// <param name="type">��ɫ��������, 1: ������ɫ��; 2:������ɫ��; 3:Ƭ����ɫ��</param>
	/// <param name="isCompile">true: ��ʾ��������ɫ���ı���; false: ��ʾ������Shader��������ɫ��������</param>
	/// <returns>�ɹ�����true,���򷵻�false</returns>
	bool checkCompileOrLink(unsigned int shaderId, int type, bool isCompile=true);
	//string getVertexCode(const char* vertexPath);
	//string getFragmentCode(const char* fragmentPath);
};

#endif // !SHADER_H_

