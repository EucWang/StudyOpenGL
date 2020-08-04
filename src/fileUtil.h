#ifndef  FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdbool.h>

/**
	从文件中读取全部字符串
	@param filePath : 文件路径
	@param desStr : 文件中的读取到的全部字符
	@return bool : 读取成功还是失败
*/
bool readStrFromFile(const char* filePath, char** desStr);

#endif // ! FILE_UTIL_H
