#version 330 core

out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main(){
	
	//�õ��۲���/������ķ�������I
	vec3 I = normalize(position - cameraPos);
	
	//�ɹ۲��ߺͷ����������㷴������
	vec3 R = reflect(I, normalize(normal));

	//ʹ�÷��������������������ͼ�в�����
	fragColor = vec4(texture(skybox, R).rgb, 1.0);
}