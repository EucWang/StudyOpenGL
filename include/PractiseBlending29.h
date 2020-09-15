#pragma once

#include "camera.h"
#include "fileUtil.h"
#include "MyShader.h"
#include "shaders.h"
#include "stb_image.h"
#include "util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PractiseBlending29 {
public:

	const char* vertFile = "shader/shader_blending_test29.vert";
	const char* fragFile = "shader/shader_blending_test29.frag";

	const char* fragFileLight = "shader/shader_blending_test29_light.frag";
	const char* fragFileOutlining = "shader/shader_blending_test29_outlining.frag";

	const char* vertFileGrass = "shader/shader_blending_test29_grass.vert";
	const char* fragFileGrass = "shader/shader_blending_test29_grass.frag";

	const char* img1 = "images/marble.jpg";
	//const char* img1 = "images/container2.png";
	const char* img2 = "images/metal.png";

	const char* img3 = "images/grass.png";

	int practiseDepth29(const char* projectDir);
};

