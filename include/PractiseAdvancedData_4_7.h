#pragma once

#include "camera.h"
#include "fileUtil.h"
#include "mesh.h"
#include "model.h"
#include "MyShader.h"
#include "RenderUtil.h"

#include <string>
#include <iostream>


using namespace std;

class PractiseAdvancedData_4_7
{

public:

	string vertFile = "shader/vert_4_7_advanced_data.vert";
	string fragFile = "shader/frag_4_7_advanced_data.frag";

    string fragFileFloor = "shader/frag_4_7_advanced_data_floor.frag";

    string fragFileLight = "shader/shader_blending_test29_light.frag";

	const char* imgFileContainer = "images/container2.png";
	const char* imgFileContainerSpecular = "images/container2_specular.png";


    const char* imgFilePlane = "images/metal.png";

    //----------------ɳĮ����------------------------------
    //������4������
    const glm::vec3 dirLightArgs[4] = {
        glm::vec3(0.3f, 0.24f, 0.14f),  //ambient
        glm::vec3(0.7f, 0.42f, 0.26f),     //diffuse
        glm::vec3(0.5f, 0.5f, 0.5f),     //specular
        glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
    };

    //���Դ��6������
    const glm::vec3 pointLightColors[5] = {
        glm::vec3(1.0f, 0.6f, 0.0f),  //��һ�����Դ�� (ambient, diffuse, specular)  ����ɫ
        glm::vec3(1.0f, 0.0f, 0.0f),  //�ڶ������Դ�� (ambient, diffuse, specular)  ��ɫ
        glm::vec3(1.0f, 1.0, 0.0),    //���������Դ�� (ambient, diffuse, specular)  ��ɫ
        glm::vec3(0.2f, 0.2f, 1.0f),  //���ĸ����Դ�� (ambient, diffuse, specular)  ��ɫ

        glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
    };

    //�۹��6������
    const glm::vec3 spotlightArgs[4] = {
        glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
        glm::vec3(0.8f, 0.8f, 0.8f),     //diffuse
        glm::vec3(0.8f, 0.8f, 0.8f),     //specular
        glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
    };

    const glm::vec3 clearColor = glm::vec3(0.75f, 0.52f, 0.3f);   //������ɫ

    const glm::vec2 spotlight_cutoff = glm::vec2(12.5f, 13.0f);  //�۹���нǺ����н�

    const glm::vec3 lightPos = glm::vec3(0.0f, 2.0f, 1.0f);  //�̶��Ĺ�Դλ��

    //--------------------------------------------------------------------------//

    //�ذ�����
    const float planeVertices[8 * 6] = {
        // (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
        //this will cause the floor texture to repeat)
        // positions          // texture Coords
         5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f,  2.0f, 2.0f
    };

	//��������/����/����
	const float cubeVertices[36 * 8] = {
		// Back face
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		 1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f  // bottom-left        
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

	const float cubeNormal[36 * 3] = {

		0.0f, 0.0f, -1.0f,// Bottom-left
		0.0f, 0.0f, -1.0f,// top-right
		0.0f, 0.0f, -1.0f,// bottom-right         
		0.0f, 0.0f, -1.0f,// top-right
		0.0f, 0.0f, -1.0f,// bottom-left
		0.0f, 0.0f, -1.0f,// top-left

		0.0f, 0.0f, 1.0f,// bottom-left
		0.0f, 0.0f, 1.0f,// bottom-right
		0.0f, 0.0f, 1.0f,// top-right
		0.0f, 0.0f, 1.0f,// top-right
		0.0f, 0.0f, 1.0f,// top-left
		0.0f, 0.0f, 1.0f,// bottom-left

		-1.0f, 0.0f, 0.0f, // top-right
		-1.0f, 0.0f, 0.0f,// top-left
		-1.0f, 0.0f, 0.0f,// bottom-left
		-1.0f, 0.0f, 0.0f,// bottom-left
		-1.0f, 0.0f, 0.0f,// bottom-right
		-1.0f, 0.0f, 0.0f,// top-right

		1.0f, 0.0f, 0.0f,// top-left
		1.0f, 0.0f, 0.0f,// bottom-right
		1.0f, 0.0f, 0.0f,// top-right         
		1.0f, 0.0f, 0.0f,// bottom-right
		1.0f, 0.0f, 0.0f,// top-left
		1.0f, 0.0f, 0.0f,// bottom-left     

		0.0f, -1.0f, 0.0f,// top-right
		0.0f, -1.0f, 0.0f,// top-left
		0.0f, -1.0f, 0.0f,// bottom-left
		0.0f, -1.0f, 0.0f,// bottom-left
		0.0f, -1.0f, 0.0f,// bottom-right
		0.0f, -1.0f, 0.0f,// top-right

		0.0f, 1.0f, 0.0f,// top-left
		0.0f, 1.0f, 0.0f,// bottom-right
		0.0f, 1.0f, 0.0f,// top-right     
		0.0f, 1.0f, 0.0f,// bottom-right
		0.0f, 1.0f, 0.0f,// top-left
		0.0f, 1.0f, 0.0f// bottom-left        
	};

	const float cubeTexCoords[36 * 2] = {
		
		0.0f, 0.0f, // Bottom-left
		1.0f, 1.0f, // top-right
		1.0f, 0.0f, // bottom-right         
		1.0f, 1.0f, // top-right
		0.0f, 0.0f, // bottom-left
		0.0f, 1.0f, // top-left
		
		0.0f, 0.0f, // bottom-left
		1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, // top-right
		1.0f, 1.0f, // top-right
		0.0f, 1.0f, // top-left
		0.0f, 0.0f, // bottom-left
		
		1.0f, 0.0f, // top-right
		1.0f, 1.0f, // top-left
		0.0f, 1.0f, // bottom-left
		0.0f, 1.0f, // bottom-left
		0.0f, 0.0f, // bottom-right
		1.0f, 0.0f, // top-right
		
		1.0f, 0.0f, // top-left
		0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, // top-right         
		0.0f, 1.0f, // bottom-right
		1.0f, 0.0f, // top-left
		0.0f, 0.0f, // bottom-left     
		
		0.0f, 1.0f, // top-right
		1.0f, 1.0f, // top-left
		1.0f, 0.0f, // bottom-left
		1.0f, 0.0f, // bottom-left
		0.0f, 0.0f, // bottom-right
		0.0f, 1.0f, // top-right
		
		0.0f, 1.0f, // top-left
		1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, // top-right     
		1.0f, 0.0f, // bottom-right
		0.0f, 1.0f, // top-left
		0.0f, 0.0f  // bottom-left        
	};

	int practise(string projectDir);
};

