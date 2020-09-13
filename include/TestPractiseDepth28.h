#pragma once

#include "camera.h"
#include "fileUtil.h"
#include "mesh.h"
#include "model.h"
#include "MyShader.h"
#include "shaders.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

class TestPractiseDepth28
{

public:
	const char* vertFile = "shader/shader_depth_test28.vert";
	const char* fragFile = "shader/shader_depth_test28.frag";

	const char* fragFileLight = "shader/shader_depth_test28_light.frag";
	const char* fragFileOutlining = "shader/shader_depth_test28_outlining.frag";

	const char* img1 = "images/marble.jpg";
	//const char* img1 = "images/container2.png";
	const char* img2 = "images/metal.png";

	int practiseDepth28(const char* projectDir);
};

