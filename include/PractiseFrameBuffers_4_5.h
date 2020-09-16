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

class PractiseFrameBuffers_4_5 {
public:
	PractiseFrameBuffers_4_5() {}

	const char* vertFile = "shader/shader_frame_buffer_test_4_5.vert";
	const char* fragFile = "shader/shader_frame_buffer_test_4_5.frag";

	const char* vertFileScreen = "shader/shader_frame_buffer_test_4_5_screen.vert";
	const char* fragFileScreen = "shader/shader_frame_buffer_test_4_5_screen.frag";

	const char* fragFileReverseColor = "shader/shader_frame_buffer_test_4_5_reverse_color.frag";  //反转颜色

	const char* fragFileGray = "shader/shader_frame_buffer_test_4_5_gray.frag";  //灰度

	const char* imgContainer = "images/container.jpg";
	const char* imgPlane = "images/metal.png";

	int practise(const char * projectDir);
};