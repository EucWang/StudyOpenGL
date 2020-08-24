#version 330 core


//����Ĳ���
struct Material {
 //ambient���������������ڻ���������������巴�����ʲô��ɫ��
 //ͨ�����Ǻ�������ɫ��ͬ����ɫ��
	vec3 ambient; 
//diffuse����������������������������������ɫ��
//���ͻ�������һ������������ɫҲҪ����Ϊ������Ҫ��������ɫ��
	vec3 diffuse;
//specular�����������õ��Ǿ�����ն��������ɫӰ��
//�������������ܷ���һ�������ض��ľ���߹���ɫ����
	vec3 specular;
//shininessӰ�쾵��߹��ɢ��/�뾶��
	float shininess;
};
uniform Material material;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;

uniform vec3 viewPos;

in vec3 fragPos;

in vec3 fragNorm;   //����ķ�����

out vec4 fragColor;

//float ambientStrength = 0.1;  //�������ǿ��
//float specularStrength = 0.5;  //����ľ���߹�ǿ��
//int shininess = 32;    //����߹ⷴ���

void main(){

	//��������
	vec3 ambient = light.ambient * material.ambient;  

	//���������
	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);	
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//���徵��߹�
	vec3 viewDir = normalize(viewPos - fragPos);  //��������
	vec3 reflectDir = reflect(-lightDir, norm);  //���������
	float diff2 = max(dot(viewDir, reflectDir), 0.0);  //���������ͷ��������ļн�
	float spec = pow(diff2, material.shininess);   //�����������
	vec3 specular = light.specular * (spec * material.specular);  

	//�ۺϵĽ��
	vec3 result= ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}