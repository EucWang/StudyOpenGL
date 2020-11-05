#pragma once

#include "WindowHelper.h"
#include "RenderUtil.h"
#include "MyShader.h"


class PractisePBRLighting2 {

public:
	const char* vertFile = "source/_6_1_2_PBR_Lighting/pbr_render.vert";
	const char* fragFile = "source/_6_1_2_PBR_Lighting/pbr_render.frag";

	const char* imgFileAlbedo = "images/pbr/rusted_iron/albedo.png";
	const char* imgFileAo = "images/pbr/rusted_iron/ao.png";
	const char* imgFileMetallic = "images/pbr/rusted_iron/metallic.png";
	const char* imgFileNormal = "images/pbr/rusted_iron/normal.png";
	const char* imgFileRoughness = "images/pbr/rusted_iron/roughness.png";

	WindowHelper* helper;

	MyShader* myshader;

    glm::vec3 lightPos[4] = {
        glm::vec3(-10.0f, 10.0f, 10.0f),
        glm::vec3(10.0f, 10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };

    //这里的光照颜色不一样呢
    glm::vec3 lightColor[4] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
    };
    int nrRows = 7;     //7行7列的球
    int nrColumn = 7;
    float spacing = 2.5;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;

    unsigned int sphereVAO = 0;
    unsigned int sphereIndexCount = 0;
    
    void createSphereData();

	int practise(const char * projectDir);
};

