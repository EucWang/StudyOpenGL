#version 330 core

out vec4 fragColor;

uniform sampler2D texture_diffuse;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

void main(){

	vec3 tex1 = texture(texture_diffuse, vs_in.texCoords).rgb;

	vec3 norm = normalize(vs_in.fragNorm);
	vec3 lightDir = normalize(lightPos - vs_in.fragPos);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = tex1 * (diff * lightColor);

	float spec = pow(max(dot(norm, halfwayDir),0.0), 64.0);
	vec3 specular = tex1 * (spec * lightColor);
	
	fragColor = vec4(ambient + diffuse + specular, 1.0);
}