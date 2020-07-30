#ifndef SHADER_SOURCE_H_
#define SHADER_SOURCE_H_

static const char* cFragmentShaderSource ="#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
"void main(){"                     //类似于C语言的main函数
"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"   //我们将一个alpha值为1.0(1.0代表完全不透明)的橘黄色的vec4赋值给颜色输出。
"}";

static const char* cFragmentShaderSourceYellow = "#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
"void main(){"                     //类似于C语言的main函数
"\tFragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);"   //我们将一个alpha值为1.0(1.0代表完全不透明)的黄色的vec4赋值给颜色输出。
"}";


static const char* cVertexShaderSource = "#version 330 core\n"   //版本声明 OpenGl3.3 core核心模式
"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) 设定了输入变量的位置值；in 关键字； vec3 类型3d坐标； aPos 输入变量；
"void main() {\n"                                  //类似于C语言的main函数
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position变量  预定义的变量，vec4类型的， 在main函数最后默认将gl_Position的值作为顶点着色器的输出
"}";

static const char* dVertexShaderSource = "#version 330 core\n"   //版本声明 OpenGl3.3 core核心模式
"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) 设定了输入变量的位置值；in 关键字； vec3 类型3d坐标； aPos 输入变量；
"out vec4 vertexColor;"  //为片段着色器指定一个颜色输出
"void main() {\n"                                  //类似于C语言的main函数
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position变量  预定义的变量，vec4类型的， 在main函数最后默认将gl_Position的值作为顶点着色器的输出
"\tvertexColor = vec4(0.5, 0.0, 0.0, 1.0);"  //把输出变量设置为暗红色
"}";

static const char* dFragmentShaderSource = "#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
"in vec4 vertexColor;"          //从顶点着色器传来的输入变量（名称相同，类型相同）
"void main(){"                     //类似于C语言的main函数
"\tFragColor = vertexColor;"   //我们将一个alpha值为1.0(1.0代表完全不透明)的黄色的vec4赋值给颜色输出。
"}";

//通过uniform关键字的变量，来动态改变图形的颜色
static const char* eFragmentShaderSource = "#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
"uniform vec4 ourColor;"          //从顶点着色器传来的输入变量（名称相同，类型相同）
"void main(){"                     //类似于C语言的main函数
"\tFragColor = ourColor;"   //我们将一个alpha值为1.0(1.0代表完全不透明)的黄色的vec4赋值给颜色输出。
"}";

static const char* fVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main(){\n"
"\tgl_Position = vec4(aPos, 1.0);\n"
"\tourColor = aColor;\n"
"}\n";

static const char* fFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main(){\n"
"FragColor = vec4(ourColor, 1.0f);\n"
"}\n";

#endif // !SHADER_SOURCE_H_