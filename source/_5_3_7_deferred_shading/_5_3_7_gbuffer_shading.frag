#version 330 core
out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
	vec3 pos;
	vec3 color;
	
	float linear;
	float quadratic;
	float radius;
} ;

const int NR_LIGHTS = 100;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

float shininess = 16.0;

void main() {
	
	vec3 fragPos = texture(gPosition, texCoords).rgb;
	vec3 fragNorm = texture(gNormal, texCoords).rgb;
	vec3 fragDiffuse = texture(gAlbedoSpec, texCoords).rgb;
	float fragSpecular = texture(gAlbedoSpec, texCoords).a;

	vec3 lighting = fragDiffuse * 0.1;// hard-coded ambient component
	vec3 viewDir = normalize(viewPos - fragPos);

	for(int i=0; i< NR_LIGHTS; i++){

		float distance = length(lights[i].pos - fragPos);
		//如果距离小于光体积半径，则计算复杂光照
		if(distance < lights[i].radius) {
			vec3 lightDir = normalize(lights[i].pos - fragPos);

			//diffuse
			float diff = max(dot(fragNorm, lightDir), 0.0);
			vec3 diffuse = diff * fragDiffuse * lights[i].color;

			//specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(fragNorm, halfwayDir), 0.0), shininess);
			vec3 specular = lights[i].color * spec * fragSpecular;
	
			//attenuation
			float attenuation = 1.0 / (1.0 + 
				lights[i].linear * distance + lights[i].quadratic * distance * distance);
		
			diffuse *= attenuation;
			specular *= attenuation;
			lighting += (diffuse + specular);
		}
	}

	fragColor = vec4(lighting, 1.0);
}