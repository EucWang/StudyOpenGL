#ifndef TEST_H_
#define TEST_H_

static char* cFragmentShaderSource = "#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
"void main(){"                     //类似于C语言的main函数
"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"   //我们将一个alpha值为1.0(1.0代表完全不透明)的橘黄色的vec4赋值给颜色输出。
"}";

static char* cFragmentShaderSourceYellow = "#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
"void main(){"                     //类似于C语言的main函数
"\tFragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);"   //我们将一个alpha值为1.0(1.0代表完全不透明)的黄色的vec4赋值给颜色输出。
"}";

static char* cVertexShaderSource = "#version 330 core\n"   //版本声明 OpenGl3.3 core核心模式
"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) 设定了输入变量的位置值；in 关键字； vec3 类型3d坐标； aPos 输入变量；
"void main() {\n"                                  //类似于C语言的main函数
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position变量  预定义的变量，vec4类型的， 在main函数最后默认将gl_Position的值作为顶点着色器的输出
"}";

int drawTriangle();

int drawRectangle();

int drawUnfillRectangle();

int practiseDrawTriangles1();

int practiseDrawTriangles2();

int practiseDrawTriangles3();

int practiseDrawTriangles4();

#endif