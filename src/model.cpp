#include "../include/Model.h"
#include "../include/MyShader.h"
#include "../include/Mesh.h"
#include "../include/stb_image.h"
#include <vector>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Assimp;

/// <summary>
/// 工具方法， 根据directory 和path 从文件系统中加载一个图片纹理，并返回纹理对象的id
/// </summary>
/// <param name="path">const char*</param>
/// <param name="directory">const string &</param>
/// <param name="gamma">bool gamma = false</param>
/// <returns>unsigned int</returns>
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma) {
	string filename = string(path);
	filename = directory + "/" + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
 

Model::Model(string const& path, bool gamma) : gammaCorrection(gamma) {
		loadModel(path);
		std::cout << "Model loadModel() finished()." << std::endl;
		std::cout << "Model meshes size is " << this->meshes.size() << std::endl;
		std::cout << "Model textures size is " << this->textures_loaded.size() << std::endl;
	}

void Model::draw(MyShader* shader) {
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}


void Model::loadModel(string const& path) {
	Importer importer;
	//ReadFile()的第二个参数是一些后期处理(Post-processing)的选项。
	//设定aiProcess_Triangulate，如果模型不是（全部）由三角形组成，它会将模型所有的图元形状变换为三角形。
	//aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标（
	//你可能还记得我们在纹理教程中说过，在OpenGL中大部分的图像的y轴都是反的，所以这个后期处理选项将会修复这个）。
	//其他选项有: 
	//	aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
	//	aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
	//	aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
	// 参考: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)	{
		cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	string dir = path.substr(0, path.find_last_of('/'));
	directory = string(dir);

	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene) {
	//处理结点中所有的网格,如果有的话
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		//我们首先检查每个节点的网格索引，并索引场景的mMeshes数组来获取对应的网格。
		//返回的网格将会传递到processMesh函数中，它会返回一个Mesh对象，我们可以将它存储在meshes列表/vector。
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//然后对其子节点重复这一过程
	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

/// <summary>
/// 将一个aiMesh对象转化为我们自己的网格对象不是那么困难。
/// 我们要做的只是访问网格的相关属性并将它们储存到我们自己的对象中
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
/// <returns></returns>
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Vertex> vectices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// 处理顶点位置、法线和纹理坐标
		aiVector3D aiVector = mesh->mVertices[i];
		glm::vec3 vector;
		vector.x = aiVector.x;
		vector.y = aiVector.y;
		vector.z = aiVector.z;
		vertex.Position = vector;  //顶点

		if (mesh->HasNormals()) {
			aiVector3D aiNormal = mesh->mNormals[i];
			glm::vec3 normal;
			normal.x = aiNormal.x;
			normal.y = aiNormal.y;
			normal.z = aiNormal.z;
			vertex.Normal = normal;
		}

		//  Assimp允许一个模型在一个顶点上有最多8个不同的纹理坐标，我们只关心第一组纹理坐标。
		//  我们同样也想检查网格是否真的包含了纹理坐标（可能并不会一直如此）
		if (mesh->mTextureCoords[0]) { //判断网格是否有纹理坐标
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = tex;

			//tangent
			glm::vec3 tangent;
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			vertex.Tangent = tangent;

			//Bitangent
			glm::vec3 bitangent;
			bitangent.x = mesh->mBitangents[i].x;
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;
			vertex.Bitangent = bitangent;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vectices.push_back(vertex);
	}
	// 处理索引
	// Assimp的接口定义了每个网格都有一个面(Face)数组，每个面代表了一个图元，
	// 在我们的例子中（由于使用了aiProcess_Triangulate选项）它总是三角形。
	// 一个面包含了多个索引，它们定义了在每个图元中，我们应该绘制哪个顶点，并以什么顺序绘制，
	// 所以如果我们遍历了所有的面，并储存了面的索引到indices这个vector中就可以了。
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// 处理材质
	//和节点一样，一个网格只包含了一个指向材质对象的索引。
	//如果想要获取网格真正的材质，我们还需要索引场景的mMaterials数组。
	//网格材质索引位于它的mMaterialIndex属性中，我们同样可以用它来检测一个网格是否包含有材质：
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//我们使用一个叫做loadMaterialTextures的工具函数来从材质中获取纹理。
		//这个函数将会返回一个Texture结构体的vector，我们将在模型的textures vector的尾部之后存储它。
		//一个材质对象的内部对每种纹理类型都存储了一个纹理位置数组。不同的纹理类型都以aiTextureType_为前缀。
		//1. diffuseMap
		vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//2. specularMap
		vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//3. normal map
		vector<Texture> normalMaps = loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//4. height map
		vector<Texture> heightMaps = loadMaterialTexture(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vectices, indices, textures);

}

vector<Texture> Model::loadMaterialTexture(aiMaterial* material, aiTextureType type, string typeName) {
	vector<Texture> textures;

	for (size_t i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		bool skip = false;
		for (size_t j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(str.C_Str(), textures_loaded[j].path.c_str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) { // 如果纹理还没有被加载，则加载它
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // 添加到已加载的纹理中
		}

	}

	return textures;
}
