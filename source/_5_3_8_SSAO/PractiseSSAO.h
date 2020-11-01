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
/// 在2007年，Crytek公司发布了一款叫做屏幕空间环境光遮蔽(Screen-Space Ambient Occlusion, SSAO)的技术，
/// 并用在了他们的看家作孤岛危机上。这一技术使用了屏幕空间场景的深度而不是真实的几何体数据来确定遮蔽量。
/// 这一做法相对于真正的环境光遮蔽不但速度快，而且还能获得很好的效果，使得它成为近似实时环境光遮蔽的标准。
/// 
/// SSAO背后的原理很简单：
/// 对于铺屏四边形(Screen-filled Quad)上的每一个片段，
/// 我们都会根据周边深度值计算一个遮蔽因子(Occlusion Factor)
/// 。这个遮蔽因子之后会被用来减少或者抵消片段的环境光照分量。
/// 遮蔽因子是通过采集片段周围球型核心(Kernel)的多个深度样本，并和当前片段深度值对比而得到的。
/// 高于片段深度值样本的个数就是我们想要的遮蔽因子。
/// 
/// 通过在法向半球体(Normal-oriented Hemisphere)周围采样，我们将不会考虑到片段底部的几何体.
/// 它消除了环境光遮蔽灰蒙蒙的感觉，从而产生更真实的结果。
/// 
/// **样本缓冲**
/// SSAO需要获取几何体的信息，因为我们需要一些方式来确定一个片段的遮蔽因子。对于每一个片段，我们将需要这些数据：
///     逐片段位置向量
///     逐片段的法线向量
///     逐片段的反射颜色
///     采样核心
///     用来旋转采样核心的随机旋转矢量
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

        std::vector<glm::vec3> ssaoKernel;   //假设一个单位半球，包含64个随机样本值的采样核心
        std::vector<glm::vec3> ssaoNoise;
        GLuint noiseTexture;
    void makeSSAOKernel();
};