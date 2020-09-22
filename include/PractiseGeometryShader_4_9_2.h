#pragma once

#include <string>
#include <iostream>

#include "camera.h"
#include "fileUtil.h"

#include "model.h"
#include "mesh.h"
#include "MyShader.h"

#include "RenderUtil.h"

class PractiseGeometryShader_4_9_2 {
public:

	const char* vertFile = "shader/_4_9_2_geometry_shader.vert";
	const char* fragFile = "shader/_4_9_2_geometry_shader.frag";
	const char* geomFile = "shader/_4_9_2_geometry_shader.geom";


	const char* modelFile = "images/nanosuit/nanosuit.obj";
	
	//���׷��ڶ���shader
	const char* vertFile2 = "shader/_4_9_2_geometry_shader3.vert";
	const char* fragFile2 = "shader/_4_9_2_geometry_shader3.frag";
	const char* geomFile2 = "shader/_4_9_2_geometry_shader3.geom";


	const char* vertFilePlane = "shader/_4_9_2_geometry_shader2.vert";
	const char* fragFilePlane = "shader/_4_9_2_geometry_shader2.frag";


	const char* planeFile = "images/metal.png";

	//���������
	const float planePosition[6 * 3] = {
		// (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
		//this will cause the floor texture to repeat)
		// positions        
		 5.0f, -0.5f,  5.0f,
		-5.0f, -0.5f,  5.0f,
		-5.0f, -0.5f, -5.0f,

		 5.0f, -0.5f,  5.0f,
		-5.0f, -0.5f, -5.0f,
		 5.0f, -0.5f, -5.0f
	};
	//����ķ���
	const float planeNormal[6 * 3] = {
		0.0f, 1.0f, 0.0f,  //���ǳ��ϵ�
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	
	};

	//�������������
	const float planeTexCoords[6 * 2] = {
		// (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
		//this will cause the floor texture to repeat)
		// texture Coords
		 2.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 2.0f,

		 2.0f, 0.0f,
		 0.0f, 2.0f,
		 2.0f, 2.0f
	};


	int practise(string projectDir);
};

