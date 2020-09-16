//#include <stdio.h>
//#include <stdlib.h>

#include "../include/test.h"
#include "../include/fileUtil.h"
#include "../include/TestPractiseLight27.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <TestPractiseDepth28.h>
#include <PractiseBlending29.h>
#include "PractiseBlending30.h"

//#include "../include/PractiseFaceCulling_4_4.h"
#include "PractiseFaceCulling_4_4.h"

using namespace std;

static void testMovePos();
static void testScale();


//void showMatrix(glm::mat4  trans) {
//	std::cout << "[ " << trans[0][0] << ",\t" << trans[0][1] << ",\t" << trans[0][2] << ",\t" << trans[0][3] << "," << std::endl
//		<< " " << trans[1][0] << ",\t" << trans[1][1] << ",\t" << trans[1][2] << ",\t" << trans[1][3] << "," << std::endl
//		<< " " << trans[2][0] << ",\t" << trans[2][1] << ",\t" << trans[2][2] << ",\t" << trans[2][3] << "," << std::endl
//		<< " " << trans[3][0] << ",\t" << trans[3][1] << ",\t" << trans[3][2] << ",\t" << trans[3][3] << "]" << std::endl;
//}


int main(int argc, char* argv[]) {

	char* debugDir;
	char* projectDir;
	//argv[0] 是当前运行的exe程序的路径
	//首先得到debug路径
	//然后得到项目路径
	if (!(getParentDir(argv[0], &debugDir) && getParentDir(debugDir, &projectDir))) {
		//printf("%s\n", "main() failed, because getParentDir() call failed, cannot get projectDir");
		return -1;
	}

	//printf("%s\n", "cpp main() run..");
	//testMovePos();
	//testScale();

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

	//practiseDrawWithTransform11(projectDir);
	//practiseDrawWithPerspective12(projectDir);
	//practiseDrawWithPerspective13(projectDir);
	//practiseDrawWithPerspective14(projectDir);
	//practiseDrawWithPerspective15(projectDir);
	//practiseDrawWithPerspective16(projectDir);
	//practiseDrawWithCamera(projectDir);
	//practise2Light(projectDir);
	//practiseLight19(projectDir);
	//practiseLight20(projectDir);
	//practiseLight21(projectDir);
	//practiseLight22(projectDir);
	//practiseLight23(projectDir);
	//practiseLight24(projectDir);
	//practiseLight25(projectDir);
	//practiseLight26(projectDir);
	//practiseLight27(projectDir);

	//TestPractiseLight27 practise = TestPractiseLight27();
	//practise.practiseLight27(projectDir);

	//TestPractiseDepth28 test = TestPractiseDepth28();
	//test.practiseDepth28(projectDir);

	//PractiseBlending29 practise = PractiseBlending29();
	//practise.practiseDepth29(projectDir);

	//PractiseBlending30 practise = PractiseBlending30();
	//practise.practise(projectDir);

	PractiseFaceCulling_4_4 practise = PractiseFaceCulling_4_4();
	practise.practise(projectDir);

	//free(debugDir);
	//free(projectDir);
	return 0;
}

//void testMovePos() {

	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

	//对角线为1的4X4单位矩阵
	//glm::mat4 trans = glm::mat4(1.0f);

	//将单位矩阵 得到需要的位移量 (1,1,0)的变换矩阵
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

	//矩阵乘， 从而让vec变量位移(1, 1, 0)的单位
	//vec = trans * vec;

	//std::cout << "x = " << vec.x
	//	<< ", y = " << vec.y
	//	<< ", z = " << vec.z << std::endl;

//}

//void testScale() {
	//4x4单位矩阵
	//glm::mat4 trans = glm::mat4(1.0f);
	//首先逆时针旋转90度， 使用rotate()方法
	//第一参数 指定单位矩阵
	//第二个参数指定需要逆时针旋转的弧度，这里用radians()方法把角度转换成弧度
	//第三个参数指定 旋转的轴系， 这里指定沿z轴旋转，既只在xy平面上旋转
//	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

	//对单位矩阵进行缩放，使用scale()方法
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

//	showMatrix(trans);
//}