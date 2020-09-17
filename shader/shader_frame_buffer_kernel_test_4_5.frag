#version 330 core

uniform sampler2D screenTexture;

in vec2 texCoords;

out vec4 fragColor;

void main(){

	fragColor = vec4(texture(screenTexture, texCoords).rgb, 1.0);
}
