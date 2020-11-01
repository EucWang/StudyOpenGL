#pragma once

#include "WindowHelper.h"
#include "MyShader.h"
#include "RenderUtil.h"

#include "Model.h"

#include <iostream>
#include <string>
#include <vector>
#include <random>

using namespace std;

/// <summary>
/// ��2007�꣬Crytek��˾������һ�������Ļ�ռ价�����ڱ�(Screen-Space Ambient Occlusion, SSAO)�ļ�����
/// �����������ǵĿ������µ�Σ���ϡ���һ����ʹ������Ļ�ռ䳡������ȶ�������ʵ�ļ�����������ȷ���ڱ�����
/// ��һ��������������Ļ������ڱβ����ٶȿ죬���һ��ܻ�úܺõ�Ч����ʹ������Ϊ����ʵʱ�������ڱεı�׼��
/// 
/// SSAO�����ԭ��ܼ򵥣�
/// ���������ı���(Screen-filled Quad)�ϵ�ÿһ��Ƭ�Σ�
/// ���Ƕ�������ܱ����ֵ����һ���ڱ�����(Occlusion Factor)
/// ������ڱ�����֮��ᱻ�������ٻ��ߵ���Ƭ�εĻ������շ�����
/// �ڱ�������ͨ���ɼ�Ƭ����Χ���ͺ���(Kernel)�Ķ��������������͵�ǰƬ�����ֵ�Աȶ��õ��ġ�
/// ����Ƭ�����ֵ�����ĸ�������������Ҫ���ڱ����ӡ�
/// 
/// ͨ���ڷ��������(Normal-oriented Hemisphere)��Χ���������ǽ����ῼ�ǵ�Ƭ�εײ��ļ�����.
/// �������˻������ڱλ����ɵĸо����Ӷ���������ʵ�Ľ����
/// 
/// **��������**
/// SSAO��Ҫ��ȡ���������Ϣ����Ϊ������ҪһЩ��ʽ��ȷ��һ��Ƭ�ε��ڱ����ӡ�����ÿһ��Ƭ�Σ����ǽ���Ҫ��Щ���ݣ�
///     ��Ƭ��λ������
///     ��Ƭ�εķ�������
///     ��Ƭ�εķ�����ɫ
///     ��������
///     ������ת�������ĵ������תʸ��
/// </summary>
class PractiseSSAO {
public:
	const char* vertFile = "source/_5_3_8_SSAO/ssao_render.vert";
	const char* fragFile = "source/_5_3_8_SSAO/ssao_render.frag";

    const char* vertFileGBuffer = "source/_5_3_8_SSAO/ssao_gbuffer.vert";
    const char* fragFileGBuffer = "source/_5_3_8_SSAO/ssao_gbuffer.frag";

	const char* imgFileWood = "images/container2.png";

    const char* vertFileQuad = "source/_5_3_8_SSAO/ssao_quad.vert";
    const char* fragFileQuad = "source/_5_3_8_SSAO/ssao_quad.frag";

    const char* vertFileBlur = "source/_5_3_8_SSAO/ssao_blur.vert";
    const char* fragFileBlur = "source/_5_3_8_SSAO/ssao_blur.frag";

    const char* vertFileLight = "source/_5_3_8_SSAO/ssao_final_light.vert";
    const char* fragFileLight = "source/_5_3_8_SSAO/ssao_final_light.frag";

    const char* modelFileNano = "images/nanosuit/nanosuit.obj";

    const char* imgFileWoodSpec = "images/container2_specular.png";

    WindowHelper* helper;

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

    float cubeVertices[8 * 36] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    // lighting info
    // -------------
    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    //glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
    glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

	int practise(const char* projectDir);

        GLuint gframebuffer;
        GLuint gTexPositionDepth, gTexNormal, gTexAlbdoSpec;
    void makeGFrameBuffer();

        GLuint ssaoFBO, ssaoTex;
    void makeSSAOFrameBuffer();

        GLuint ssaoBlurFBO, ssaoBlurTex;
    void makeSSAOBlurFrameBuffer();

        std::vector<glm::vec3> ssaoKernel;   //����һ����λ���򣬰���64���������ֵ�Ĳ�������
        std::vector<glm::vec3> ssaoNoise;
        GLuint noiseTexture;
    void makeSSAOKernel();
};