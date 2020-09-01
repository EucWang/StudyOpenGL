#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNorm;
out vec2 ourTexCoord;
out vec3 fragPos;

void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	fragPos = vec3(model * vec4(aPos, 1.0));

	ourNorm = mat3(transpose(inverse(model))) * aNormal;

	ourTexCoord = aTexCoord;
}