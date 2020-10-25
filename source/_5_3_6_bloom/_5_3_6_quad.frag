#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture_bloom;
uniform sampler2D texture_bloomblur;

uniform bool bloom;
uniform float exposure;
uniform bool use_gamma;

const float gamma = 2.2;

void main(){

	
	vec3 hdrtex = texture(texture_bloom, texCoords).rgb;
	vec3 bloomTex = texture(texture_bloomblur, texCoords).rgb;

	if(bloom) {
		hdrtex += bloomTex;  //结合两个图像
	}

	//tone mapping
	vec3 result = vec3(1.0) - exp(-hdrtex * exposure);
	//also gamma correct while we're at it
	if(use_gamma){
		result = pow(result, vec3(1.0/gamma));
	}
	fragColor = vec4(result, 1.0);
}