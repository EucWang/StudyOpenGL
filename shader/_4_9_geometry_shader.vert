#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

//uniform mat4 model;
//layout (std140) uniform Matrices {
//	mat4 view;
//	mat4 projection;
//};

out VS_INTERFACE{
	vec3 ourColor;
} vs_out;

void main(){
	//gl_PointSize = gl_Position.z;
	//gl_Position  = projection * view * model * vec4(aPos, 0.0, 1.0);
	gl_Position  = vec4(aPos.x, aPos.y, 0.0, 1.0);

	vs_out.ourColor = aColor;
}