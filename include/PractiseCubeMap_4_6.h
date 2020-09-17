#pragma once

#include "fileUtil.h"
#include "shaders.h"
#include "stb_image.h"
#include "util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MyShader.h"
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"

#include "skyBoxData.h"

#include <iostream>
using namespace std;

/// <summary>
/// 练习 创建 立方体贴图
/// </summary>
class PractiseCubeMap_4_6 {

public:
	const char* vertFile = "shader/shader_cube_map_test_4_6.vert";
	const char* fragFile = "shader/shader_cube_map_test_4_6.frag";

	struct SkyBoxData skybox;

	int practise(const char* projectDir);
};

