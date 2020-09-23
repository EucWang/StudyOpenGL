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
/// 工具方法， 根据directory 和path 从文件系统中加载一个图片纹理，并返回纹理对象的id
/// </summary>
/// <param name="path">const char*</param>
/// <param name="directory">const string &</param>
/// <param name="gamma">bool gamma = false</param>
/// <returns>unsigned int</returns>
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {
public:

	/*已经加载过的纹理集合, 但需要加载新的纹理时,首先在此集合中遍历查找是否已经加载过相同的*/
	vector<Texture> textures_loaded;

	/*  模型数据 , 一个Mesh对象的vector */
	vector<Mesh> meshes;

	/*一个文件路径*/
	string directory;

	bool gammaCorrection;

	Model(string const& path, bool gamma = false);
	Model(const char* parentDir, const char* objFilePath, bool gamma = false);

	void draw(MyShader* shader);

private:

	void loadModel(string const& path);

	void processNode(aiNode* node, const aiScene* scene);

	/// <summary>
	/// 将一个aiMesh对象转化为我们自己的网格对象不是那么困难。
	/// 我们要做的只是访问网格的相关属性并将它们储存到我们自己的对象中
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="scene"></param>
	/// <returns></returns>
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, string typeName);

};

#endif // ! MODEL_H_
