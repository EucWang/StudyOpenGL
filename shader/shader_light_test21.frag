#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 fragPos;

in vec3 fragNorm;   //物体的法向量

out vec4 fragColor;

float ambientStrength = 0.1;  //漫反射光强度
float specularStrength = 0.5;  //物体的镜面高光强度
int shininess = 32;    //物体高光反射度

void main(){

	//环境光照
	vec3 ambient = ambientStrength * lightColor;  

	//漫反射光照
	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(lightPos - fragPos);	
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = diff * lightColor;

	//物体镜面高光
	vec3 viewDir = normalize(viewPos - fragPos);  //视线向量
	vec3 reflectDir = reflect(-lightDir, norm);  //反射光向量
	float diff2 = max(dot(viewDir, reflectDir), 0.0);  //视线向量和反射向量的夹角
	float spec = pow(diff2, shininess);   //镜面分量参数
	vec3 specular = specularStrength * spec * lightColor;  

	//综合的结果
	vec3 result= (ambient + diffuse + specular ) * objectColor;
	fragColor = vec4(result, 1.0);
}