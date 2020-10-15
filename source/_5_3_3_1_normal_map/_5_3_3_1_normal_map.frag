#version 330 core

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

uniform sampler2D normal_map;

in VS_OUT {
	vec3 fragPos;
	//vec3 fragNorm;
	vec2 texCoords;

} vs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 lightColor = vec3(0.7f);

float shininess = 64.0;

void main(){

	vec3 tex1 = texture(texture_diffuse1, vs_in.texCoords).rgb;
	vec3 norm = texture(normal_map, vs_in.texCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);

	vec3 lightDir = normalize(lightPos - vs_in.fragPos);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(halfwayDir, norm), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	fragColor = vec4((ambient + diffuse + specular)*tex1, 1.0);
}