#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/fileUtil.h"

#include "../include/shaders.h"
#include "../include/test.h"
#include "../include/shaderSource.h"

int testGetParentDir() {
	//char* exePath = "D:/workspace/cppWorkspace/StudyOpenGL/Debug/FirstOpenGL.exe";
	char* exePath = "D:\\workspace\\cppWorkspace\\StudyOpenGL\\Debug\\FirstOpenGL.exe";
	char* parentPath;
	bool result = getParentDir(exePath, &parentPath);

	printf("%s%d%s%s\n", "result is ", result, ", parentPath is ", parentPath);

	char* superParentPath;
	result = getParentDir(parentPath, &superParentPath);
	printf("%s%d%s%s\n", "result is ", result, ", superParentPath is ", superParentPath);

	return 0;
}

int testGetChildPath() {
	//char* projectPath = "D:\\workspace\\cppWorkspace\\StudyOpenGL";
	char* projectPath = "D:/workspace/cppWorkspace/StudyOpenGL";
	char* fileName = "container.jpg";
	char* filePath;
	if (getChildPath(&filePath, projectPath, fileName)) {
		printf("%s%s\n", "filePath is :", filePath);
	}
	return 1;
}