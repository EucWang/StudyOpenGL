#pragma once

#include <string>
#include <iostream>

#include "camera.h"
#include "fileUtil.h"
#include "MyShader.h"
#include "RenderUtil.h"
//#include "mesh.h"
//#include "model.h"


class PractiseAdvancedGLSL_4_8_2 {
public:
	const char* vertFile = "shader/_4_8_advanced_glsl2.vert";
	const char* fragFile = "shader/_4_8_advanced_glsl2.frag";

	const char* vertFile2 = "shader/_4_8_advanced_glsl3.vert";
	const char* fragFile2 = "shader/_4_8_advanced_glsl3.frag";

	const char* vertFilePlane = "shader/_4_8_advanced_glsl3_2.vert";
	const char* fragFilePlane = "shader/_4_8_advanced_glsl3_2.frag";

	const char* imgFileMarble2 = "images/marble2.jpg";
	const char* imgFileMarbleInside = "images/marble_inside.jpg";

	const char* imgFilePlane = "images/plane2.jpg";

	//地面的坐标
	const float planePosition[6 * 3] = {
		// (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
		//this will cause the floor texture to repeat)
		// positions        
		 5.0f, -0.5f,  5.0f, 
		-5.0f, -0.5f,  5.0f, 
		-5.0f, -0.5f, -5.0f, 

		 5.0f, -0.5f,  5.0f, 
		-5.0f, -0.5f, -5.0f, 
		 5.0f, -0.5f, -5.0f
	};
	//地面的纹理坐标
	const float planeTexCoords[6 * 2] = {
		// (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). 
		//this will cause the floor texture to repeat)
		// texture Coords
		 2.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 2.0f,

		 2.0f, 0.0f,
		 0.0f, 2.0f,
		 2.0f, 2.0f
	};


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
	* 片段着色器 中 
	* 使用 gl_FragCoord.x/.y 根据片段在窗口中的位置来应用不同的颜色
	**/
	int practise(string projectDir);

	/**
	* 片段着色器 中
	* 使用 gl_FrontFacing 
	* 在 正方体内部和外部使用不同的纹理
	* 注意，如果你开启了面剔除，你就看不到箱子内部的面了，
	* 所以现在再使用gl_FrontFacing就没有意义了。
	*/
	int practise2(string projectDir);



	/**
	* gl_FragDepth的输出变量，我们可以使用它来在着色器内设置片段的深度值。
	* 自己设置深度值有一个很大的缺点，只要我们在片段着色器中对gl_FragDepth进行写入，
	* OpenGL就会（像深度测试小节中讨论的那样）禁用所有的提前深度测试(Early Depth Testing)。
	* 它被禁用的原因是，OpenGL无法在片段着色器运行之前得知片段将拥有的深度值，因为片段着色器可能会完全修改这个深度值。
	*
	* 从OpenGL 4.2起，我们仍可以对两者进行一定的调和，在片段着色器的顶部使用深度条件(Depth Condition)重新声明gl_FragDepth变量：
	*
	* layout (depth_<condition>) out float gl_FragDepth;
	* condition可以为下面的值：
	* ------------------------------------------------------------------------
	*	条件			| 描述
	* ------------------------------------------------------------------------
	*	any			| 默认值。提前深度测试是禁用的，你会损失很多性能
	*	greater		| 你只能让深度值比gl_FragCoord.z更大
	*	less		| 你只能让深度值比gl_FragCoord.z更小
	*	unchanged	| 如果你要写入gl_FragDepth，你将只能写入gl_FragCoord.z的值
	* ------------------------------------------------------------------------
	*
	* 通过将深度条件设置为greater或者less，OpenGL就能假设你只会写入比当前片段深度值更大或者更小的值了。
	* 注意这个特性只在OpenGL 4.2版本或以上才提供。
	*/
	int practise3(string projectDir);
};

