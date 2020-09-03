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

struct Light {

	//����3����	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//��Դλ��
	vec3 position;

	//��˥����ʽ��ϵ��
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;


void main(){
	
	//2��������ͼ
	vec3 fragTex1 = vec3(texture(material.diffuse, fragTexCoord));
	vec3 fragTex2 = vec3(texture(material.specular, fragTexCoord));

	//4������, ��������,���������Ĺ�������, �����������ӽ�����,��������淴���ȥ�Ĺ�������
	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectLightDir = reflect(-lightDir, norm);

	float distance = length(light.position - fragPos);  //�⵽�������ľ���
	//�⵽��������˥����
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