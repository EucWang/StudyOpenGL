#version 330 core


in VS2_INTERFACE{
	vec2 texCoords;
} vs2_in;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main(){
	fragColor = texture(texture_diffuse1, vs2_in.texCoords);
}

