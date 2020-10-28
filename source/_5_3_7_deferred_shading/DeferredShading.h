#pragma once

#include "WindowHelper.h"
#include "MyShader.h"
#include "RenderUtil.h"
#include "mesh.h"
#include "model.h"

#include <iostream>
#include <string>

using namespace std;

class DeferredShading {
public:
	const char* vertFile = "source/_5_3_7_deferred_shading/_5_3_7_render.vert";
	const char* fragFile = "source/_5_3_7_deferred_shading/_5_3_7_render.frag";

	const char* imgFileWood = "images/wood.png";

    const char* modelFileNano = "images/nanosuit/nanosuit.obj";

    const char* vertFileQuad = "source/_5_3_7_deferred_shading/_5_3_7_quad.vert";
    const char* fragFileQuad = "source/_5_3_7_deferred_shading/_5_3_7_quad.frag";

    //vertFileGBuffer, fragFileGBuffer
    const char* vertFileGBuffer = "source/_5_3_7_deferred_shading/_5_3_7_gbuffer_render.vert";
    const char* fragFileGBuffer = "source/_5_3_7_deferred_shading/_5_3_7_gbuffer_render.frag";

    float planeVertices[8 * 6] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    float quadVertices[4 * 5] = {
        // positions            // texture Coords
        -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
    };

    int practise(const char * projectDir);
};

