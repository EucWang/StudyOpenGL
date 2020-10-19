#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_out;

//�Ƿ�ת����
uniform bool inverse_normals;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.texCoords = aTexCoords;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));

	vec3 norm = inverse_normals ? -aNormal : aNormal;

	vs_out.fragNorm = mat3(transpose(inverse(model))) * norm;
}