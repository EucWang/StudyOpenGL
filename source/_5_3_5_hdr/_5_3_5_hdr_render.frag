#version 330 core

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

struct Light {
	vec3 lightPos;
	vec3 lightColor;
};

out vec4 fragColor;

uniform sampler2D texture_diffuse;

//uniform vec3 lightPos;
uniform Light lights[16];
uniform vec3 viewPos;

float shininess = 32.0;

void main(){
	vec3 tex1 = texture(texture_diffuse, vs_in.texCoords).rgb;

	vec3 norm = normalize(vs_in.fragNorm);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	
	vec3 ambient = 0.0 * tex1;

	vec3 lighting = vec3(0.0);
	for(int i=0; i<16; i++) {
		vec3 lightDir = normalize(lights[i].lightPos - vs_in.fragPos);
		//vec3 halfwayDir = normalize(lightDir + viewDir);

		float diff = max(dot(lightDir, norm), 0.0);
		vec3 diffuse = lights[i].lightColor * diff * tex1;
		vec3 result = diffuse;

		//float spec = pow(max(dot(halfwayDir, norm), 0.0), shininess);
		//vec3 specular = spec * lights[i].lightColor;
		//lighting += (diffuse + specular);

		float distance = length(vs_in.fragPos - lights[i].lightPos);
		result *= 1.0 / (distance * distance);
		lighting += result;
	}
	//fragColor = vec4(tex1, 1.0);
	fragColor = vec4(ambient + lighting, 1.0);
}