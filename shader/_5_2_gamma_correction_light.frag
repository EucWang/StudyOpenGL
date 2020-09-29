#version 330 core

//in VS_OUT{
//	vec2 texCoords;
//} vs_in;

out vec4 fragColor;

uniform vec3 lightColor;

float gamma = 2.2;

void main(){

	fragColor = vec4(lightColor, 1.0);
}
