#pragma once

#include <string>
#include <iostream>

#include "camera.h"
#include "fileUtil.h"
#include "mesh.h"
#include "MyShader.h"
#include "model.h"
#include "RenderUtil.h"

class PractiseAdvancedGLSL_4_8
{
public:

	const char* vertFile = "shader/_4_8_advanced_glsl.vert";
	const char* fragFile = "shader/_4_8_advanced_glsl.frag";

	const float somePosition[12] = {
		// Top face
		-0.7f,  0.7f, -0.7f,   //0
		 0.7f,  0.7f, -0.7f,   //1

		 0.7f,  0.7f,  0.7f,   //2
		-0.7f,  0.7f,  0.7f,   //3
	};

	//---------��cubeVertices ���Ϊ3�����飬�ֱ�Ϊ λ�ã����ߣ���������-------------
	const float cubePosition[36 * 3] = {
		// Back face
		-0.5f, -0.5f, -0.5f, // Bottom-left
		 0.5f,  0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // bottom-right         
		 0.5f,  0.5f, -0.5f, // top-right
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f,  0.5f, -0.5f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-right
		 0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f,  0.5f, // top-left
		-0.5f, -0.5f,  0.5f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f,  0.5f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right         
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f, -0.5f, -0.5f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f, // top-left
		 0.5f,  0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right     
		 0.5f,  0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f,  0.5f,  0.5f, // bottom-left        
	};

	int practise(string projectDir);
};

