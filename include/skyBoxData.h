#ifndef SKY_BOX_DATA_H_
#define SKY_BOX_DATA_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/// <summary>
/// ��պ��ӵ�����, ͼƬ��ַ, �Լ���������
/// </summary>
struct SkyBoxData
{
	const vector<string> images = {   //OpenGL�������ṩ��6�����������Ŀ�꣬ר�Ŷ�Ӧ��������ͼ��һ���档ע��˳��
		"images/skybox/left.jpg",   //��    GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		"images/skybox/right.jpg",  //��    GL_TEXTURE_CUBE_MAP_POSITIVE_X
		
        "images/skybox/top.jpg",    //��    GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		"images/skybox/bottom.jpg", //��    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y

        "images/skybox/back.jpg",   //��    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		"images/skybox/front.jpg"   //ǰ    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

    //ֻ�ж��������
    float skyboxVertices[36*3] = {
        // positions          
        -1.0f,  1.0f, -1.0f,  //����6��
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  //���6��
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,  //�ұ�6��
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  //����6��
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,  //����6��
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,  //�ײ�6��
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

};

#endif // !SKY_BOX_DATA_H_
