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
/// ���߷����� ����directory ��path ���ļ�ϵͳ�м���һ��ͼƬ������������������id
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
	//ReadFile()�ĵڶ���������һЩ���ڴ���(Post-processing)��ѡ�
	//�趨aiProcess_Triangulate�����ģ�Ͳ��ǣ�ȫ��������������ɣ����Ὣģ�����е�ͼԪ��״�任Ϊ�����Ρ�
	//aiProcess_FlipUVs���ڴ����ʱ��תy����������꣨
	//����ܻ��ǵ�����������̳���˵������OpenGL�д󲿷ֵ�ͼ���y�ᶼ�Ƿ��ģ�����������ڴ���ѡ����޸��������
	//����ѡ����: 
	//	aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
	//	aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�ֻ����Ⱦ��С��������ô����ǳ����á�
	//	aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���
	// �ο�: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
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
	//�����������е�����,����еĻ�
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		//�������ȼ��ÿ���ڵ������������������������mMeshes��������ȡ��Ӧ������
		//���ص����񽫻ᴫ�ݵ�processMesh�����У����᷵��һ��Mesh�������ǿ��Խ����洢��meshes�б�/vector��
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//Ȼ������ӽڵ��ظ���һ����
	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

/// <summary>
/// ��һ��aiMesh����ת��Ϊ�����Լ��������������ô���ѡ�
/// ����Ҫ����ֻ�Ƿ��������������Բ������Ǵ��浽�����Լ��Ķ�����
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
		// ������λ�á����ߺ���������
		aiVector3D aiVector = mesh->mVertices[i];
		glm::vec3 vector;
		vector.x = aiVector.x;
		vector.y = aiVector.y;
		vector.z = aiVector.z;
		vertex.Position = vector;  //����

		if (mesh->HasNormals()) {
			aiVector3D aiNormal = mesh->mNormals[i];
			glm::vec3 normal;
			normal.x = aiNormal.x;
			normal.y = aiNormal.y;
			normal.z = aiNormal.z;
			vertex.Normal = normal;
		}

		//  Assimp����һ��ģ����һ�������������8����ͬ���������꣬����ֻ���ĵ�һ���������ꡣ
		//  ����ͬ��Ҳ���������Ƿ���İ������������꣨���ܲ�����һֱ��ˣ�
		if (mesh->mTextureCoords[0]) { //�ж������Ƿ�����������
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
	// ��������
	// Assimp�Ľӿڶ�����ÿ��������һ����(Face)���飬ÿ���������һ��ͼԪ��
	// �����ǵ������У�����ʹ����aiProcess_Triangulateѡ������������Ρ�
	// һ��������˶�����������Ƕ�������ÿ��ͼԪ�У�����Ӧ�û����ĸ����㣬����ʲô˳����ƣ�
	// ����������Ǳ��������е��棬�����������������indices���vector�оͿ����ˡ�
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// �������
	//�ͽڵ�һ����һ������ֻ������һ��ָ����ʶ����������
	//�����Ҫ��ȡ���������Ĳ��ʣ����ǻ���Ҫ����������mMaterials���顣
	//�����������λ������mMaterialIndex�����У�����ͬ���������������һ�������Ƿ�����в��ʣ�
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//����ʹ��һ������loadMaterialTextures�Ĺ��ߺ������Ӳ����л�ȡ����
		//����������᷵��һ��Texture�ṹ���vector�����ǽ���ģ�͵�textures vector��β��֮��洢����
		//һ�����ʶ�����ڲ���ÿ���������Ͷ��洢��һ������λ�����顣��ͬ���������Ͷ���aiTextureType_Ϊǰ׺��
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

		if (!skip) { // �������û�б����أ��������
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
		}

	}

	return textures;
}
