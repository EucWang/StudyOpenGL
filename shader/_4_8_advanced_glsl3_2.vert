#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

layout (std140) uniform Matrices2 {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_INTERFACE{
	vec2 texCoords;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.texCoords = aTexCoords;
}