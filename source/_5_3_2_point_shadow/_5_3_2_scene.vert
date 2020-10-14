#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 fragPos;
	vec3 fragNorm;
	vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//控制，在箱子内部，光照依然可以发挥作用
//是否反转法线
uniform bool reverse_normals;

void main(){

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));

	if(reverse_normals) {
		vs_out.fragNorm = mat3(transpose(inverse(model))) * (-1.0 * aNormal);
	} else {
		vs_out.fragNorm = mat3(transpose(inverse(model))) * aNormal;
	}
	vs_out.texCoords = aTexCoords;
}