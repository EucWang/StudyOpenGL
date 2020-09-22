#version 330 core

//in VS_INTERFACE {
//	vec2 texCoords;
//	vec3 fragPos;
//	vec3 fragNormal;
//} vs_in;

in vec2 ourTexCoords;

out vec4 fragColor;

uniform sampler2D texture_diffuser1;

void main(){
	//fragColor = texture(texture_diffuser1, vs_in.texCoords);
	fragColor = texture(texture_diffuser1, ourTexCoords);
}