#ifndef SKY_BOX_DATA_H_
#define SKY_BOX_DATA_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/// <summary>
/// 天空盒子的数据, 图片地址, 以及定点数据
/// </summary>
struct SkyBoxData
{
	const vector<string> images = {   //OpenGL给我们提供了6个特殊的纹理目标，专门对应立方体贴图的一个面。注意顺序
		"images/skybox/left.jpg",   //左    GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		"images/skybox/right.jpg",  //右    GL_TEXTURE_CUBE_MAP_POSITIVE_X
		
        "images/skybox/top.jpg",    //上    GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		"images/skybox/bottom.jpg", //下    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y

        "images/skybox/back.jpg",   //后    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		"images/skybox/front.jpg"   //前    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

    //只有顶点的数据
    float skyboxVertices[36*3] = {
        // positions          
        -1.0f,  1.0f, -1.0f,  //背面6点
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  //左边6点
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,  //右边6点
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  //正面6点
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,  //上面6点
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,  //底部6点
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

};

#endif // !SKY_BOX_DATA_H_
