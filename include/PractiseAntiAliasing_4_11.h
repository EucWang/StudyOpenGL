#pragma once

#include "camera.h"
#include "fileUtil.h"

#include "MyShader.h"
#include "RenderUtil.h"

#include "mesh.h"
#include "model.h"
#include "WindowHelper.h"

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

	const char* imgFileCube = "images/container2.png";

	const char* vertFile0 = "shader/_4_11_anti_aliasing3.vert";
	const char* fragFile0 = "shader/_4_11_anti_aliasing3.frag";

	const char* vertFile3 = "shader/_4_11_anti_aliasing4.vert";
	const char* fragFile3 = "shader/_4_11_anti_aliasing4.frag";

	const char* vertFileScreen = "shader/_4_11_anti_aliasing_screen.vert";
	const char* fragFileScreen = "shader/_4_11_anti_aliasing_screen.frag";

    //�ذ�����귨������
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

    //�ݵ���������
    const float grassVertices[6 * 5] = {
        //positions			//texture Coords
        -0.5f, 0.5f, 0.0f,   0.0f,  1.0f,
        0.5f, 0.5f, 0.0f,    1.0f,  1.0f,
        0.5f, -0.5f, 0.0f,   1.0f,  0.0f,

        -0.5f, 0.5f, 0.0f,   0.0f,  1.0f,
        0.5f, -0.5f, 0.0f,   1.0f,  0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,
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

	/// <summary>
	/// �򵥵� ���ز�����������
	/// </summary>
	/// <param name="projectDir"></param>
	/// <returns></returns>
	int practise0(string projectDir);

	/// <summary>
	/// ʵ�����飬 ����ͬһ�������Ⱦ�Ż�
	/// ���ز������壬 �����
	/// </summary>
	/// <param name="projectDir"></param>
	/// <returns></returns>
	int practise(string projectDir);



	/// <summary>
	///  �������ز�������
	///  ֡����
	/// </summary>
	/// <param name="projectDir"></param>
	/// <returns></returns>
	int practise1(string projectDir);


	/// <summary>
	///  ֡����
	/// </summary>
	/// <param name="projectDir"></param>
	/// <returns></returns>
	int practise2(string projectDir);
};

