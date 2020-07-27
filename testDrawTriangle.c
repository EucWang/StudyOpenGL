//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
#include "shaders.h"
#include <stdio.h>
#include "test.h"

unsigned int VBO;
unsigned int VAO;

const char* fragmentShaderSource = "#version 330 core\n"     //�汾���� OpenGl3.3 core����ģʽ
"out vec4 FragColor;"            //Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ��
"void main(){"                     //������C���Ե�main����
"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"   //���ǽ�һ��alphaֵΪ1.0(1.0������ȫ��͸��)���ٻ�ɫ��vec4��ֵ����ɫ�����
"}";

const char* vertexShaderSource = "#version 330 core\n"   //�汾���� OpenGl3.3 core����ģʽ
"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) �趨�����������λ��ֵ��in �ؼ��֣� vec3 ����3d���ꣻ aPos ���������
"void main() {\n"                                  //������C���Ե�main����
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position����  Ԥ����ı�����vec4���͵ģ� ��main�������Ĭ�Ͻ�gl_Position��ֵ��Ϊ������ɫ�������
"}";

float vertices[] = {
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 0.0f,  0.5f, 0.0f
};

int linkVertexAttribute();

//�Զ����ѭ�����е���Ⱦ����
void render(unsigned int shaderProgram);

int drawTriangle() {
	glfwInit();  //��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //���ǽ����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// ������ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)
	//���ʹ�õ���Mac OS Xϵͳ���㻹��Ҫ���������д��뵽��ĳ�ʼ����������Щ���ò���������
#ifdef __APPLE__
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ���������� ������������ʾ������ڵ����ƣ����⣩
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("%s\n", "Failed to create GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  //��ʼ��GLAD
		printf("%s\n", "Failed to initialize GLAD");
		return -1;
	}

	//�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, 800, 600);
	//ע������ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ��������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	////����2����ɫ���� ������ɫ���� Ƭ����ɫ��
	int shaderProgram = makeShaderProgram(vertexShaderSource, fragmentShaderSource); 
	if (shaderProgram < 0) {
		return -1;
	}
	linkVertexAttribute(shaderProgram);

	//��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ�������С�
	//glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	while (!glfwWindowShouldClose(window)){
		processInput(window);  //����

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //������
		glClear(GL_COLOR_BUFFER_BIT);   

		render(shaderProgram);

		// glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��
		// ������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(window);

		//glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬��
		//�����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
	glfwTerminate();
	return 0;
}

//���Ǳ�������Ⱦǰָ��OpenGL����ν��Ͷ������ݡ�
//������ɫ����������ָ���κ��Զ�������Ϊ��ʽ�����롣
//��ʹ����к�ǿ������Ե�ͬʱ��������ȷ��ζ�����Ǳ����ֶ�ָ���������ݵ���һ�����ֶ�Ӧ������ɫ������һ���������ԡ�
//λ�����ݱ�����Ϊ32λ��4�ֽڣ�����ֵ��
//ÿ��λ�ð���3��������ֵ��
//����3��ֵ֮��û�п�϶��������ֵ�����⼸��ֵ�������н�������(Tightly Packed)��
//�����е�һ��ֵ�ڻ��忪ʼ��λ�á�
int linkVertexAttribute(unsigned int shaderProgram) {

	//ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ���
	//��һ������ָ������Ҫ���õĶ������ԡ�
	//���ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0��
	//��Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
	//�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	//����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
	//���ĸ������Ƿ�ϣ�����ݱ���׼��(Normalize)�������ΪGL_TRUE�����ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
	//�������������(Stride)����ʾ�������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
    //Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶��
	//��������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
	//һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ����������ӣ���ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
	//������������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//���������Ѿ�������OpenGL����ν��Ͷ������ݣ�
	//��������Ӧ��ʹ��glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	//��������Ĭ���ǽ��õġ�


	//���ж������Ѿ����ú��ˣ�
	//����ʹ��һ�����㻺����󽫶������ݳ�ʼ���������У�������һ�������һ��Ƭ����ɫ����
	//��������OpenGL��ΰѶ����������ӵ�������ɫ���Ķ��������ϡ�
	//��OpenGL�л���һ�����壬���������������

	//����һ��VAO����
	glGenVertexArrays(1, &VAO);
	//Ҫ��ʹ��VAO��Ҫ����ֻ��ʹ��glBindVertexArray��VAO��
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);  //ʹ��glGenBuffers������һ������ID����һ��VBO����
	//ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	//����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//����glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	//glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	//��һ��������Ŀ�껺�������,���㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ����
	//�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	//����������������ϣ�����͵�ʵ�����ݡ�
	//���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
	//GL_STATIC_DRAW �����ݲ���򼸺�����ı�;GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶ�;GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���������Ѿ��Ѷ������ݴ������Կ����ڴ��У���VBO������㻺��������

	//1 ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 1;
}

void render(unsigned int shaderProgram) {
	glUseProgram(shaderProgram);  //ʹ�õ�ǰ�������ɫ��
	glBindVertexArray(VAO);       //֮ǰ����Ķ����������ã���VBO�Ķ������ݣ�ͨ��VAO��Ӱ󶨣�������ͼԪ
	glDrawArrays(GL_TRIANGLES, 0, 3);  //����������
}