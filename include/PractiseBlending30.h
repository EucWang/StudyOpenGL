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

class PractiseBlending30{

public:
	const char* vertFile = "shader/shader_blending_test30.vert";
	const char* fragFile = "shader/shader_blending_test30.frag";

	const char* imageFileCube = "images/marble.jpg";
	const char* imageFileFloor = "images/metal.png";
	const char* imageFileTransparentRed = "images/blending_transparent_window.png";

	const char* objFileBackpack = "images/backpack/backpack.obj";

	int practise(const char * projectDir);
};

