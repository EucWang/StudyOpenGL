#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices3 {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT {
//	vec2 texCoords;
	vec3 normal;   // ����, ת�����ü��ռ�ķ�����
} vs_out;

void main(){
	//vs_out.texCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);

	//Ϊ�����䣨�۲��ģ�;���ģ����ź���ת��
	//�ڽ����߱任���ü��ռ�����֮ǰ�� ��ʹ�÷��߾���任һ��
	//��Ϊ������ɫ�����ܵ�λ���������ǲü��ռ����꣬
	//��������Ӧ�ý��������任����ͬ�Ŀռ���
	//����ģ�ͺ��ӽǾ��� �ķ��߱任����
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));

	// 
	vs_out.normal = normalize(vec3(projection * 
								vec4( normalMatrix * aNormal,
								1.0)));
	
}