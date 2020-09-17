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

class PractiseFrameBuffer_kernel_4_5 {
public:
	PractiseFrameBuffer_kernel_4_5() {}
	 
	const char* vertFileScreen = "shader/shader_frame_buffer_kernel_test_4_5.vert";
	const char* fragFileScreen = "shader/shader_frame_buffer_kernel_test_4_5.frag";
	const char* fragFileScreen2 = "shader/shader_frame_buffer_kernel_handle_test_4_5.frag";

	const char* vertFileFrame = "shader/shader_frame_buffer_kernel_frame_test_4_5.vert";
	const char* fragFileFrame = "shader/shader_frame_buffer_kernel_frame_test_4_5.frag";

	const char* vertFileGrass = "shader/shader_blending_test29_grass.vert";
	const char* fragFileGrass = "shader/shader_blending_test29_grass.frag";

	const char* imageFileContainer = "images/container.jpg";
	const char* imageFilePlane = "images/metal.png";

	const char* imageFileGlass = "images/blending_transparent_window.png";
	const char* imageFileGrass = "images/grass.png";

	int practise(const char* projectDir);
};

