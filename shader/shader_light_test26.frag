#version 330 core

out vec4 fragColor;

in vec2 fragTexCoord;
in vec3 fragNorm;
in vec3 fragPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

//全局的定向光
struct DirLight {
	vec3 direciton;  //方向
	
	vec3 ambient;//光照3分量	
	vec3 diffuse;
	vec3 specular;
};

// 点光源
struct PointLight {
	vec3 ambient;//光照3分量	
	vec3 diffuse;
	vec3 specular;
	
	vec3 position;//光源位置

	float constant;//光衰减公式的系数
	float linear;
	float quadratic;
};

//聚光
struct SpotLight {
	vec3 ambient;  //光照3分量
	vec3 diffuse;
	vec3 specular;
	
	vec3 position;   //光源位置
	vec3 direction;  //光源方向

	float cutoff;       //光源的切光角度
	float outerCutoff;   //光源的外切光角度
};

struct Light {

	//光照3分量	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//光源位置
	vec3 position;

	//光衰减公式的系数
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

//计算定向光对物体产生颜色分量
vec3 calcDirLight(DirLight dlight, vec3 norm, vec3 viewDir);

//计算点光源对物体产生的颜色分量
vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir);

//计算聚光对物体产生的颜色分量
vec3 calcSpotLight(SpotLight slight, vec3 norm, vec3 fragPos, vec3 viewDir);

void main(){
	
	//2个箱子贴图
	vec3 fragTex1 = vec3(texture(material.diffuse, fragTexCoord));
	vec3 fragTex2 = vec3(texture(material.specular, fragTexCoord));

	//4个向量, 法线向量,到物体表面的光线向量, 到物体表面的视角向量,从物体表面反射出去的光线向量
	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);

	float distance = length(light.position - fragPos);  //光到物体表面的距离
	//光到物体表面的衰减量
	float attenuation = 1.0 / (light.constant + light.linear * distance
		+ light.quadratic * (distance * distance));	

 
	vec3 ambient = light.ambient * fragTex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = light.diffuse * (diff * fragTex1);

	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
	float spec = pow(diff2, material.shininess);
	vec3 specular = light.specular * (spec * fragTex2);

	vec3 result = ambient + diffuse + specular;
	result *= attenuation;
	fragColor = vec4(result, 1.0);
}