#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//我们同样也希望能够在GLFW中实现一些输入控制，这可以通过使用GLFW的几个输入函数来完成。
//我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。
void processInput(GLFWwindow* window);

//循环中的自定义渲染
void render(GLFWwindow * window);

//编译创建顶点着色器
int makeVertexShader();

//编译创建片段着色器
int makeFragmentShader();

int linkVertexAttribute();


//着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
//如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。
//已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
int makeShaderProgram();

//正式画图
int someOpenGlFunctionThatDrawsOurTriangle();


unsigned int VBO;
unsigned int VAO;

int main() {
	//printf("%s\n", "Hello glad.");
	//return 0;

	glfwInit();  //初始化GLFW

	//使用glfwWindowHint函数来配置GLFW
	// glfwWindowHint函数的第一个参数代表选项的名称，
	// 我们可以从很多以GLFW_开头的枚举值中选择；
	// 第二个参数接受一个整型，用来设置这个选项的值。
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //我们将主版本号(Major)和次版本号(Minor)都设为3。
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// 这样明确告诉GLFW我们使用的是核心模式(Core-profile),
	// 明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集（没有我们已不再需要的向后兼容特性）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//如果使用的是Mac OS X系统，你还需要加下面这行代码到你的初始化代码中这些配置才能起作用
#ifdef __APPLE__
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。
	// 第三个参数表示这个窗口的名称（标题），
	// 这里我们使用"LearnOpenGL"，当然你也可以使用你喜欢的名称。
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("%s\n", "Failed to create GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("%s\n", "Failed to initialize GLAD");
		return -1;
	}

	//两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	glViewport(0, 0, 800, 600);
	//注册这个回调函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int shaderProgram = makeShaderProgram();
	if (shaderProgram < 0) {
		return -1;
	}
	linkVertexAttribute(shaderProgram);


	//渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行。
	//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	while (!glfwWindowShouldClose(window)){
		//输入
		processInput(window);

		// 渲染指令
		//render(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //画背景
		glClear(GL_COLOR_BUFFER_BIT);   

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
		// 它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);


		//glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
		//并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//正确释放/删除之前的分配的所有资源。
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	//检查用户是否按下了返回键(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
		//通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW
		glfwSetWindowShouldClose(window, true); 
	}
}


//我们必须在渲染前指定OpenGL该如何解释顶点数据。
//顶点着色器允许我们指定任何以顶点属性为形式的输入。
//这使其具有很强的灵活性的同时，它还的确意味着我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性。
//位置数据被储存为32位（4字节）浮点值。
//每个位置包含3个这样的值。
//在这3个值之间没有空隙（或其他值）。这几个值在数组中紧密排列(Tightly Packed)。
//数据中第一个值在缓冲开始的位置。
int linkVertexAttribute(unsigned int shaderProgram) {

	//使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了
	//第一个参数指定我们要配置的顶点属性。
	//还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。
	//因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	//第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	//第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
	//第四个参数是否希望数据被标准化(Normalize)。如果置为GL_TRUE，数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	//第五个参数步长(Stride)，表示在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
    //要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
	//所以我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
	//一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
	//第六个参数表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//现在我们已经定义了OpenGL该如何解释顶点数据，
	//我们现在应该使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；
	//顶点属性默认是禁用的。


	//所有东西都已经设置好了：
	//我们使用一个顶点缓冲对象将顶点数据初始化至缓冲中，建立了一个顶点和一个片段着色器，
	//并告诉了OpenGL如何把顶点数据链接到顶点着色器的顶点属性上。
	//在OpenGL中绘制一个物体，代码会像是这样：

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);  //使用glGenBuffers函数和一个缓冲ID生成一个VBO对象

	//0 复制顶点数组到缓冲中供Opengl使用
	glBindVertexArray(VAO);

	//使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	//从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
	//glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
	//第一个参数是目标缓冲的类型,顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上
	//第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	//第三个参数是我们希望发送的实际数据。
	//第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
	//GL_STATIC_DRAW ：数据不会或几乎不会改变;GL_DYNAMIC_DRAW：数据会被改变很多;GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//现在我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理。

	 
	//1 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//2. 当我们渲染一个物体时要使用着色器程序
	//glUseProgram(shaderProgram);

	//3. 绘制物体
	//someOpenGlFunctionThatDrawsOurTriangle();

	return 1;
}

//链接生成最终的着色器程序对象
//返回着色器程序对象id， 如果小于0： 小时创建失败
int makeShaderProgram() {
	unsigned int shaderProgram;

	//glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	shaderProgram = glCreateProgram();

	int mVertexShader = makeVertexShader();
	int mFragmentShader = makeFragmentShader();

	if (mVertexShader < 0 || mFragmentShader < 0) {
		return -1;
	}

	//把之前编译的着色器附加到程序对象上
	glAttachShader(shaderProgram, mVertexShader);
	glAttachShader(shaderProgram, mFragmentShader);

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

	//调用glUseProgram函数，用刚创建的程序对象作为它的参数，以激活这个程序对象
	//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。
	glUseProgram(shaderProgram);  

	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);

	return shaderProgram;
}


//编译创建着色器
//返回 着色器的id， 如果小于0： 小时创建失败
int makeVertexShader() {
	unsigned int vertexShader;

	char* vertexShaderSource = "#version 330 core\n"   //版本声明 OpenGl3.3 core核心模式
		"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) 设定了输入变量的位置值；in 关键字； vec3 类型3d坐标； aPos 输入变量；
		"void main() {\n"                                  //类似于C语言的main函数
		"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position变量  预定义的变量，vec4类型的， 在main函数最后默认将gl_Position的值作为顶点着色器的输出
		"}";

	//编译，创建顶点着色器
	//把需要创建的着色器类型以参数形式提供给glCreateShader,
	//由于我们正在创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//下一步我们把这个着色器源码附加到着色器对象上，然后编译它
	//第一个参数是要编译的着色器对象
	//第二参数指定了传递的源码字符串数量，这里只有一个
	//第三个参数是顶点着色器真正的源码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//检测在调用glCompileShader后编译是否成功了
	int success;        //是否编译成功的状态
	char infoLog[512];  //错误信息的容器
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //获取编译之后的状态

	if (!success){  //编译失败
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  //获取编译之后的错误信息日志
		printf("%s%s\n", "ERROR:SHADER::VERTEX::COMPILATION_FAILED", infoLog); //控制台输出
		return -1;
	}

	return vertexShader;
}

//片段着色器所做的是计算像素最后的颜色输出。
//返回 着色器的id， 如果小于0： 小时创建失败
int makeFragmentShader() {
	unsigned int fragmentShader; 
	
	char* fragmentShaderSource = "#version 330 core\n"     //版本声明 OpenGl3.3 core核心模式
		"out vec4 FragColor;"            //片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，
		"void main(){"                     //类似于C语言的main函数
		"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"   //我们将一个alpha值为1.0(1.0代表完全不透明)的橘黄色的vec4赋值给颜色输出。
		"}";

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    //创建片段着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  //输入片段着色器的源码
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