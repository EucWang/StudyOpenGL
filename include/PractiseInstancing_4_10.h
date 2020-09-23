#pragma once

#include "camera.h"
#include "fileUtil.h"
#include "MyShader.h"
#include "RenderUtil.h"

#include "model.h"
#include "mesh.h"

#include <string>
#include <iostream>

using namespace std;

class PractiseInstancing_4_10
{

public:

	const char* vertFile = "shader/_4_10_instancing.vert";
	const char* fragFile = "shader/_4_10_instancing.frag";

    const char* vertFile2 = "shader/_4_10_instancing2.vert";

    const char* vertFile3 = "shader/_4_10_instancing3.vert";
    const char* fragFile3 = "shader/_4_10_instancing3.frag";

    const char* vertFile4 = "shader/_4_10_instancing4.vert";
    const char* fragFile4 = "shader/_4_10_instancing4.frag";

    const char* imgFileAsteroid = "images/asteroid_belt/planet.obj";
    const char* imgFileRock = "images/asteroid_belt/rock.obj";

	const float quadVertices[6 * 2] = {
        // λ��        
      -0.05f,  0.05f, 
       0.05f, -0.05f, 
      -0.05f, -0.05f, 

      -0.05f,  0.05f, 
       0.05f, -0.05f, 
       0.05f,  0.05f 
	};

	const float quadColors[6 * 3] = {
      // ��ɫ
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,

      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f
	};

    /// <summary>
    /// ʵ����ͬһ������Ķ����Ⱦ
    /// ʹ��uniform ���� ����ƫ����������
    /// Ȼ�� ʹ�� gl_InstanceID ����ȡÿ����Ⱦ��id
    /// </summary>
    /// <param name="projectDir"></param>
    /// <returns></returns>
    int practise(string projectDir);

    /// <summary>
    /// ʹ��ʵ����������������� ƫ��������
    /// �ؼ� api:
    ///     glVertexAttribDivisor(int vertexIndex, int attributeDivisor);
    ///     ��һ����������Ҫ�Ķ�������
    ///     �ڶ������������Գ���(Attribute Divisor)
    /// 
    /// </summary>
    /// <param name="projectDir"></param>
    /// <returns></returns>
    int practise2(string projectDir);
    
    //����С���Ǵ�
    int practise3(string projectDir);

    /// <summary>
    /// С���Ǵ�, ���ĸ�����,
    /// ʹ�� ʵ�������������� ���ģ����������, �Ż���Ⱦ
    /// </summary>
    /// <param name="projectDir"></param>
    /// <returns></returns>
    int practise4(string projectDir);
};

