#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec2 fragTexCoord;
in vec3 fragNorm;
in vec3 fragPos;

out vec4 fragColor;

void main(){

	vec3 fragTex1 = vec3(texture(material.diffuse, fragTexCoord));
	vec3 fragTex2 = vec3(texture(material.specular, fragTexCoord));

	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0/ (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 ambient = light.ambient * fragTex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = light.diffuse * (diff * fragTex1);

	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
	float spec = pow(diff2, material.shininess);
	vec3 specular = light.specular * (spec * fragTex2);

	vec3 result = ambient * attenuation + diffuse * attenuation + specular * attenuation;
	fragColor = vec4(result, 1.0);
}
