#version 330 core

out vec4 fragColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
in vec2 ourTexCoord;
in vec3 fragPos;
in vec3 fragNorm;

//全局的定向光
struct DirLight {
	vec3 direction;  //方向
	
	vec3 ambient;//光照3分量	
	vec3 diffuse;
	vec3 specular;
};
//
//// 点光源
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

	float constant;//光衰减公式的系数
	float linear;
	float quadratic;
};

uniform DirLight dirlight;
uniform PointLight pointlight;
uniform SpotLight spotlight;

uniform vec3 viewPos;
uniform float shininess;

//计算定向光对物体产生颜色分量
vec3 calcDirLight(DirLight dlight, vec3 norm, vec3 viewDir, vec3 tex1, vec3 tex2);

//计算点光源对物体产生的颜色分量
vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

//计算聚光对物体产生的颜色分量
vec3 calcSpotLight(SpotLight slight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

void main(){

	//2个箱子贴图
	vec3 fragTex1 = vec3(texture(texture_diffuse1, ourTexCoord));
	vec3 fragTex2 = vec3(texture(texture_specular1, ourTexCoord));

	//4个向量, 法线向量,到物体表面的光线向量, 到物体表面的视角向量,从物体表面反射出去的光线向量
	vec3 norm = normalize(fragNorm);
	vec3 viewDir = normalize(viewPos - fragPos);
	
	vec3 result = calcDirLight(dirlight, norm, viewDir, fragTex1, fragTex2);
	result += calcPointLight(pointlight, norm, fragPos, viewDir, fragTex1, fragTex2);
	result += calcSpotLight(spotlight, norm, fragPos, viewDir, fragTex1, fragTex2);
	//fragColor = vec4(result, 1.0);
	fragColor = texture(texture_diffuse1, ourTexCoord); 
}

//计算定向光对物体产生颜色分量
vec3 calcDirLight(DirLight dlight, vec3 norm, vec3 viewDir, vec3 tex1, vec3 tex2) {
	vec3 result;

	vec3 lightDir = normalize(dlight.direction);
	vec3 reflectLightDir = reflect(-lightDir, norm);
	
	vec3 ambient = dlight.ambient * tex1;
	
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = dlight.diffuse * (diff * tex1);

	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
	//float spec = pow(diff2, material.shininess);
	float spec = pow(diff2, shininess);
	//vec3 specular = dlight.specular * (spec * tex2);
	//这里屏蔽掉tex2的分量影响
	vec3 specular = dlight.specular * (spec * 1.0f);

	result = ambient + diffuse + specular;
	return result;
}

//计算点光源对物体产生的颜色分量
vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2) {
	vec3 result;

	vec3 lightDir = normalize(plight.position - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);


 	vec3 ambient = plight.ambient * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = plight.diffuse * (diff * tex1);
	
	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
	//float spec = pow(diff2, material.shininess);
	float spec = pow(diff2, shininess);
	//vec3 specular = plight.specular * (spec * tex2);
	//这里屏蔽掉tex2的分量影响
	vec3 specular = plight.specular * (spec * 1.0f);
	
	result = ambient + diffuse + specular;

	float distance = length(plight.position - fragPos);  //光到物体表面的距离
	//光到物体表面的衰减量
	float attenuation = 1.0 / (plight.constant + plight.linear * distance
		+ plight.quadratic * (distance * distance));	

	result *= attenuation;
	fragColor = vec4(result, 1.0);

	return result;
}

//计算聚光对物体产生的颜色分量
vec3 calcSpotLight(SpotLight slight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2) {
	vec3 result;
	
	vec3 lightDir = normalize(slight.position - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);
	
 	vec3 ambient = slight.ambient * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = slight.diffuse * (diff * tex1);
	
	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
	//float spec = pow(diff2, material.shininess);
	float spec = pow(diff2, shininess);
	//vec3 specular = slight.specular * (spec * tex2);
	//这里屏蔽掉tex2的分量影响
	vec3 specular = slight.specular * (spec * 1.0f);
	
	//聚光的切角
	float theta = dot(lightDir, normalize(-slight.direction));  //光线到物体表面的向量与聚光的方向之间的夹角
	float epsilon = slight.cutoff - slight.outerCutoff;
	float intense = (theta - slight.outerCutoff) / epsilon;
	float intensity = clamp(intense, 0.0, 1.0);

	float distance = length(slight.position - fragPos);  //光到物体表面的距离
	//光到物体表面的衰减量
	float attenuation = 1.0 / (slight.constant + slight.linear * distance
		+ slight.quadratic * (distance * distance));	

	
	result = ambient + diffuse + specular;
	result *= attenuation;
	result *= intensity;

	return result;
}
