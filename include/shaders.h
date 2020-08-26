#ifndef SHADER_H_
#define SHADER_H_

#ifdef __cplusplus    //__cplusplus是cpp中自定义的一个宏
extern "C" {          //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

//#define true 1
#define TRUE 1
//#define false 0
#define FALSE 0

#define SMALL_SCREEN_WIDTH 800
#define SMALL_SCREEN_HEIGHT 640

GLFWwindow* createGLWindow(int width, int height, const char* title);

//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//我们同样也希望能够在GLFW中实现一些输入控制，这可以通过使用GLFW的几个输入函数来完成。
//我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。
void processInput(GLFWwindow* window);

/**
* 创建 纹理对象， 绑定纹理， 并设置纹理的默认环绕为 GL_REPEAT /过滤 为GL_LINEAR 
* @param texture 纹理对象的int值， 会创建纹理对象， 这里传地址
*/
void textureGenSet(/*in,out*/GLuint* texture);

/**
* 创建 纹理对象， 绑定纹理， 并设置纹理的默认环绕 /过滤
* @param texture 纹理对象的int值， 会创建纹理对象， 这里传地址
* @param wrapS  取值为 GL_REPEAT ,  对纹理的默认行为。重复纹理图像。
										GL_MIRRORED_REPEAT, 和GL_REPEAT一样，但每次重复图片是镜像放置的。
										GL_CLAMP_TO_EDGE, 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
										GL_CLAMP_TO_BORDER, 超出的坐标为用户指定的边缘颜色。
* @param wrapT 取值为同 wrapS
* @param minFilter  取值为  GL_NEAREST  （也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。
*																		设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素。
*																		GL_NEAREST产生了颗粒状的图案，我们能够清晰看到组成纹理的像素，
*											  GL_LINEAR      （也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色。
*																		 GL_LINEAR能够产生更平滑的图案，很难看出单个的纹理像素。
*/
void textureGenSets(/*in,out*/GLuint* texture, /*in*/int wrapS, /*in*/int wrapT, /*in*/int minFilter, /*in*/int magFilter);

/**
* 在绑定纹理之后， 加载纹理图片
* @param parentDir   图片所在目录
* @param imgName  图片文件名
* @param rgbType   图片的rgb类型 取值 GL_RGB (jpg图片取值) ,
*																 GL_RGBA (png图片取值，有透明通道)
*/
bool textureLoadImg(/*in*/const char* parentDir, /*in*/const char* imgName, /*in*/int rgbType);

void textureBind(GLuint shaderProgram, const char* sampler2dName, int index);

void textureUse(GLuint texture, int index);

#ifdef __cplusplus
}
#endif

#endif