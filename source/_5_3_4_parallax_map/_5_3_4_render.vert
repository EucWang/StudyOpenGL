#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 viewPos;
uniform vec3 lightPos;

out VS_OUT {
	vec2 texCoords;
	vec3 tangentFragPos;
	vec3 tangnetViewPos;
	vec3 tangnetLightPos;
}vs_out;

void main(){
	vec3 fragPos = vec3(model * vec4(aPos, 1.0));
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 N = normalize(normalMatrix * aNormal);
	vec3 T = normalize(normalMatrix * aTangent);
	T = normalize(T - dot(T, N)*N);
	vec3 B = cross(T, N);

	mat3 TBN = transpose(mat3(T,B, N));

	vs_out.tangentFragPos = TBN * fragPos;
	vs_out.tangnetLightPos = TBN * lightPos;
	vs_out.tangnetViewPos = TBN * viewPos;
	
	vs_out.texCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}