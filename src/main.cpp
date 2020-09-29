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
#include "PractiseFrameBuffers_4_5.h"
#include "PractiseFrameBuffer_kernel_4_5.h"
#include "PractiseCubeMap_4_6.h"
#include "PractiseAdvancedData_4_7.h"
#include "PractiseAdvancedGLSL_4_8.h"
#include "PractiseAdvancedGLSL_4_8_2.h"
#include "PractiseGeometryShader_4_9.h"
#include "PractiseGeometryShader_4_9_2.h"
#include "PractiseInstancing_4_10.h"
#include "PractiseAntiAliasing_4_11.h"
#include "PractiseAdvancedLighting_5_1.h"
#include "PractiseGammaCorrection_5_2.h"
#include "PractiseGammaCorrection_5_2_2.h"

using namespace std;

static void testMovePos();
static void testScale();

static void showMatrix(glm::mat4  trans) {
	std::cout << "[ " << trans[0][0] << ",\t" << trans[0][1] << ",\t" << trans[0][2] << ",\t" << trans[0][3] << "," << std::endl
		<< " " << trans[1][0] << ",\t" << trans[1][1] << ",\t" << trans[1][2] << ",\t" << trans[1][3] << "," << std::endl
		<< " " << trans[2][0] << ",\t" << trans[2][1] << ",\t" << trans[2][2] << ",\t" << trans[2][3] << "," << std::endl
		<< " " << trans[3][0] << ",\t" << trans[3][1] << ",\t" << trans[3][2] << ",\t" << trans[3][3] << "]" << std::endl;
}

int main(int argc, char* argv[]) {

	char* debugDir;
	char* projectDir;
	//argv[0] �ǵ�ǰ���е�exe�����·��
	//���ȵõ�debug·��
	//Ȼ��õ���Ŀ·��
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
	//PractiseFaceCulling_4_4 practise = PractiseFaceCulling_4_4();
	//PractiseFrameBuffers_4_5 practise = PractiseFrameBuffers_4_5();

	//PractiseFrameBuffer_kernel_4_5 practise = PractiseFrameBuffer_kernel_4_5();

	//PractiseCubeMap_4_6 practise = PractiseCubeMap_4_6();
	//PractiseAdvancedData_4_7 practise = PractiseAdvancedData_4_7();
	//PractiseAdvancedGLSL_4_8 practise = PractiseAdvancedGLSL_4_8();
	//PractiseAdvancedGLSL_4_8_2 practise = PractiseAdvancedGLSL_4_8_2();
	//practise.practise(projectDir);
	//practise.practise2(projectDir);

	//PractiseGeometryShader_4_9 practise = PractiseGeometryShader_4_9();
	//PractiseGeometryShader_4_9_2 practise = PractiseGeometryShader_4_9_2();
	
	//PractiseInstancing_4_10 practise = PractiseInstancing_4_10();
	//practise.practise(projectDir);
	//practise.practise2(projectDir);
	//practise.practise3(projectDir);
	//practise.practise4(projectDir);

	//PractiseAntiAliasing_4_11 practise = PractiseAntiAliasing_4_11();
	//practise.practise0(projectDir);
	//practise.practise(projectDir);
	//practise.practise1(projectDir);
	//practise.practise2(projectDir);

	//PractiseAdvancedLighting_5_1 practise = PractiseAdvancedLighting_5_1();
	PractiseGammaCorrection_5_2 practise = PractiseGammaCorrection_5_2();
	//PractiseGammaCorrection_5_2_2 practise = PractiseGammaCorrection_5_2_2();
	practise.practise(projectDir);

	//free(debugDir);
	//free(projectDir);
	return 0;
}

//void testMovePos() {

	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

	//�Խ���Ϊ1��4X4��λ����
	//glm::mat4 trans = glm::mat4(1.0f);

	//����λ���� �õ���Ҫ��λ���� (1,1,0)�ı任����
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

	//����ˣ� �Ӷ���vec����λ��(1, 1, 0)�ĵ�λ
	//vec = trans * vec;

	//std::cout << "x = " << vec.x
	//	<< ", y = " << vec.y
	//	<< ", z = " << vec.z << std::endl;

//}

//void testScale() {
	//4x4��λ����
	//glm::mat4 trans = glm::mat4(1.0f);
	//������ʱ����ת90�ȣ� ʹ��rotate()����
	//��һ���� ָ����λ����
	//�ڶ�������ָ����Ҫ��ʱ����ת�Ļ��ȣ�������radians()�����ѽǶ�ת���ɻ���
	//����������ָ�� ��ת����ϵ�� ����ָ����z����ת����ֻ��xyƽ������ת
//	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

	//�Ե�λ����������ţ�ʹ��scale()����
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

//	showMatrix(trans);
//}