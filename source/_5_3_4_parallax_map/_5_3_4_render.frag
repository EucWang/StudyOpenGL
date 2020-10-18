#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec2 texCoords;
	
	vec3 tangentFragPos;
	vec3 tangnetViewPos;
	vec3 tangnetLightPos;
}vs_in;

vec3 lightColor = vec3(0.7);
float shininess = 64.0;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

void main(){
	
	vec3 tex1 = texture(texture_diffuse, vs_in.texCoords).rgb;
	vec3 norm = texture(texture_normal, vs_in.texCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);

	vec3 lightDir = normalize(vs_in.tangnetLightPos - vs_in.tangentFragPos);
	vec3 viewDir = normalize(vs_in.tangnetViewPos - vs_in.tangentFragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir );

	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(norm, halfwayDir),0.0), shininess);
	vec3 specular = spec * lightColor;

	fragColor = vec4((ambient + diffuse + specular) * tex1, 1.0);

}