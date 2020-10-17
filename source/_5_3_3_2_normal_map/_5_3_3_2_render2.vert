#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT{
	vec2 texCoords;
	vec3 fragPos;
	//mat3 TBN;

	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentFragPos;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.texCoords = aTexCoords;

	mat3 normMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normMatrix * aTangent);
	vec3 N = normalize(normMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);

	//vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
	//vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	//vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	// vec3 B = cross(T, B);
	//vs_out.TBN = mat3(T, B, N);

	//vs_out.TBN = transpose(mat3(T,B,N));
	mat3 TBN = transpose(mat3(T,B,N));
	vs_out.tangentFragPos = TBN * vs_out.fragPos;
	vs_out.tangentLightPos = TBN * lightPos;
	vs_out.tangentViewPos = TBN * viewPos;
}