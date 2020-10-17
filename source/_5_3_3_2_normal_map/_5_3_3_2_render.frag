#version 330 core

out vec4 fragColor;

in VS_OUT {
 vec2 texCoords;
 vec3 fragPos;
}vs_in;

uniform sampler2D texture_diffuse;

uniform sampler2D texture_normal;  //法线贴图 

uniform vec3 viewPos;

uniform vec3 lightPos;

vec3 lightColor = vec3(0.7);
float shininess = 64.0;

void main(){
	vec3 tex1 = texture(texture_diffuse, vs_in.texCoords).rgb; //纹理贴图
	vec3 norm = texture(texture_normal, vs_in.texCoords).rgb;//从法线贴图中获取法线
	norm = normalize(norm * 2.0 - 1.0); //将法线向量转换为范围[-1, 1]
	
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	vec3 lightDir = normalize(lightPos - vs_in.fragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(halfwayDir, norm), 0.0), shininess);
	vec3 specular = spec * lightColor;
	
	fragColor = vec4( (ambient + diffuse + specular) * tex1, 1.0);
}