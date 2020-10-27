#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

uniform sampler2D texture_diffuse;

void main(){
	vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	fragColor = vec4(tex, 1.0);
}