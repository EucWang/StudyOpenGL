#include "shaders.h"
#include "test.h"
#include "fileUtil.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

int main1(int argc, char * argv[]) {
	char* debugDir;
	char * projectDir;
	//argv[0] �ǵ�ǰ���е�exe�����·��
	//���ȵõ�debug·��
	//Ȼ��õ���Ŀ·��
	if(!(getParentDir(argv[0], &debugDir) && getParentDir(debugDir, &projectDir))){
		printf("%s\n", "main() failed, because getParentDir() call failed, cannot get projectDir");
		return -1;
	}

	//drawTriangle();
	//drawRectangle();
	//drawUnfillRectangle();
	//practiseDrawTriangles1();
	//practiseDrawTriangles2();
	//practiseDrawTriangles4();
	//practiseDrawTriangles5();
	//practiseDrawTriangle6();
	//practiseDrawTriangle7();
	//testReadFile();
	//practiseDrawTriangle8();
	//testLoadStbImage();
	//testPractiseDrawWithTexure9(projectDir);
	//testGetParentDir();
	//testGetChildPath();
	//practiseDrawWithTexture10(projectDir);

	free(debugDir);
	free(projectDir);
	return 0;
}