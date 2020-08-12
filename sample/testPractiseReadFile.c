#include "../include/test.h"
#include "../include/shaderSource.h"
#include "../include/fileUtil.h"
#include <stdio.h>

void testReadFile() {
	char* str = "D:/workspace/cppWorkspace/StudyOpenGL/src/testPractiseReadFile.c";
	//char* str = "../src/testPractiseReadFile.c";
	char* retVal;
	readStrFromFile(str, &retVal);
	printf("%s\n", retVal);
}