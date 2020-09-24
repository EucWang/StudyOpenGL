#version 330 core

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

in VS_OUT{
	vec2 texCoords;
} vs_in;

void main(){
	fragColor = texture(texture_diffuse1, vs_in.texCoords);
}