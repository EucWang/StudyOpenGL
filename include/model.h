#pragma once


#include <vector>
#include <assimp/scene.h>
#include "shader.h"
#include "mesh.h"

using namespace std;

/// <summary>
/// ���߷����� ����directory ��path ���ļ�ϵͳ�м���һ��ͼƬ������������������id
/// </summary>
/// <param name="path">const char*</param>
/// <param name="directory">const string &</param>
/// <param name="gamma">bool gamma = false</param>
/// <returns>unsigned int</returns>
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {

public:

	/*�Ѿ����ع���������, ����Ҫ�����µ�����ʱ,�����ڴ˼����б��������Ƿ��Ѿ����ع���ͬ��*/
	vector<Texture> textures_loaded;

	/*  ģ������ , һ��Mesh�����vector */
	vector<Mesh> meshes;

	/*һ���ļ�·��*/
	string directory;

	bool gammaCorrection;

	Model(string const & path, bool gamma = false) : gammaCorrection(gamma){
		loadModel(path);
	}
	void draw(Shader &shader);

private:

	void loadModel(string const & path);

	void processNode(aiNode* node, const aiScene* scene);

	/// <summary>
	/// ��һ��aiMesh����ת��Ϊ�����Լ��������������ô���ѡ�
	/// ����Ҫ����ֻ�Ƿ��������������Բ������Ǵ��浽�����Լ��Ķ�����
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="scene"></param>
	/// <returns></returns>
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, string typeName);

};