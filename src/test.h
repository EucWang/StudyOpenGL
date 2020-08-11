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

int practiseDrawWithTransform11(const char* projectDir);

int practiseDrawWithPerspective12(const char* projectDir);

#ifdef __cplusplus
}
#endif

#endif