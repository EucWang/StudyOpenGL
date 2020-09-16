#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 fragNorm;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main(){
	fragColor = texture(texture_diffuse1, texCoords);
}