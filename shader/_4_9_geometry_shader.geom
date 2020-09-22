#version 330 core

/**
* 需要声明从顶点着色器输入的图元类型。
* 在in关键字前声明一个布局修饰符(Layout Qualifier)。
* 这个输入布局修饰符可以从顶点着色器接收下列任何一个图元值：
*---------------------------------------------------------------------------- 
* 取值					意思
*--------------------------------------------------------------------------
* points				绘制GL_POINTS图元时（1）。
* lines					绘制GL_LINES或GL_LINE_STRIP时（2）
* lines_adjacency		GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY（4）
* triangles				GL_TRIANGLES、GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN（3）
* triangles_adjacency	GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY（6）
*--------------------------------------------------------- -------------------
* 	
* 以上是能提供给glDrawArrays渲染函数的几乎所有图元了。
* 如果我们想要将顶点绘制为GL_TRIANGLES，我们就要将输入修饰符设置为triangles。
* 括号内的数字表示的是一个图元所包含的最小顶点数。
* 
* ^^^^^^^^^^^^^^^^ 
* 指定几何着色器输出的图元类型，需要在out关键字前面加一个布局修饰符。
* 输出布局修饰符也可以接受几个图元值：
*	- points
*	- line_strip
*	- triangle_strip
* 
* 有了这3个输出修饰符，我们就可以使用输入图元创建几乎任意的形状了。
* 要生成一个三角形的话，我们将输出定义为triangle_strip，并输出3个顶点。
* 
* OpenGL中，三角形带(Triangle Strip)是绘制三角形更高效的方式，它使用顶点更少。
* 在第一个三角形绘制完之后，每个后续顶点将会在上一个三角形边上生成另一个三角形：每3个临近的顶点将会形成一个三角形。
* 如果我们一共有6个构成三角形带的顶点，
* 那么我们会得到这些三角形：(1, 2, 3)、(2, 3, 4)、(3, 4, 5)和(4, 5, 6)，共形成4个三角形。
*/
layout (points) in;


//layout (points, max_vertices=1) out;
layout (line_strip, max_vertices=2) out;

void main(){

	//原样输出原有的点
	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//EndPrimitive();

	gl_Position = gl_in[0].gl_Position + vec4(-0.2f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(0.2f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	
	EndPrimitive();
}