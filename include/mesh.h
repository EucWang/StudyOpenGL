#ifndef MESH_H_
#define MESH_H_

#include "MyShader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class MyShader;

using namespace std;

struct Vertex {
	glm::vec3 Position;   // ����λ��
	glm::vec3 Normal;     // ��������
	glm::vec2 TexCoords;  // ��������

	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	/**
		�����id
	*/
	unsigned int id;

	/**
		��������ͣ���������������ͼ�����Ǿ������ͼ��
	**/
	string type;

	/* �����ļ���·��*/
	string path;
};


class Mesh
{
public:
	unsigned int VAO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	/*
	* �ڹ������У����ǽ����б�������ݸ���������
	*/
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	/**
	* ����ʹ��Draw��������������
	* ע�����ǽ�һ����ɫ��������Draw�����У�
	* ����ɫ�������������п����������ڻ���֮ǰ����һЩuniform���������Ӳ�����������Ԫ����
	*
	* Ϊ�˽��������⣬������Ҫ�趨һ��������׼��
	*    ÿ����������������Ϊtexture_diffuseN��ÿ�����������Ӧ�ñ�����Ϊtexture_specularN��
	* ����N�ķ�Χ��1����������������������֡�
	* ����˵���Ƕ�ĳһ��������3������������2��������������ǵ����������Ӧ��֮��ᱻ���ã�
	*		uniform sampler2D texture_diffuse1;
	*		uniform sampler2D texture_diffuse2;
	*		uniform sampler2D texture_diffuse3;
	*		uniform sampler2D texture_specular1;
	*		uniform sampler2D texture_specular2;
	*/
	void draw(MyShader* shader);

	//~Mesh();

private:
	/* ��Ⱦ���� */
	unsigned int VBO, EBO;

	/**
	* ��setupMesh�����г�ʼ������
	*/
	void setupMesh();
};
 
#endif // !MESH_H_