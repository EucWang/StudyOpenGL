#include "../include/Mesh.h"
#include "../include/MyShader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;
 
/*
* �ڹ������У����ǽ����б�������ݸ���������
*/
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}


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
void Mesh::draw(MyShader* shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (size_t i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);         // �ڰ�֮ǰ������Ӧ������Ԫ
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse") {         // ��ȡ������ţ�diffuse_textureN �е� N��
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++);
		}

		//shader.setFloat(("material." + name + number).c_str(), i);
		glUniform1i(glGetUniformLocation(shader->id, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

/**
* ��setupMesh�����г�ʼ������
*/
void Mesh::setupMesh() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//����λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//���㷨��
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//������������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	//vertex tangent  ����
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	//vertex bitangent ��������/˫����
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

 