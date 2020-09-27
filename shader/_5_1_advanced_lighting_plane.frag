#version 330 core
out vec4 fragColor;

in VS_OUT{
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

uniform sampler2D texture_diffuse1;

//全局的定向光
struct DirLight {
	vec3 direction;  //方向
	
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

	float constant;//光衰减公式的系数
	float linear;
	float quadratic;
};

//计算定向光对物体产生颜色分量
vec3 calcDirLight(DirLight dlight, vec3 norm, vec3 viewDir, vec3 tex1, vec3 tex2);

//计算点光源对物体产生的颜色分量
vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

//计算聚光对物体产生的颜色分量
vec3 calcSpotLight(SpotLight slight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

///计算点光源对物体产生的颜色分量  ， 采用Blinn-Phong着色模型
vec3 calcPointLightBlinnPhong(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

uniform DirLight dirlight;
uniform PointLight pointlight;
uniform SpotLight spotlight;

uniform vec3 viewPos;

float shininess = 32.0;

uniform bool blinn;
uniform bool spot;

void main(){
	vec3 tex1= texture(texture_diffuse1, vs_in.texCoords).rgb;
	vec3 tex2 = vec3(0.2);

	vec3 norm = normalize(vs_in.fragNorm);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);

	vec3 result;
	result = calcDirLight(dirlight, norm, viewDir, tex1, tex2);

	if(!blinn){
		tex2 = vec3(0.7);
		result += calcPointLight(pointlight, norm, vs_in.fragPos, viewDir, tex1, tex2);
	} else {
		tex2 = vec3(0.05);
		result += calcPointLightBlinnPhong(pointlight, norm, vs_in.fragPos, viewDir, tex1, tex2);
	}
	if(spot){
		result += calcSpotLight(spotlight, norm, vs_in.fragPos, viewDir, tex1, tex2);
	}
	fragColor = vec4(result, 1.0);
}

//计算定向光对物体产生颜色分量 ， 冯氏模型
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
	vec3 specular = dlight.specular * (spec * tex2);

	//result = ambient + diffuse + specular;
	result = ambient + diffuse;
	return result;
}

///计算点光源对物体产生的颜色分量  ， 采用Blinn-Phong着色模型
vec3 calcPointLightBlinnPhong(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2){
	vec3 result;

	vec3 lightDir = normalize(plight.position - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 ambient = plight.ambient * tex1; 

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = plight.diffuse * (diff * tex1);

	float spec = pow( max(dot(norm, halfwayDir), 0.0), shininess);
	vec3 specular = plight.specular * (spec + tex2);

	result = ambient + diffuse + specular;
	
	float distance = length(plight.position - fragPos);  //光到物体表面的距离
	//光到物体表面的衰减量
	float attenuation = 1.0 / (plight.constant + plight.linear * distance
		+ plight.quadratic * (distance * distance));	

	result *= attenuation;

	return result;
}

//计算点光源对物体产生的颜色分量  ， 冯氏模型
vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2) {
	vec3 result;

	vec3 lightDir = normalize(plight.position - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);

 	vec3 ambient = plight.ambient * tex1;  //环境光分量

	float diff = max(dot(lightDir, norm), 0.0); 
	vec3 diffuse = plight.diffuse * (diff * tex1);   // 漫反射分量
	
	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
	//float spec = pow(diff2, material.shininess);
	float spec = pow(diff2, shininess);
	vec3 specular = plight.specular * (spec * tex2);  //物体高光反射分量
	
	result = ambient + diffuse + specular;
	//result = ambient + diffuse;

	float distance = length(plight.position - fragPos);  //光到物体表面的距离
	//光到物体表面的衰减量
	float attenuation = 1.0 / (plight.constant + plight.linear * distance
		+ plight.quadratic * (distance * distance));	

	result *= attenuation;
	return result;
}

//计算聚光对物体产生的颜色分量 ， 冯氏模型
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
	vec3 specular = slight.specular * (spec * tex2);
	
	//聚光的切角
	float theta = dot(lightDir, normalize(-slight.direction));  //光线到物体表面的向量与聚光的方向之间的夹角
	float epsilon = slight.cutoff - slight.outerCutoff;
	float intense = (theta - slight.outerCutoff) / epsilon;
	float intensity = clamp(intense, 0.0, 1.0);

	float distance = length(slight.position - fragPos);  //光到物体表面的距离
	//光到物体表面的衰减量
	float attenuation = 1.0 / (slight.constant + slight.linear * distance
		+ slight.quadratic * (distance * distance));	

	
	//result = ambient + diffuse + specular;
	result = ambient + diffuse;
	result *= attenuation;
	result *= intensity;

	return result;
}
