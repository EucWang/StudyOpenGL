#include "util.h"

/**
* ����ϵͳʱ�䣬��ȡ����ֵ��0.0 - 1.0֮��仯
*/
float getFVal() {
	float retVal = (float)(sin((float)glfwGetTime()) / 2.0f + 0.5f);
	//std::cout << "retVal  = " << retVal << std::endl;
	return retVal;
}
