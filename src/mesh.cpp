#include "../include/Mesh.h"
#include "../include/MyShader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;
 
/*
* 在构造器中，我们将所有必须的数据赋予了网格
*/
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}


/**
* 最终使用Draw函数来绘制网格。
* 注意我们将一个着色器传入了Draw函数中，
* 将着色器传入网格类中可以让我们在绘制之前设置一些uniform（像是链接采样器到纹理单元）。
*
* 为了解决这个问题，我们需要设定一个命名标准：
*    每个漫反射纹理被命名为texture_diffuseN，每个镜面光纹理应该被命名为texture_specularN，
* 其中N的范围是1到纹理采样器最大允许的数字。
* 比如说我们对某一个网格有3个漫反射纹理，2个镜面光纹理，它们的纹理采样器应该之后会被调用：
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
		glActiveTexture(GL_TEXTURE0 + i);         // 在绑定之前激活相应的纹理单元
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse") {         // 获取纹理序号（diffuse_textureN 中的 N）
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
* 在setupMesh函数中初始化缓冲
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

	//顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//顶点法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//顶点纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	//vertex tangent  切线
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	//vertex bitangent 二重切线/双切线
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

 