#version 330 core

out vec4 fragColor;

in VS_OUT{

	vec3 fragPos;
	vec3 fragNorm;
	vec2 texCoords;
} vs_in;

// 点光源
//struct PointLight {
//	vec3 ambient;//光照3分量	
//	vec3 diffuse;
//	vec3 specular;
//	
//	vec3 position;//光源位置
//
//	float constant;//光衰减公式的系数
//	float linear;
//	float quadratic;
//};


uniform sampler2D floorTexture;
uniform sampler2D floorTextureSpecular;

uniform vec3 lightPosition[4];
uniform vec3 lightColors[4];

//uniform PointLight lights[4];

uniform vec3 viewPos;
uniform bool gamma;

uniform vec3 attenuation_args;

vec3 blinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor) {
	vec3 result;

	//diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	//vec3 reflectDir = reflect(-lightDir, normal);

	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec  =pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	//simple attenuation   简化的光线衰减方程
	float distance = length(lightPos - fragPos);
	float quadratic = attenuation_args.x + attenuation_args.y * distance 
		+ attenuation_args.z * (distance * distance);
	float attenuation = 1.0 / (gamma? quadratic : distance);

	diffuse *= attenuation;
	specular *= attenuation;

	result = diffuse + specular;
	return result;
}

void main(){

	vec3 color = texture(floorTexture, vs_in.texCoords).rgb;
	vec3 colorSpecular = texture(floorTextureSpecular, vs_in.texCoords).rgb;
	vec3 lighting = vec3(0.0);

	vec3 norm = normalize(vs_in.fragNorm);

	for(int i=0 ; i<4; i++){
		lighting += blinnPhong(norm, vs_in.fragPos, lightPosition[i], lightColors[i]);
	}
	color *= lighting;
	if(gamma){
		color = pow(color, vec3(1.0/2.2));
	}
	fragColor = vec4(color, 1.0);
}