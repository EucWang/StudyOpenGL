#pragma once

#include "camera.h"
#include "fileUtil.h"

#include "MyShader.h"
#include "RenderUtil.h"

#include "mesh.h"
#include "model.h"

#include <string>
#include <iostream>

using namespace std;

class PractiseAntiAliasing_4_11 {
public:
	const char* vertFile = "shader/_4_11_anti_aliasing.vert";
	const char* fragFile = "shader/_4_11_anti_aliasing.frag";

    const char* vertFileGrass = "shader/_4_11_anti_aliasing2_grass.vert";
	const char* fragFileGrass = "shader/_4_11_anti_aliasing2_grass.frag";

	const char* imgFileGrass = "images/grass.png";
	const char* imgFilePlane = "images/metal.png";

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

    const float grassVertices[6 * 5] = {
        //positions			//texture Coords
        -0.5f, 0.5f, 0.0f,   0.0f,  1.0f,
        0.5f, 0.5f, 0.0f,    1.0f,  1.0f,
        0.5f, -0.5f, 0.0f,   1.0f,  0.0f,

        -0.5f, 0.5f, 0.0f,   0.0f,  1.0f,
        0.5f, -0.5f, 0.0f,   1.0f,  0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,
    };

	int practise0(string projectDir);

	int practise(string projectDir);

};

