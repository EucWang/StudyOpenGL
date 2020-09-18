#pragma once

#include "fileUtil.h"
//#include "shaders.h"
#include "RenderUtil.h"
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
/// ��ϰ ���� ��������ͼ
/// </summary>
class PractiseCubeMap_4_6 {

public:
	const char* vertFile = "shader/shader_cube_map_test_4_6.vert";
	const char* fragFile = "shader/shader_cube_map_test_4_6.frag";

	const char* vertFileCube = "shader/shader_cube_map_test_4_6_box.vert";
	const char* fragFileCube = "shader/shader_cube_map_test_4_6_box.frag";

	const char* vertFileReflect = "shader/shader_cube_map_test_4_6_box_reflect.vert";
	const char* fragFileReflect = "shader/shader_cube_map_test_4_6_box_reflect.frag";

	//��֡���������д�뵽��Ļʱ,ʹ�����µ���ɫ��
	const char* vertFileScreen = "shader/shader_cube_map_test_4_6_framebuffer.vert";
	const char* fragFileScreen = "shader/shader_cube_map_test_4_6_framebuffer.frag";

	const char* imageFileContainer = "images/container.jpg";

	struct SkyBoxData skybox;

	int practise(const char* projectDir);
};

