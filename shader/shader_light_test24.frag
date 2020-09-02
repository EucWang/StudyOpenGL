#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 ambient;       //��Դ�Ļ��������         һ���ȡ vec3(0.2)
	vec3 diffuse;      //��Դ��������������������  һ���ȡ vec3(0.5) , vec3(1.0)
	vec3 specular;     //��Դ�Ķ�����ĸ߹ⷴ����� һ��ȡvec3(1.0)

	vec3 position;     //��Դ����λ��

	float constant;  //����˥����ʽ�ĳ�����
	float linear;    //����˥����ʽ��һ����
	float quadratic; //����˥����ʽ�Ķ�����
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

in vec3 ourNorm;
in vec2 ourTexCoord;
in vec3 fragPos;

out vec4 fragColor;

void main(){
	vec3 fragTex =  vec3(texture(material.diffuse, ourTexCoord));      //������ͼ
	vec3 fragMapTex =  vec3(texture(material.specular, ourTexCoord));  //������ͼ

	vec3 norm = normalize(ourNorm);  //���߷���
	vec3 lightDir = normalize(light.position - fragPos);  //���߷���  
	
	vec3 viewDir = normalize(viewPos - fragPos);  //�ӽǷ��� 
	vec3 reflectDir =  reflect(-lightDir, norm);        //������߷���

	float distance = length(light.position - fragPos);  //�����Դ������֮��ľ���, ���ڼ������˥��
	float attenuation = 1.0 / (light.constant + light.linear * distance
		+ light.quadratic * (distance * distance));   //����õ�˥������

	//��������
	vec3 ambient = light.ambient * fragTex;	
	ambient *= attenuation;   //�������յ�˥��

	//������
	float diff = max(dot(lightDir, norm), 0.0);      //���ߺͷ��ߵļн�
	vec3 diffuse = light.diffuse * (diff * fragTex);
	diffuse *= attenuation;     //�������˥��

	//�߹ⷴ��
	float diff2 = max(dot(reflectDir, viewDir), 0.0);    //������ߺ��ӽ������ļн�
	float spec = pow(diff2, material.shininess);				//�߹��ɢ����������
	vec3 specular = light.specular * (spec * fragMapTex);
	specular *= attenuation;   //�߹ⷴ���˥��
	
	fragColor = vec4( ambient  + diffuse + specular, 1.0);
}