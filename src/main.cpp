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
	//practise.practise(projectDir);

	PractiseFaceCulling_4_4 practise = PractiseFaceCulling_4_4();
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