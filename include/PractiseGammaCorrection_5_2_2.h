#pragma once

#include "WindowHelper.h"
#include "RenderUtil.h"
#include "fileUtil.h"
#include "MyShader.h"

class PractiseGammaCorrection_5_2_2 {
public:

    const char* vertFile = "shader/_5_2_2_gamma_correction.vert";
    const char* fragFile = "shader/_5_2_2_gamma_correction.frag";

    const char* imgFileWood = "images/wood.png";

    float planeVertices[6*8] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };

    glm::vec3 lightPositions[4] = {
       glm::vec3(-3.0f, 0.0f, 0.0f),
       glm::vec3(-1.0f, 0.0f, 0.0f),
       glm::vec3(1.0f, 0.0f, 0.0f),
       glm::vec3(3.0f, 0.0f, 0.0f)
    };

    glm::vec3 lightColors[4] = {
        glm::vec3(0.25),
        glm::vec3(0.50),
        glm::vec3(0.75),
        glm::vec3(1.00)
    };

	int practise(const char* projectDir);
};

