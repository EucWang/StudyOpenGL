#version 330 core

uniform sampler2D texture_diffuse1;

out vec4 fragColor;

in VS_OUT{
	vec2 texCoords;
} vs_in;

void main(){
	fragColor = texture(texture_diffuse1, vs_in.texCoords);
}