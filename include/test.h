#ifndef TEST_H_
#define TEST_H_

#ifdef __cplusplus    //__cplusplus是cpp中自定义的一个宏
extern "C" {          //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
#endif

//包含渲染的着色器的字符串头文件
#include "shaders.h"
#include "shaderSource.h"

int drawTriangle();

int drawRectangle();

int drawUnfillRectangle();

int practiseDrawTriangles1();

int practiseDrawTriangles2();

int practiseDrawTriangles3();

int practiseDrawTriangles4();

int practiseDrawTriangles5();

int practiseDrawTriangle6();

int practiseDrawTriangle7();

void testReadFile();

int practiseDrawTriangle8();

void testLoadStbImage();

int testPractiseDrawWithTexure9(char * projectDir);

int testGetParentDir();

int testGetChildPath();

/**
* 
*/
int practiseDrawWithTransform11(const char* projectDir);

/**
*  利用坐标系变换矩阵， 
* 将一面面子立体化的平放到地面
*/
int practiseDrawWithPerspective12(const char* projectDir);

/**
* 渲染一个旋转的立方体箱子
*/
int practiseDrawWithPerspective13(const char* projectDir);

int practiseDrawWithPerspective14(const char* projectDir);

/**
* 视角矩自动旋转
*/
int practiseDrawWithPerspective15(const char* projectDir);

/**
* 练习
* 可以用上下左右 来控制 镜头视角
*/
int practiseDrawWithPerspective16(const char* projectDir);

/**
* 练习
* 通过新增的Camera类对象来控制镜头视角
*/
int practiseDrawWithCamera(char* projectDir);

/**
*  练习光照
*/
int practise2Light(const char* projectDir);

int practiseLight19(const char * projectDir);

int practiseLight20(const char * projectDir);

int practiseLight21(const char* projectDir);

int practiseLight22(const char* projectDir);

/**
* 平行光， 模拟太阳光
*/
int practiseLight23(const char* projectDir);

/**
* 点光源， 在以前讲话的点光源基础上，增加 强度，范围，衰减
*/
int practiseLight24(const char* projectDir);

#ifdef __cplusplus
}
#endif

#endif