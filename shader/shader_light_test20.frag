#version 330 core

in vec3 FragPos;
in vec3 ourNormal;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 fragColor;

float ambientStrenght = 0.1; //漫反射光照强度

float specularStrenght = 0.5;// 高光量反射强度


void main(){

	//背景光分量
	vec3 ambient = ambientStrenght * lightColor;

	//漫反射光分量
	vec3 norm = normalize(ourNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff  = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//镜面高光分量
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrenght * spec * lightColor;

	vec3 result = ( ambient  + diffuse + specular ) * objectColor;	
	fragColor = vec4(result, 1.0);
}