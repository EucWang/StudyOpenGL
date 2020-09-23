#ifndef  MODEL_H_
#define MODEL_H_

#include "MyShader.h"
#include "Mesh.h"
#include <vector>
#include <assimp/scene.h>
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

using namespace std;
using namespace Assimp;
class MyShader;

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

	Model(string const& path, bool gamma = false);
	Model(const char* parentDir, const char* objFilePath, bool gamma = false);

	void draw(MyShader* shader);

private:

	void loadModel(string const& path);

	void processNode(aiNode* node, const aiScene* scene);

	/// <summary>
	/// ��һ��aiMesh����ת��Ϊ�����Լ��������������ô���ѡ�
	/// ����Ҫ����ֻ�Ƿ��������������Բ������Ǵ��浽�����Լ��Ķ�����
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="scene"></param>
	/// <returns></returns>
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, string typeName);

};

#endif // ! MODEL_H_
