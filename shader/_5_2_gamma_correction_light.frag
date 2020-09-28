#version 330 core

//in VS_OUT{
//	vec2 texCoords;
//} vs_in;

out vec4 fragColor;

uniform vec3 lightColor;

void main(){
	fragColor = vec4(lightColor, 1.0);
}
