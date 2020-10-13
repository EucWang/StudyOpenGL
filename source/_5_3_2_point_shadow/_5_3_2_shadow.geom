#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadow_matrices[6];

out vec4 fragPos;  //fragPos from GS (output per emitvertex)

void main(){

	for(int face = 0; face < 6; ++ face) {
		//������ɫ����һ���ڽ���������gl_Layer����ָ����ɢ������ͼ���͵���������ͼ���ĸ��档
		//�������Ǹ�������������ܿ���ÿ������ͼ�ν���Ⱦ����������ͼ����һ���档
		//��Ȼ��ֻ�е���������һ�����ӵ������֡�������������ͼ�������Ч
		gl_Layer = face;
		for(int i = 0; i < 3; ++i) {
			//����ͨ������Ĺ�ռ�任�������FragPos����ÿ������ռ䶥��任����صĹ�ռ䣬����ÿ��������
			fragPos = gl_in[i].gl_Position;
			gl_Position = shadow_matrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}