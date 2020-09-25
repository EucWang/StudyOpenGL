#version 330 core

out vec4 fragColor;

uniform sampler2D textureScreen;

in vec2 texCoords;

void main(){
	fragColor = vec4(texture(textureScreen, texCoords).rgb, 1.0);
}