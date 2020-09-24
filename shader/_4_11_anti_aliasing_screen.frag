#version 330 core

out vec4 fragColor;

uniform sampler2D texture_screen;

in vec2 texCoords;

void main(){
	fragColor = vec4(texture(texture_screen, texCoords).rgb, 1.0);
}