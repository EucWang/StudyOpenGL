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
        // 位置        
      -0.05f,  0.05f, 
       0.05f, -0.05f, 
      -0.05f, -0.05f, 

      -0.05f,  0.05f, 
       0.05f, -0.05f, 
       0.05f,  0.05f 
	};

	const float quadColors[6 * 3] = {
      // 颜色
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,

      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f
	};

    /// <summary>
    /// 实例化同一个物体的多次渲染
    /// 使用uniform 数组 传递偏移数据数组
    /// 然后 使用 gl_InstanceID 来获取每次渲染的id
    /// </summary>
    /// <param name="projectDir"></param>
    /// <returns></returns>
    int practise(string projectDir);

    /// <summary>
    /// 使用实例化数组来打包传递 偏移量数组
    /// 关键 api:
    ///     glVertexAttribDivisor(int vertexIndex, int attributeDivisor);
    ///     第一个参数是需要的顶点属性
    ///     第二个参数是属性除数(Attribute Divisor)
    /// 
    /// </summary>
    /// <param name="projectDir"></param>
    /// <returns></returns>
    int practise2(string projectDir);
    
    //加载小行星带
    int practise3(string projectDir);

    /// <summary>
    /// 小行星带, 第四个例子,
    /// 使用 实例化数组来传递 多个模型数据数组, 优化渲染
    /// </summary>
    /// <param name="projectDir"></param>
    /// <returns></returns>
    int practise4(string projectDir);
};

