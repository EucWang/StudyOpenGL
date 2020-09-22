#pragma once

#include "camera.h"
#include "fileUtil.h"
#include "RenderUtil.h"
#include "MyShader.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class PractiseGeometryShader_4_9 {
public:
	const char* vertFile = "shader/_4_9_geometry_shader.vert";
	const char* fragFile = "shader/_4_9_geometry_shader.frag";
	const char* geomFile = "shader/_4_9_geometry_shader.geom";
	const char* geomFile2 = "shader/_4_9_geometry_shader2.geom";

	const float points[4 * 2] = {
		 -0.5f,  0.5f, // 左上
		 0.5f,  0.5f, // 右上
		 0.5f, -0.5f, // 右下
		-0.5f, -0.5f  // 左下
	};

	//附加上颜色
	const float pointsColor[4 * 3] = {
		1.0f, 0.0f, 0.0f, // 左上
		0.0f, 1.0f, 0.0f, // 右上
		0.0f, 0.0f, 1.0f, // 右下
		1.0f, 1.0f, 0.0f  // 左下
	};


	int practise(string projectDir);
};

