#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture_gPosDepth;
uniform sampler2D texture_gNormal;
uniform sampler2D texture_gAlbedo;
uniform sampler2D texture_ssao;

struct Light {
	vec3 pos;
	vec3 color;

	float linear;
	float quadratic;
	float radius;
};

uniform Light light;

float shininess = 8.0;

void main(){
	
	//get data from gframebuffer
	vec3 fragPos = texture(texture_gPosDepth, texCoords).rgb;
	vec3 fragNorm = texture(texture_gNormal, texCoords).rgb;
	vec3 fragDiffuse = texture(texture_gAlbedo, texCoords).rgb;
	float ambient_occlusion = texture(texture_ssao, texCoords).r;


	// blinn-Phong  view 
	vec3 ambient = vec3(0.3 * ambient_occlusion);
	vec3 lighting = ambient;  //环境光照

	vec3 viewDir = normalize(-fragPos);   //Viewpos为(0.0.0) 在观察空间中

	//diffuse 漫反射 
	vec3 lightDir = normalize(light.pos - fragPos);
	float diff = max(dot(fragNorm, lightDir), 0.0);
	vec3 diffuse = diff * fragDiffuse * light.color;

	//specular 镜面
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(fragNorm, halfwayDir), 0.0), shininess);
	vec3 specular = light.color * spec;

	//attenuation 衰减
	float distance = length(light.pos -  fragPos);
	float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
	diffuse *= attenuation;
	specular *= attenuation;

	lighting += diffuse + specular;

	fragColor = vec4(lighting, 1.0);
}