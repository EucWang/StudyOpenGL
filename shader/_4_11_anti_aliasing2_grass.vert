#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in mat4 aModel;

layout (std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

//uniform mat4 model;

out VS_OUT{
	vec2 texCoords;
} vs_out;

void main(){
	vs_out.texCoords = aTexCoords;
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = projection * view * aModel * vec4(aPos, 1.0);
}
