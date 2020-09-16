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

#include <iostream>
using namespace std;

class PractiseFaceCulling_4_4 {

public:
	const char* vertFile = "shader/shader_faceculling_test4_4.vert";
	const char* fragFile = "shader/shader_faceculling_test4_4.frag";

	const char* grassVertFile = "shader/shader_faceculling_test4_4_grass.vert";
	const char* grassFragFile = "shader/shader_faceculling_test4_4_grass.frag";

	const char* imgFileCube = "images/marble.jpg";
	const char* imgFilePlane = "images/metal.png";
	const char* imgFileGrass = "images/grass.png";

	int practise(const char * projectDir);
};

