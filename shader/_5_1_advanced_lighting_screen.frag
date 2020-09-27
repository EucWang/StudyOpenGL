#version 330 core

out vec4 fragColor;

uniform sampler2D texture_screen;

in VS_OUT{
	vec2 texCoords;
} vs_in;

void main(){
	fragColor = vec4( texture(texture_screen, vs_in.texCoords).rgb, 1.0);
}