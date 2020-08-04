#include "fileUtil.h"

#include "list.h"
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