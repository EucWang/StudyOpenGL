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

class PractiseFrameBuffers_4_5
{

public:
	PractiseFrameBuffers_4_5() {}

	const char* vertFile = "shader/shader_frame_buffer.vert";
	const char* vertFile = "shader/shader_frame_buffer.frag";

	int practise(const char * projectDir);
};