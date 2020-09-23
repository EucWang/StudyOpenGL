#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// �����Ƕ������Ե����ʹ���vec4ʱ����Ҫ�����һ�������ˡ�
// �������������������ݴ�С����һ��vec4��
// ��Ϊһ��mat4��������4��vec4��������ҪΪ�������Ԥ��4���������ԡ�
// ��Ϊ���ǽ�����λ��ֵ����Ϊ3������ÿһ�еĶ�������λ��ֵ����3��4��5��6��
layout (location = 3) in mat4 instanceModels;

out VS_OUT{
	vec2 texCoords;
} vs_out;

//uniform mat4 model;

layout (std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

void main(){
	gl_Position  = projection * view * instanceModels * vec4(aPos, 1.0);
	vs_out.texCoords = aTexCoords;
}