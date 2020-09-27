#pragma once

#include "RenderUtil.h"
#include "WindowHelper.h"
#include "MyShader.h"

#include "camera.h"
#include "fileUtil.h"

#include <string>
#include <iostream>

using namespace std;

class PractiseAdvancedLighting_5_1 {

public:

	const char* vertFile = "shader/_5_1_advanced_lighting.vert";
	const char* fragFile = "shader/_5_1_advanced_lighting.frag";

	const char* vertFilePlane = "shader/_5_1_advanced_lighting_plane.vert";
	const char* fragFilePlane = "shader/_5_1_advanced_lighting_plane.frag";

	const char* fragFileLight = "shader/_5_1_advanced_lighting_light.frag";

	const char* vertFileScreen = "shader/_5_1_advanced_lighting_screen.vert";
	const char* fragFileScreen = "shader/_5_1_advanced_lighting_screen.frag";

	const char* imgFileCube = "images/container2.png";
	const char* imgFileCubeSpecular = "images/container2_specular.png";

	const char* imgFilePlane = "images/wood.png";

    //�ذ�����귨������
    const float planeVertices[8 * 6] = {
        // (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
        //this will cause the floor texture to repeat)
        // positions								// texture Coords
         5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 2.0f
    };

	//���ӵ����귨������
	const float cubeVertices[36 * 8] = {
		// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//��׼����ϵ�£� ��֡�������������ʾ��Ҫ������/����
	const float quadVertices[6 * 4] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	//----------------��ѧʵ���ҳ���------------------------------------------>>>>>>>>>>>>
	//������4������
	const glm::vec3 dirLightArgs[4] = {
		//glm::vec3(0.5f, 0.5f, 0.5f),  //ambient
		//glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
		//glm::vec3(1.0f, 1.0f, 1.0f),     //specular
		glm::vec3(0.1f, 0.1f, 0.1f),  //ambient
		glm::vec3(0.4f, 0.4f, 0.4f),     //diffuse
		glm::vec3(0.5f, 0.5f, 0.5f),     //specular
		glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
	};

	//���Դ��6������
	const glm::vec3 pointLightColors[5] = {
		//glm::vec3(0.4f, 0.7f, 0.1f),  //��һ�����Դ�� (ambient, diffuse, specular)  ����ɫ
		glm::vec3(1.0f, 1.0f, 1.0f),  //��һ�����Դ�� (ambient, diffuse, specular)  ����ɫ
		glm::vec3(0.4f, 0.7f, 0.1f),  //�ڶ������Դ�� (ambient, diffuse, specular)  ��ɫ
		glm::vec3(0.4f, 0.7f, 0.1f),   //���������Դ�� (ambient, diffuse, specular)  ��ɫ
		glm::vec3(0.4f, 0.7f, 0.1f),  //���ĸ����Դ�� (ambient, diffuse, specular)  ��ɫ

		glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
	};

	//�۹��6������
	const glm::vec3 spotlightArgs[4] = {
		glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
		//glm::vec3(0.0f, 1.0f, 0.0f),     //diffuse
		//glm::vec3(0.0f, 1.0f, 0.0f),     //specular
		glm::vec3(0.3f, 0.3f, 0.3f),     //diffuse
		glm::vec3(0.7f, 0.7f, 0.7f),     //specular
		glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
	};

	const glm::vec3 clearColor = glm::vec3(0.9f, 0.9f, 0.9f);   //������ɫ

	const glm::vec2 spotlight_cutoff = glm::vec2(7.0f, 10.0f);  //�۹���нǺ����н�

	const glm::vec3 lightPos = glm::vec3(2.0f, 0.5f, -2.0f);
	//-----------------------------------------------------------------<<<<<<<<<<<<<<

	int practise(const char* projectDir);
};

