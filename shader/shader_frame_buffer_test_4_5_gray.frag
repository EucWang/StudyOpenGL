#version 330 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main(){

	fragColor = texture(screenTexture, texCoords);
	//人眼对绿色更加敏感,对蓝色不怎么敏感,
	//这里使用加权的通道,达到更加真实的灰度效果
	float average = (0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b);

	fragColor = vec4(average, average, average, 1.0);

}