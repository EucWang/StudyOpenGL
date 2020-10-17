#version 330 core

out vec4 fragColor;

in VS_OUT{
	vec2 texCoords;
	vec3 fragPos;
	//mat3 TBN;

	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentFragPos;

} vs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

vec3 lightColor = vec3(0.7);
float shininess = 32.0;

void main(){
	vec3 tex1 = texture(texture_diffuse, vs_in.texCoords).rgb;
	vec3 norm = texture(texture_normal, vs_in.texCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	//norm = normalize(vs_in.TBN * norm);
	
	//vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	//vec3 lightDir = normalize(lightPos - vs_in.fragPos);
	//vec3 viewDir = vs_in.TBN * normalize(viewPos - vs_in.fragPos);
	//vec3 lightDir = vs_in.TBN *  normalize(lightPos - vs_in.fragPos);
	vec3 viewDir = normalize(vs_in.tangentViewPos - vs_in.tangentFragPos);
	vec3 lightDir = normalize(vs_in.tangentLightPos - vs_in.tangentFragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
	vec3 specular = spec * lightColor;
	
	fragColor = vec4( (ambient + diffuse + specular) * tex1, 1.0);
}

