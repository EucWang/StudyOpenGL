#ifndef  FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdbool.h>

/**
	���ļ��ж�ȡȫ���ַ���
	@param filePath : �ļ�·��
	@param desStr : �ļ��еĶ�ȡ����ȫ���ַ�
	@return bool : ��ȡ�ɹ�����ʧ��
*/
bool readStrFromFile(const char* filePath, char** desStr);

#endif // ! FILE_UTIL_H
