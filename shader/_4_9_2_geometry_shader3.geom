#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices=6) out;  //������

in VS_OUT {
	//vec2 texCoords;
	vec3 normal;   // ����, ת�����ü��ռ�ķ�����
} vs_in[];

//out vec2 ourTexCoords;

const float MAGNITUDE = 0.4;

void generateLine(int index){

	//ourTexCoords = vs_in[index].texCoords;

	gl_Position = gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(vs_in[index].normal, 0.0) * MAGNITUDE;
	EmitVertex();

	EndPrimitive();

}

void main(){
	generateLine(0);  //��һ�����㷨��
	generateLine(1);  //�ڶ������㷨��
	generateLine(2);  //���������㷨��
}