#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D texture_diffuse;

uniform bool horizontal;

uniform float weight[5] = float[](
	0.227027,
	0.1945946,
	0.1216216,
	0.054054,
	0.016216);

void main(){
	//gets size of single texture
	vec2 tex_offset = 1.0 / textureSize(texture_diffuse, 0);
	//current fragment's contribution
	vec3 result = texture(texture_diffuse, texCoords).rgb * weight[0];

	if(horizontal) {
		for(int i=1; i<5; i++) {
			result += texture(texture_diffuse, texCoords + vec2(tex_offset.x *i, 0.0)).rgb * weight[i];
			result += texture(texture_diffuse, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	} else {
		for (int i=1; i<5; i++) {
			result += texture(texture_diffuse, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(texture_diffuse, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0);
}