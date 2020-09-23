#version 330 core


in VS_OUT {
	vec2 texCoords;
} vs_in;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main(){
	fragColor = texture(texture_diffuse1, vs_in.texCoords);
}
