#version 330 core

layout (location =0) in vec3 aPos;
layout (location =1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

//�Ƿ�ת����
uniform bool invertedNormals;

out VS_OUT {
	vec2 texCoords;
	vec3 fragPos;  //�����fragPos ���ڹ۲��߿ռ��
	vec3 fragNorm;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	vs_out.texCoords = aTexCoords;

	vs_out.fragPos = vec3(view * model * vec4(aPos, 1.0));
	vs_out.fragNorm = mat3(transpose(inverse(model))) *  (invertedNormals ? - aNormal : aNormal);
}