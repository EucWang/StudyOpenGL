#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main (){

	vec4 tex = texture(texture_diffuse1, texCoords);
	if (tex.a < 0.4){
		discard;
	}
	fragColor = tex;
}