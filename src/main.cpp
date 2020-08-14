#include <stdio.h>
#include <stdlib.h>

#include "../include/test.h"
#include "../include/fileUtil.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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
		printf("%s\n", "main() failed, because getParentDir() call failed, cannot get projectDir");
		return -1;
	}

	printf("%s\n", "cpp main() run..");
	//testMovePos();
	//testScale();
	//practiseDrawWithTransform11(projectDir);
	//practiseDrawWithPerspective12(projectDir);
	//practiseDrawWithPerspective13(projectDir);
	//practiseDrawWithPerspective14(projectDir);
	//practiseDrawWithPerspective15(projectDir);
	//practiseDrawWithPerspective16(projectDir);
	practiseDrawWithCamera(projectDir);

	free(debugDir);
	free(projectDir);
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