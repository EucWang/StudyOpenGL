#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices4 {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_INTERFACE4 {
	vec2 texCoords;
} vs_i4;

void main(){
	vs_i4.texCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}