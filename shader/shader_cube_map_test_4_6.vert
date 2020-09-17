#version 330 core

/**
* �������崦��ԭ��(0, 0, 0)ʱ������ÿһ��λ���������Ǵ�ԭ������ķ���������
* ��������������ǻ�ȡ���������ض�λ�õ�����ֵ����Ҫ�ġ�������Ϊ���������ֻ��Ҫ�ṩλ���������������������ˡ�
*/
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;


/**
* ���ǽ������λ��������Ϊ�����Ƭ����ɫ�����������ꡣ
* Ƭ����ɫ���Ὣ����Ϊ����������samplerCube
*/
out vec3 textureDir;

void main(){
	gl_Position = projection * view * vec4(aPos, 1.0);
	textureDir = aPos;
}