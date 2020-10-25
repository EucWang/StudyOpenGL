#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D texture_diffuse;

uniform vec3 viewPos;

struct Light {
	vec3 pos;
	vec3 color;
};

uniform Light lights[4];

//uniform vec3 lightPos;
//uniform vec3 lightColor;

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

void main(){

	vec3 tex1 = texture(texture_diffuse, vs_in.texCoords).rgb;

	vec3 norm = normalize(vs_in.fragNorm);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	
	vec3 ambient = 0.3 * tex1;
	vec3 lighting = vec3(0.0);
	for(int i=0; i<4; i++) {
		vec3 lightDir = normalize(lights[i].pos - vs_in.fragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);

		float diff = max(dot(lightDir, norm), 0.0);
		//vec3 diffuse = tex1 * (diff * light[i].color);
		vec3 diffuse =  lights[i].color * (diff * tex1);

		float spec = pow(max(dot(norm, halfwayDir),0.0), 32.0);
		vec3 specular =  lights[i].color* (spec * tex1);

		float distance = length(vs_in.fragPos - lights[i].pos);
		diffuse *= 1.0 / (distance * distance);
		specular *= 1.0 / (distance * distance);

		lighting += diffuse;
		lighting += specular;
	}

	vec3 result = ambient + lighting;
	
    // check whether result is higher than some threshold, if so, output as bloom threshold color
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));

	if (brightness > 1.0) {
		BrightColor = vec4(result, 1.0);
	} else {
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}

	FragColor = vec4(result, 1.0);
}