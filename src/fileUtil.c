#include "../include/fileUtil.h"

#include "../include/list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LineMaxLen 2048
#define KeyMaxLen 64

/**
* 从文件中读取全部文本字符
*/
bool readStrFromFile(const char* filePath, char** desStr) {
	if (filePath == NULL) { return false;	}

	FILE* file = NULL;

	char filebuf[1024 * 16] = { 0 };
	char lineBuf[LineMaxLen];

	int error = fopen_s(&file, filePath, "r");
	if (error != 0) {  //打开文件失败
		printf("%s%s%s%d\n", "failed to open file. file path is [", filePath, "], error number is ", error);
		return false;
	}
	char* pTmp;
	List  list;
	list_init(&list, free);
	int totalLen = 0;
	while (!feof(file)) {
		memset(lineBuf, 0, sizeof(lineBuf));  //情况buf
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
* 获取父级目录的字符串
* @param path : 输出参数， 需要处理的路径
* @param parentPath : 输出参数， path的父级路径，malloc分配的空间，需要主动释放
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
* 根据父级路径，文件名，获取文件的路径字符串
* @param desPath :  文件对应的全路径
* @param parentPath 输入参数， 父级路径
* @param fileName  输入参数，  路径下的文件名称
* @return bool 是否创建成功
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