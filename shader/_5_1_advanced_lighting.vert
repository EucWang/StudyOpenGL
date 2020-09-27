#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT{
	vec2 texCoords;
	vec3 normal;
	vec3 fragPos;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.texCoords = aTexCoords;

	vs_out.fragPos = vec3( model * vec4(aPos, 1.0));
	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
}