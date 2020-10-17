#pragma once

#include "WindowHelper.h"
#include "fileUtil.h"
#include "RenderUtil.h"
#include "MyShader.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class NormalMap2 {
public:
	//const char* vertFile = "source/_5_3_3_2_normal_map/_5_3_3_2_render.vert";
	//const char* fragFile = "source/_5_3_3_2_normal_map/_5_3_3_2_render.frag";
	const char* vertFile = "source/_5_3_3_2_normal_map/_5_3_3_2_render2.vert";
	const char* fragFile = "source/_5_3_3_2_normal_map/_5_3_3_2_render2.frag";

    const char* vertFileLight = "source/_5_3_3_2_normal_map/_5_3_3_2_light.vert";
    const char* fragFileLight = "source/_5_3_3_2_normal_map/_5_3_3_2_light.frag";

	const char* imgFileBrickWall = "images/brickwall.jpg";
	const char* imgFileBrickWallNormal = "images/brickwall_normal.jpg";

	const float wallVertices[8 * 6] = {
		//position				//normal              //texture coordinates
		-1.0f,  1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,     1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,     0.0f, 0.0f
	};

    float cubeVertices[8 * 36] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

	//position
	glm::vec3 pos1 = glm::vec3(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2 = glm::vec3(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3 = glm::vec3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4 = glm::vec3(1.0f, 1.0f, 0.0f);
	//texture coordinates
	glm::vec2 uv1 = glm::vec2(0.0f, 1.0f);
	glm::vec2 uv2 = glm::vec2(0.0f, 0.0f);
	glm::vec2 uv3 = glm::vec2(1.0f, 0.0f);
	glm::vec2 uv4 = glm::vec2(1.0f, 1.0f);
	//normal
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);

	int practise(const char* projectDir);
	void render(MyShader &shader);
};

