#ifndef  FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdbool.h>

/**
	���ļ��ж�ȡȫ���ַ���
	@param filePath : ��������� �ļ�·��
	@param desStr : ��������� �ļ��еĶ�ȡ����ȫ���ַ��� malloc������ڴ棬�����ֶ��ͷ�
	@return bool : ��ȡ�ɹ�����ʧ��
*/
bool readStrFromFile(/*in*/const char* filePath, /*out*/char** desStr);

/*
* ��ȡ����Ŀ¼���ַ���
* @param path : ��������� ��Ҫ�����·��
* @param parentPath : ��������� path�ĸ���·���� malloc������ڴ棬�����ֶ��ͷ�
* @return bool �Ƿ񴴽��ɹ�
*/
bool getParentDir(/*in*/const char * path, /*out*/char ** parentPath);

/**
* ���ݸ���·�����ļ�������ȡ�ļ���·���ַ���
* @param desPath :  �ļ���Ӧ��ȫ·���� malloc������ڴ棬�����ֶ��ͷ�
* @param parentPath ��������� ����·��
* @param fileName  ���������  ·���µ��ļ�����
* @return bool �Ƿ񴴽��ɹ�
*/
bool getChildPath(/*out*/char** desPath, /*in*/const char* parentPath, /*in*/const char* fileName);

#endif // ! FILE_UTIL_H
