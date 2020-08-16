#include "../include/fileUtil.h"

#include "../include/list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LineMaxLen 2048
#define KeyMaxLen 64

/**
* ���ļ��ж�ȡȫ���ı��ַ�
*/
bool readStrFromFile(const char* filePath, char** desStr) {
	if (filePath == NULL) { return false;	}

	FILE* file = NULL;

	char filebuf[1024 * 16] = { 0 };
	char lineBuf[LineMaxLen];

	int error = fopen_s(&file, filePath, "r");
	if (error != 0) {  //���ļ�ʧ��
		printf("%s%s%s%d\n", "failed to open file. file path is [", filePath, "], error number is ", error);
		return false;
	}
	char* pTmp;
	List  list;
	list_init(&list, free);
	int totalLen = 0;
	while (!feof(file)) {
		memset(lineBuf, 0, sizeof(lineBuf));  //���buf
		pTmp = fgets(lineBuf, LineMaxLen, file);
		if (pTmp == NULL) {
			break;
		}

		int len = strlen(lineBuf);
		char* str = malloc((len + 1) + sizeof(char));
		memset(str, 0, (len + 1) + sizeof(char));
		strcpy_s(str, len + 1, lineBuf);
		list_add(&list, str);
		totalLen += (len);
	}

	int listSize = list_size(&list);
	if (listSize <= 0) {
		return false;
	}

	char* retVal = malloc((totalLen + 1) * sizeof(char));
	memset(retVal, 0, totalLen + 1);
	char* tmp;
	int len = 0;
	char* ptr = retVal;
	list_resetIterator(&list);
	while (list_hasNext(&list)) {
		list_moveToNext(&list);
		list_iterator(&list, (void**)(&tmp));
		len = strlen(tmp);
		memcpy(ptr, tmp, len);
		ptr = ptr + len;
	}

	//printf("%s%d\n", "total totalLen is ", totalLen);
	//printf("\n%s\n", "============================");
	//printf("%s\n", retVal);

	list_destroy(&list);
	*desStr = retVal;
	return true;
}

/*
* ��ȡ����Ŀ¼���ַ���
* @param path : ��������� ��Ҫ�����·��
* @param parentPath : ��������� path�ĸ���·����malloc����Ŀռ䣬��Ҫ�����ͷ�
*/
bool getParentDir(const char* path, char ** parentPath) {

	char * end = strrchr(path, '/');
	if (end == NULL) {
		end = strrchr(path, '\\');
	}
		
	if (end == NULL)	{
		return false;
	}

	int size = end - path + 1;

	char * retVal  = malloc(size * sizeof(char));
	memset(retVal, 0, size);
	memcpy(retVal, path, size - 1);

	*parentPath = retVal;
	return true;
}
 
/**
* ���ݸ���·�����ļ�������ȡ�ļ���·���ַ���
* @param desPath :  �ļ���Ӧ��ȫ·��
* @param parentPath ��������� ����·��
* @param fileName  ���������  ·���µ��ļ�����
* @return bool �Ƿ񴴽��ɹ�
*/
bool getChildPath(/*out*/char** desPath, /*in*/const char* parentPath, /*in*/const char* fileName) {
	int pLen = strlen(parentPath);
	int fLen = strlen(fileName);

	if (pLen <= 0 || fLen <= 0) {
		return false;
	}
	char separator = '/';
	if (!strrchr(parentPath, '/')) {
		separator = '\\';
	}

	char* retVal = (char*)malloc((pLen + fLen + 2) * sizeof(char));
	if (!retVal)	{
		printf("%s\n", "getChildPath() failed, because malloc failed.");
		return false;
	}
	memset(retVal, 0, (pLen + fLen + 2) * sizeof(char));

	sprintf_s(retVal, (pLen + fLen + 2) * sizeof(char), "%s%c%s", parentPath, separator, fileName);
	*desPath = retVal;
	return true;
}