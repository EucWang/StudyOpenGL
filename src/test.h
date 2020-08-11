#ifndef TEST_H_
#define TEST_H_

#ifdef __cplusplus    //__cplusplus��cpp���Զ����һ����
extern "C" {          //���߱��������ⲿ�ִ��밴C���Եĸ�ʽ���б��룬������C++��
#endif

//������Ⱦ����ɫ�����ַ���ͷ�ļ�
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