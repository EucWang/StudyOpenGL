#pragma once

#include <string>
#include <iostream>

#include "camera.h"
#include "fileUtil.h"
#include "mesh.h"
#include "MyShader.h"
#include "model.h"
#include "RenderUtil.h"


class PractiseAdvancedGLSL_4_8_2
{
public:

	const char* vertFile = "shader/_4_8_advanced_glsl2.vert";
	const char* fragFile = "shader/_4_8_advanced_glsl2.frag";

	const char* vertFile2 = "shader/_4_8_advanced_glsl3.vert";
	const char* fragFile2 = "shader/_4_8_advanced_glsl3.frag";

	const char* imgFileMarble2 = "images/marble2.jpg";
	const char* imgFileMarbleInside = "images/marble_inside.jpg";

	const float cubePosition[36 * 3] = {
		// Back face
		-0.5f, -0.5f, -0.5f, // Bottom-left
		 0.5f,  0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // bottom-right         
		 0.5f,  0.5f, -0.5f, // top-right
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f,  0.5f, -0.5f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-right
		 0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f,  0.5f, // top-left
		-0.5f, -0.5f,  0.5f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f,  0.5f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right         
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f, -0.5f, -0.5f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f, // top-left
		 0.5f,  0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right     
		 0.5f,  0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f,  0.5f,  0.5f, // bottom-left        
	};

	const float cubeTexCoords[36 * 2] = {

		0.0f, 0.0f, // Bottom-left
		1.0f, 1.0f, // top-right
		1.0f, 0.0f, // bottom-right         
		1.0f, 1.0f, // top-right
		0.0f, 0.0f, // bottom-left
		0.0f, 1.0f, // top-left

		0.0f, 0.0f, // bottom-left
		1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, // top-right
		1.0f, 1.0f, // top-right
		0.0f, 1.0f, // top-left
		0.0f, 0.0f, // bottom-left

		1.0f, 0.0f, // top-right
		1.0f, 1.0f, // top-left
		0.0f, 1.0f, // bottom-left
		0.0f, 1.0f, // bottom-left
		0.0f, 0.0f, // bottom-right
		1.0f, 0.0f, // top-right

		1.0f, 0.0f, // top-left
		0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, // top-right         
		0.0f, 1.0f, // bottom-right
		1.0f, 0.0f, // top-left
		0.0f, 0.0f, // bottom-left     

		0.0f, 1.0f, // top-right
		1.0f, 1.0f, // top-left
		1.0f, 0.0f, // bottom-left
		1.0f, 0.0f, // bottom-left
		0.0f, 0.0f, // bottom-right
		0.0f, 1.0f, // top-right

		0.0f, 1.0f, // top-left
		1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, // top-right     
		1.0f, 0.0f, // bottom-right
		0.0f, 1.0f, // top-left
		0.0f, 0.0f  // bottom-left        
	};

	/**
	* Ƭ����ɫ�� �� 
	* ʹ�� gl_FragCoord.x/.y ����Ƭ���ڴ����е�λ����Ӧ�ò�ͬ����ɫ
	**/
	int practise(string projectDir);

	/**
	* Ƭ����ɫ�� ��
	* ʹ�� gl_FrontFacing 
	* �� �������ڲ����ⲿʹ�ò�ͬ������
	* ע�⣬����㿪�������޳�����Ϳ����������ڲ������ˣ�
	* ����������ʹ��gl_FrontFacing��û�������ˡ�
	*/
	int practise2(string projectDir);



	/**
	* gl_FragDepth��������������ǿ���ʹ����������ɫ��������Ƭ�ε����ֵ��
	* �Լ��������ֵ��һ���ܴ��ȱ�㣬ֻҪ������Ƭ����ɫ���ж�gl_FragDepth����д�룬
	* OpenGL�ͻᣨ����Ȳ���С�������۵��������������е���ǰ��Ȳ���(Early Depth Testing)��
	* �������õ�ԭ���ǣ�OpenGL�޷���Ƭ����ɫ������֮ǰ��֪Ƭ�ν�ӵ�е����ֵ����ΪƬ����ɫ�����ܻ���ȫ�޸�������ֵ��
	*
	* ��OpenGL 4.2�������Կ��Զ����߽���һ���ĵ��ͣ���Ƭ����ɫ���Ķ���ʹ���������(Depth Condition)��������gl_FragDepth������
	*
	* layout (depth_<condition>) out float gl_FragDepth;
	* condition����Ϊ�����ֵ��
	* ------------------------------------------------------------------------
	*	����			| ����
	* ------------------------------------------------------------------------
	*	any			| Ĭ��ֵ����ǰ��Ȳ����ǽ��õģ������ʧ�ܶ�����
	*	greater		| ��ֻ�������ֵ��gl_FragCoord.z����
	*	less		| ��ֻ�������ֵ��gl_FragCoord.z��С
	*	unchanged	| �����Ҫд��gl_FragDepth���㽫ֻ��д��gl_FragCoord.z��ֵ
	* ------------------------------------------------------------------------
	*
	* ͨ���������������Ϊgreater����less��OpenGL���ܼ�����ֻ��д��ȵ�ǰƬ�����ֵ������߸�С��ֵ�ˡ�
	* ע���������ֻ��OpenGL 4.2�汾�����ϲ��ṩ��
	*/
	int practise3(string projectDir);
};

