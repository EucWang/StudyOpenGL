#ifndef SHADER_SOURCE_H_
#define SHADER_SOURCE_H_

static const char* cFragmentShaderSource ="#version 330 core\n"     //�汾���� OpenGl3.3 core����ģʽ
"out vec4 FragColor;"            //Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ��
"void main(){"                     //������C���Ե�main����
"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"   //���ǽ�һ��alphaֵΪ1.0(1.0������ȫ��͸��)���ٻ�ɫ��vec4��ֵ����ɫ�����
"}";

static const char* cFragmentShaderSourceYellow = "#version 330 core\n"     //�汾���� OpenGl3.3 core����ģʽ
"out vec4 FragColor;"            //Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ��
"void main(){"                     //������C���Ե�main����
"\tFragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);"   //���ǽ�һ��alphaֵΪ1.0(1.0������ȫ��͸��)�Ļ�ɫ��vec4��ֵ����ɫ�����
"}";


static const char* cVertexShaderSource = "#version 330 core\n"   //�汾���� OpenGl3.3 core����ģʽ
"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) �趨�����������λ��ֵ��in �ؼ��֣� vec3 ����3d���ꣻ aPos ���������
"void main() {\n"                                  //������C���Ե�main����
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position����  Ԥ����ı�����vec4���͵ģ� ��main�������Ĭ�Ͻ�gl_Position��ֵ��Ϊ������ɫ�������
"}";


static const char* dVertexShaderSource = "#version 330 core\n"   //�汾���� OpenGl3.3 core����ģʽ
"layout (location = 0) in vec3 aPos;\n"            //layout(location = 0) �趨�����������λ��ֵ��in �ؼ��֣� vec3 ����3d���ꣻ aPos ���������
"out vec4 vertexColor;"  //ΪƬ����ɫ��ָ��һ����ɫ���
"void main() {\n"                                  //������C���Ե�main����
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position����  Ԥ����ı�����vec4���͵ģ� ��main�������Ĭ�Ͻ�gl_Position��ֵ��Ϊ������ɫ�������
"\tvertexColor = vec4(0.5, 0.0, 0.0, 1.0);"  //�������������Ϊ����ɫ
"}";

static const char* dFragmentShaderSource = "#version 330 core\n"     //�汾���� OpenGl3.3 core����ģʽ
"out vec4 FragColor;"            //Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ��
"in vec4 vertexColor;"          //�Ӷ�����ɫ�����������������������ͬ��������ͬ��
"void main(){"                     //������C���Ե�main����
"\tFragColor = vertexColor;"   //���ǽ�һ��alphaֵΪ1.0(1.0������ȫ��͸��)�Ļ�ɫ��vec4��ֵ����ɫ�����
"}";

#endif // !SHADER_SOURCE_H_