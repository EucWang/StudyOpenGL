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
		 -0.5f,  0.5f, // ����
		 0.5f,  0.5f, // ����
		 0.5f, -0.5f, // ����
		-0.5f, -0.5f  // ����
	};

	//��������ɫ
	const float pointsColor[4 * 3] = {
		1.0f, 0.0f, 0.0f, // ����
		0.0f, 1.0f, 0.0f, // ����
		0.0f, 0.0f, 1.0f, // ����
		1.0f, 1.0f, 0.0f  // ����
	};


	int practise(string projectDir);
};

