#include "../include/shaders.h"
#include "../include/shaderSource.h"
#include "../include/list.h"
#include "../include/fileUtil.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
* @param vertexPath       顶点着色器的文件路径
* @param fragmentPath  片段插值器的文件路径
* @shaderId  : int *  输出参数，
* @return bool  是否创建成功
*/
bool createShaderProgram(char* vertexPath, char* fragmentPath, int * shaderId) {
	char* vertexStr;
	char* fragStr;
	bool readVertexSuccess = readStrFromFile(vertexPath, &vertexStr);
	bool readFragSuccess = readStrFromFile(fragmentPath, &fragStr);
	if (!readVertexSuccess && !readFragSuccess) {
		return false;
	}

	if (vertexStr == NULL) { printf("%s%s\n", "failed to read from vertexPath file, file path is ", vertexPath);	}

	if (fragStr == NULL) { printf("%s%s\n", "failed to read from fragmentPath file, file path is ", fragmentPath);	}

	int tShader = makeShaderProgram(vertexStr, fragStr);
	if (tShader < 0) { return false;	}

	*shaderId = tShader;

	return true;
}


//片段着色器所做的是计算像素最后的颜色输出。
//返回 着色器的id， 如果小于0： 小时创建失败
static int makeFragmentShader(const char* tfragShaderSource) {
	unsigned int fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    //创建片段着色器
	glShaderSource(fragmentShader, 1, &tfragShaderSource, NULL);  //输入片段着色器的源码
	glCompileShader(fragmentShader);                      //编译片段着色器

	//检测在调用glCompileShader后编译是否成功了
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("%s%s\n", "ERROR:SHADER:FRAGMENT:COMPILATION_FAILED", infoLog);
		return -1;
	}
	return fragmentShader;
}

//编译创建着色器
//返回 着色器的id， 如果小于0： 小时创建失败
int makeVertexShader(const char* tvertexShaderSource) {
	unsigned int vertexShader;

	//编译，创建顶点着色器
	//把需要创建的着色器类型以参数形式提供给glCreateShader,
	//由于我们正在创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//下一步我们把这个着色器源码附加到着色器对象上，然后编译它
	//第一个参数是要编译的着色器对象
	//第二参数指定了传递的源码字符串数量，这里只有一个
	//第三个参数是顶点着色器真正的源码
	glShaderSource(vertexShader, 1, &tvertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//检测在调用glCompileShader后编译是否成功了
	int success;        //是否编译成功的状态
	char infoLog[512];  //错误信息的容器
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //获取编译之后的状态

	if (!success) {  //编译失败
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  //获取编译之后的错误信息日志
		printf("%s%s\n", "ERROR:SHADER::VERTEX::COMPILATION_FAILED", infoLog); //控制台输出
		return -1;
	}

	return vertexShader;
}

//链接生成最终的着色器程序对象
//返回着色器程序对象id， 如果小于0： 小时创建失败
/**
* @param tvertexShaderSource  输入参数， 顶点着色器的程序的字符串
* @param tfragShaderSource  输入参数， 片段着色器的程序字符串
* @return int  返回创建的shaderProgram的id值
*/
int makeShaderProgram(const char* tvertexShaderSource, const char* tfragShaderSource) {
	if (tvertexShaderSource == NULL && tfragShaderSource == NULL)	{ return -1; }
	
	unsigned int shaderProgram;
	int mVertexShader = -1; 
	int mFragmentShader = -1;

	//glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	shaderProgram = glCreateProgram();
	if (tvertexShaderSource != NULL){
		mVertexShader = makeVertexShader(tvertexShaderSource);
		if (mVertexShader < 0) { return -1;	 }
		//把之前编译的着色器附加到程序对象上
		glAttachShader(shaderProgram, mVertexShader);
	}

	if (tfragShaderSource != NULL)	{
		mFragmentShader = makeFragmentShader(tfragShaderSource);
		if (mFragmentShader < 0) { return -1; }
		//把之前编译的着色器附加到程序对象上
		glAttachShader(shaderProgram, mFragmentShader);
	}

	//用glLinkProgram链接它们
	glLinkProgram(shaderProgram);

	//检测在调用glLinkProgram后编译是否成功了
	int success;        //是否编译成功的状态
	char infoLog[512];  //错误信息的容器
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //获取链接之后的状态
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s%s\n", "ERROR:SHADER::PROGRAM::LINK_FAILED", infoLog); //控制台输出
		return -1;
	}

	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
	if (mVertexShader != -1)	{ glDeleteShader(mVertexShader);	}
	if (mFragmentShader != -1){ glDeleteShader(mFragmentShader); }

	return shaderProgram;
}