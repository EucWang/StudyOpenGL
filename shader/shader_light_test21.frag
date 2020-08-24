#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 fragPos;

in vec3 fragNorm;   //����ķ�����

out vec4 fragColor;

float ambientStrength = 0.1;  //�������ǿ��
float specularStrength = 0.5;  //����ľ���߹�ǿ��
int shininess = 32;    //����߹ⷴ���

void main(){

	//��������
	vec3 ambient = ambientStrength * lightColor;  

	//���������
	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(lightPos - fragPos);	
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = diff * lightColor;

	//���徵��߹�
	vec3 viewDir = normalize(viewPos - fragPos);  //��������
	vec3 reflectDir = reflect(-lightDir, norm);  //���������
	float diff2 = max(dot(viewDir, reflectDir), 0.0);  //���������ͷ��������ļн�
	float spec = pow(diff2, shininess);   //�����������
	vec3 specular = specularStrength * spec * lightColor;  

	//�ۺϵĽ��
	vec3 result= (ambient + diffuse + specular ) * objectColor;
	fragColor = vec4(result, 1.0);
}