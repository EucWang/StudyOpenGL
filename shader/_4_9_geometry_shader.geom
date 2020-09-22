#version 330 core

/**
* ��Ҫ�����Ӷ�����ɫ�������ͼԪ���͡�
* ��in�ؼ���ǰ����һ���������η�(Layout Qualifier)��
* ������벼�����η����ԴӶ�����ɫ�����������κ�һ��ͼԪֵ��
*---------------------------------------------------------------------------- 
* ȡֵ					��˼
*--------------------------------------------------------------------------
* points				����GL_POINTSͼԪʱ��1����
* lines					����GL_LINES��GL_LINE_STRIPʱ��2��
* lines_adjacency		GL_LINES_ADJACENCY��GL_LINE_STRIP_ADJACENCY��4��
* triangles				GL_TRIANGLES��GL_TRIANGLE_STRIP��GL_TRIANGLE_FAN��3��
* triangles_adjacency	GL_TRIANGLES_ADJACENCY��GL_TRIANGLE_STRIP_ADJACENCY��6��
*--------------------------------------------------------- -------------------
* 	
* ���������ṩ��glDrawArrays��Ⱦ�����ļ�������ͼԪ�ˡ�
* ���������Ҫ���������ΪGL_TRIANGLES�����Ǿ�Ҫ���������η�����Ϊtriangles��
* �����ڵ����ֱ�ʾ����һ��ͼԪ����������С��������
* 
* ^^^^^^^^^^^^^^^^ 
* ָ��������ɫ�������ͼԪ���ͣ���Ҫ��out�ؼ���ǰ���һ���������η���
* ����������η�Ҳ���Խ��ܼ���ͼԪֵ��
*	- points
*	- line_strip
*	- triangle_strip
* 
* ������3��������η������ǾͿ���ʹ������ͼԪ���������������״�ˡ�
* Ҫ����һ�������εĻ������ǽ��������Ϊtriangle_strip�������3�����㡣
* 
* OpenGL�У������δ�(Triangle Strip)�ǻ��������θ���Ч�ķ�ʽ����ʹ�ö�����١�
* �ڵ�һ�������λ�����֮��ÿ���������㽫������һ�������α���������һ�������Σ�ÿ3���ٽ��Ķ��㽫���γ�һ�������Ρ�
* �������һ����6�����������δ��Ķ��㣬
* ��ô���ǻ�õ���Щ�����Σ�(1, 2, 3)��(2, 3, 4)��(3, 4, 5)��(4, 5, 6)�����γ�4�������Ρ�
*/
layout (points) in;


//layout (points, max_vertices=1) out;
layout (line_strip, max_vertices=2) out;

void main(){

	//ԭ�����ԭ�еĵ�
	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//EndPrimitive();

	gl_Position = gl_in[0].gl_Position + vec4(-0.2f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(0.2f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	
	EndPrimitive();
}