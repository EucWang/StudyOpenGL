#ifndef  FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus    //__cplusplus是cpp中自定义的一个宏
extern "C" {          //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
#endif


/**
	从文件中读取全部字符串
	@param filePath : 输入参数， 文件路径
	@param desStr : 输出参数， 文件中的读取到的全部字符， malloc分配的内存，必须手动释放
	@return bool : 读取成功还是失败
*/
bool readStrFromFile(/*in*/const char* filePath, /*out*/char** desStr);

/*
* 获取父级目录的字符串
* @param path : 输出参数， 需要处理的路径
* @param parentPath : 输出参数， path的父级路径， malloc分配的内存，必须手动释放
* @return bool 是否创建成功
*/
bool getParentDir(/*in*/const char * path, /*out*/char ** parentPath);

/**
* 根据父级路径，文件名，获取文件的路径字符串
* @param desPath :  文件对应的全路径， malloc分配的内存，必须手动释放
* @param parentPath 输入参数， 父级路径
* @param fileName  输入参数，  路径下的文件名称
* @return bool 是否创建成功
*/
bool getChildPath(/*out*/char** desPath, /*in*/const char* parentPath, /*in*/const char* fileName);

#ifdef __cplusplus
}
#endif
#endif // ! FILE_UTIL_H
