#include "util.h"

/**
* 根据系统时间，获取的数值在0.0 - 1.0之间变化
*/
float getFVal() {
	float retVal = (float)(sin((float)glfwGetTime()) / 2.0f + 0.5f);
	//std::cout << "retVal  = " << retVal << std::endl;
	return retVal;
}
