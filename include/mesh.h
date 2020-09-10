#ifndef MESH_H_
#define MESH_H_

#include "MyShader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class MyShader;

using namespace std;

struct Vertex {
	glm::vec3 Position;   // 世界位置
	glm::vec3 Normal;     // 法线向量
	glm::vec2 TexCoords;  // 纹理坐标

	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	/**
		纹理的id
	*/
	unsigned int id;

	/**
		纹理的类型，比如是漫反射贴图或者是镜面光贴图。
	**/
	string type;

	/* 纹理文件的路径*/
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
	* 在构造器中，我们将所有必须的数据赋予了网格
	*/
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

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
	void draw(MyShader* shader);

	//~Mesh();

private:
	/* 渲染数据 */
	unsigned int VBO, EBO;

	/**
	* 在setupMesh函数中初始化缓冲
	*/
	void setupMesh();
};
 
#endif // !MESH_H_