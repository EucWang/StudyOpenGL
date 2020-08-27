#version 330 core

struct Material {

	sampler2D diffuse;   //��������ͼ
	sampler2D specular;  //�߹ⷴ����ͼ

	float shininess;   //�߹ⷴ������� ���Ƹ߹�ȣ�ɢ���

};
uniform Material material;

struct Light {

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
};

uniform Light light;

uniform vec3 viewPos;

in vec3 fragNorm;
in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 fragColor;

void main(){

	vec3 ambient = light.ambient * vec3(texture(material.diffuse,fragTexCoord));  //�������߷���

	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, norm), 0.0);   //���������ͷ���������ˣ��õ��н�cos��ֵ
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, fragTexCoord)));  //��������߷���

	vec3 viewDir = normalize(viewPos - fragPos);  //�ӽ�����
	vec3 reflectDir = reflect(-lightDir, norm);

	diff = max(dot(reflectDir, viewDir), 1.0);
	float spec = pow(diff, material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, fragTexCoord)));

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result , 1.0);
}