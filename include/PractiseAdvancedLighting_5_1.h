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
	const char* imgFilePlaneSpecular = "images/wood_specular.png";

    //地板的坐标法线纹理
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

	//箱子的坐标法线纹理
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

	//标准坐标系下， 将帧缓冲纹理输出显示需要的坐标/纹理
	const float quadVertices[6 * 4] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	//----------------化学实验室场景------------------------------------------>>>>>>>>>>>>
	//定向光的4个数据
	const glm::vec3 dirLightArgs[4] = {
		//glm::vec3(0.5f, 0.5f, 0.5f),  //ambient
		//glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
		//glm::vec3(1.0f, 1.0f, 1.0f),     //specular
		glm::vec3(0.2f, 0.2f, 0.2f),  //ambient
		glm::vec3(0.4f, 0.4f, 0.4f),     //diffuse
		glm::vec3(0.5f, 0.5f, 0.5f),     //specular
		glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
	};

	//点光源的6个数据
	const glm::vec3 pointLightColors[5] = {
		//glm::vec3(0.4f, 0.7f, 0.1f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
		glm::vec3(1.0f, 1.0f, 1.0f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
		glm::vec3(0.4f, 0.7f, 0.1f),  //第二个点光源的 (ambient, diffuse, specular)  红色
		glm::vec3(0.4f, 0.7f, 0.1f),   //第三个点光源的 (ambient, diffuse, specular)  黄色
		glm::vec3(0.4f, 0.7f, 0.1f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色

		glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
	};

	//聚光的6个数据
	const glm::vec3 spotlightArgs[4] = {
		glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
		//glm::vec3(0.0f, 1.0f, 0.0f),     //diffuse
		//glm::vec3(0.0f, 1.0f, 0.0f),     //specular
		glm::vec3(0.3f, 0.3f, 0.3f),     //diffuse
		glm::vec3(0.7f, 0.7f, 0.7f),     //specular
		glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
	};

	const glm::vec3 clearColor = glm::vec3(0.9f, 0.9f, 0.9f);   //背景颜色

	const glm::vec2 spotlight_cutoff = glm::vec2(7.0f, 10.0f);  //聚光的切角和外切角

	const glm::vec3 lightPos = glm::vec3(2.0f, 0.5f, -2.0f);
	//-----------------------------------------------------------------<<<<<<<<<<<<<<

	int practise(const char* projectDir);
};

