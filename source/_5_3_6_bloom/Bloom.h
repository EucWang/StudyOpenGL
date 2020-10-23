#pragma once

#include "WindowHelper.h"
#include "RenderUtil.h"
#include "MyShader.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Bloom
{

public:

    const char* vertFile = "source/_5_3_6_bloom/_5_3_6_bloom.vert";
    const char* fragFile = "source/_5_3_6_bloom/_5_3_6_bloom.frag";

    const char* imgFileWood = "images/wood.png";

    float planeVertices[8 * 6] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightPos = glm::vec3(1.0f, 2.0f, 0.0f);

    int practise(const char * projectDir);
};

