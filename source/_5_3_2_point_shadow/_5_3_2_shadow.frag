#version 330 core

//out vec4 fragColor;

in vec4 fragPos;  //�Ӽ�����ɫ���еõ���λ������

uniform vec3 lightPos;
uniform float far_plane;

void main(){
	float lightDistance = length(fragPos.xyz - lightPos);
	lightDistance = lightDistance / far_plane;
	//�������ǰ�fragment�͹�Դ֮��ľ��룬ӳ�䵽0��1�ķ�Χ������д��Ϊfragment�����ֵ��
	gl_FragDepth = lightDistance;
}