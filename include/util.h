#ifndef  UTIL_H_
#define UTIL_H_


#ifdef __cplusplus    //__cplusplus是cpp中自定义的一个宏
extern "C" {          //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
#endif

#include <GLFW/glfw3.h>
#include <math.h>

float getFVal();

#ifdef __cplusplus
}
#endif

#endif // ! UTIL_H_