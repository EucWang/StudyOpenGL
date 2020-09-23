#version 330 core


in VS_INTERFACE4 {
	vec2 texCoords;
} vs_o4;

uniform sampler2D texture_diffuse1;

out vec4 fragColor;

void main(){
	fragColor = texture(texture_diffuse1, vs_o4.texCoords);
}