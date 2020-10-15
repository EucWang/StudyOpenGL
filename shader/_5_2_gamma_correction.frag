#version 330 core

#define POINT_SIZE 4

//ȫ�ֵĶ����
struct DirLight {
	vec3 direction;  //����
	
	vec3 ambient;//����3����	
	vec3 diffuse;
	vec3 specular;
};

// ���Դ
struct PointLight {
	vec3 ambient;//����3����	
	vec3 diffuse;
	vec3 specular;
	
	vec3 position;//��Դλ��

	float constant;//��˥����ʽ��ϵ��
	float linear;
	float quadratic;
};

//�۹�
struct SpotLight {
	vec3 ambient;  //����3����
	vec3 diffuse;
	vec3 specular;
	
	vec3 position;   //��Դλ��
	vec3 direction;  //��Դ����

	float cutoff;       //��Դ���й�Ƕ�
	float outerCutoff;   //��Դ�����й�Ƕ�

	float constant;//��˥����ʽ��ϵ��
	float linear;
	float quadratic;
};

//���㶨�������������ɫ����
//vec3 calcDirLight(DirLight dlight, vec3 norm, vec3 viewDir, vec3 tex1, vec3 tex2);

//������Դ�������������ɫ����
//vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

//����۹�������������ɫ����
//vec3 calcSpotLight(SpotLight slight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2);

///������Դ�������������ɫ����  �� ����Blinn-Phong��ɫģ��
vec3 calcPointLightBlinnPhong(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir);

out vec4 fragColor;

in VS_OUT{
	vec2 texCoords;
	vec3 fragNorm;
	vec3 fragPos;
} vs_in;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

//uniform DirLight dirlight;
uniform PointLight pointlights[POINT_SIZE];
//uniform SpotLight spotlight;

uniform vec3 viewPos;

float shininess = 64.0;
//uniform bool useSpot;
//uniform bool usePoint;
//uniform bool useBlinn;

void main(){
	vec3 tex1 = texture(texture_diffuse1, vs_in.texCoords).rgb;
	//vec3 tex2 = texture(texture_specular1, vs_in.texCoords).rgb;
	
	vec3 norm = normalize(vs_in.fragNorm);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);
	
	vec3 result = vec3(0.0);
	//result = calcDirLight(dirlight, norm, viewDir, tex1, tex2);
	
	//if(usePoint) {
		for(int i =0; i<POINT_SIZE;i++) {
			//if(!useBlinn){
			//	result += calcPointLight(pointlights[i], norm, vs_in.fragPos, viewDir, tex1, tex2);
			//}else {
				result += calcPointLightBlinnPhong(pointlights[i], norm, vs_in.fragPos, viewDir);
			//}
		}
	//}
	//if(useSpot)
	//	result += calcSpotLight(spotlight, norm, vs_in.fragPos, viewDir, tex1, tex2);
	
	tex1 *= result;

	tex1 = pow(tex1, vec3(1.0/2.2));

	fragColor =vec4(tex1, 1.0);
}


///������Դ�������������ɫ����  �� ����Blinn-Phong��ɫģ��
vec3 calcPointLightBlinnPhong(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir){
	vec3 result = vec3(0.0);

	vec3 lightDir = normalize(plight.position - fragPos);

	//vec3 ambient = plight.ambient * tex1; 
	vec3 ambient = plight.ambient; 

	float diff = max(dot(lightDir, norm), 0.0);
	//vec3 diffuse = plight.diffuse * (diff * tex1);
	vec3 diffuse = (diff) * plight.diffuse;

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow( max(dot(norm, halfwayDir), 0.0), shininess);
	//vec3 specular = plight.specular * (spec * tex2);
	vec3 specular = spec * plight.specular;
	
	
	float distance = length(plight.position - fragPos);  //�⵽�������ľ���
	//�⵽��������˥����
	float attenuation = 1.0 / (plight.constant + plight.linear * distance
		+ plight.quadratic * (distance * distance));	

	//result = ambient + diffuse + specular;
	result = ambient * attenuation + diffuse * attenuation + specular * attenuation;

	return result;
}

//������Դ�������������ɫ����
//vec3 calcPointLight(PointLight plight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2) {
//	vec3 result;
//
//	vec3 lightDir = normalize(plight.position - fragPos);
//	vec3 reflectLightDir = reflect(-lightDir, norm);
//
//
// 	vec3 ambient = plight.ambient * tex1;
//
//	float diff = max(dot(lightDir, norm), 0.0);
//	vec3 diffuse = plight.diffuse * (diff * tex1);
//	
//	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
//	//float spec = pow(diff2, material.shininess);
//	float spec = pow(diff2, shininess);
//	vec3 specular = plight.specular * (spec * tex2);
//	
//	result = ambient + diffuse + specular;
//
//	float distance = length(plight.position - fragPos);  //�⵽�������ľ���
//	//�⵽��������˥����
//	float attenuation = 1.0 / (plight.constant + plight.linear * distance
//		+ plight.quadratic * (distance * distance));	
//
//	result *= attenuation;
//	return result;
//}

//����۹�������������ɫ����
//vec3 calcSpotLight(SpotLight slight, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 tex1, vec3 tex2) {
//	vec3 result;
//	
//	vec3 lightDir = normalize(slight.position - fragPos);
//	vec3 reflectLightDir = reflect(-lightDir, norm);
//	
// 	vec3 ambient = slight.ambient * tex1;
//
//	float diff = max(dot(lightDir, norm), 0.0);
//	vec3 diffuse = slight.diffuse * (diff * tex1);
//	
//	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
//	//float spec = pow(diff2, material.shininess);
//	float spec = pow(diff2, shininess);
//	vec3 specular = slight.specular * (spec * tex2);
//	
//	//�۹���н�
//	float theta = dot(lightDir, normalize(-slight.direction));  //���ߵ���������������۹�ķ���֮��ļн�
//	float epsilon = slight.cutoff - slight.outerCutoff;
//	float intense = (theta - slight.outerCutoff) / epsilon;
//	float intensity = clamp(intense, 0.0, 1.0);
//
//	float distance = length(slight.position - fragPos);  //�⵽�������ľ���
//	//�⵽��������˥����
//	float attenuation = 1.0 / (slight.constant + slight.linear * distance
//		+ slight.quadratic * (distance * distance));	
//	
//	result = ambient + diffuse + specular;
//	result *= attenuation;
//	result *= intensity;
//
//	return result;
//}
//


//���㶨�������������ɫ����
//vec3 calcDirLight(DirLight dlight, vec3 norm, vec3 viewDir, vec3 tex1, vec3 tex2) {
//	vec3 result;
//
//	vec3 lightDir = normalize(dlight.direction);
//	vec3 reflectLightDir = reflect(-lightDir, norm);
//	
//	vec3 ambient = dlight.ambient * tex1;
//	
//	float diff = max(dot(lightDir, norm), 0.0);
//	vec3 diffuse = dlight.diffuse * (diff * tex1);
//
//	float diff2 = max(dot(reflectLightDir, viewDir), 0.0);
//	//float spec = pow(diff2, material.shininess);
//	float spec = pow(diff2, shininess);
//	vec3 specular = dlight.specular * (spec * tex2);
//
//	result = ambient + diffuse + specular;
//	return result;
//}