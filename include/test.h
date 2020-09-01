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

/**
* 
*/
int practiseDrawWithTransform11(const char* projectDir);

/**
*  ��������ϵ�任���� 
* ��һ���������廯��ƽ�ŵ�����
*/
int practiseDrawWithPerspective12(const char* projectDir);

/**
* ��Ⱦһ����ת������������
*/
int practiseDrawWithPerspective13(const char* projectDir);

int practiseDrawWithPerspective14(const char* projectDir);

/**
* �ӽǾ��Զ���ת
*/
int practiseDrawWithPerspective15(const char* projectDir);

/**
* ��ϰ
* �������������� ������ ��ͷ�ӽ�
*/
int practiseDrawWithPerspective16(const char* projectDir);

/**
* ��ϰ
* ͨ��������Camera����������ƾ�ͷ�ӽ�
*/
int practiseDrawWithCamera(char* projectDir);

/**
*  ��ϰ����
*/
int practise2Light(const char* projectDir);

int practiseLight19(const char * projectDir);

int practiseLight20(const char * projectDir);

int practiseLight21(const char* projectDir);

int practiseLight22(const char* projectDir);

/**
* ƽ�й⣬ ģ��̫����
*/
int practiseLight23(const char* projectDir);

/**
* ���Դ�� ����ǰ�����ĵ��Դ�����ϣ����� ǿ�ȣ���Χ��˥��
*/
int practiseLight24(const char* projectDir);

#ifdef __cplusplus
}
#endif

#endif