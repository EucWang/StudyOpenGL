#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};
uniform Light light;

uniform vec3 viewPos;

in vec3 fragPos;
in vec3 fragNorm;

out vec4 fragColor;

void main(){

	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float diff2  = max(dot(reflectDir, viewDir), 0.0);
	float spec = pow(diff2, material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}