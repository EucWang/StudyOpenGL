#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

/**
* 聚光
*/
struct Light {
	vec3 position;
	vec3 direction;  //聚光得方向
	float cutoff;     //聚光的切光角指定了聚光的半径 
	float outerCutoff;  // 聚光的外切光角

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec2 fragTexCoord;
in vec3 fragNorm;
in vec3 fragPos;

out vec4 fragColor;

void main(){

	vec3 fragTex1 = vec3(texture(material.diffuse, fragTexCoord));
	vec3 fragTex2 = vec3(texture(material.specular, fragTexCoord));

	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);

	//光的位置和箱子位置的距离
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/ (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 result;
	//箱子到光源的向量与光源射出光线向量的点乘，得到两向量的夹角的余弦值
	float theta = dot(lightDir, normalize(-light.direction));  
	float epsilon = light.cutoff - light.outerCutoff;
	float intense = (theta - light.outerCutoff) / epsilon;
	//使用了clamp函数，它把第一个参数约束(Clamp)在了0.0到1.0之间。这保证强度值不会在[0, 1]区间之外。
	float intensity = clamp(intense, 0.0, 1.0);


	//if(theta > light.cutoff) { //比较的余弦值大，则表示在聚光的范围内，执行聚光计算
		vec3 ambient = light.ambient * fragTex1;
		float diff = max(dot(lightDir, norm), 0.0);
		vec3 diffuse = light.diffuse * (diff * fragTex1);

		float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
		float spec = pow(diff2, material.shininess);
		vec3 specular = light.specular * (spec * fragTex2);

		result = ambient * attenuation +
			diffuse * attenuation * intensity + 
			specular * attenuation * intensity;		
	//} else {   //否则，使用环境光，不至于太黑暗
		//result = light.ambient * fragTex1;
	//}
	
	fragColor = vec4(result, 1.0);
}
