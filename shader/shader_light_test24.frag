#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 ambient;       //光源的环境光分量         一般可取 vec3(0.2)
	vec3 diffuse;      //光源对照射物体的漫反射分量  一般可取 vec3(0.5) , vec3(1.0)
	vec3 specular;     //光源的对物体的高光反射分量 一般取vec3(1.0)

	vec3 position;     //光源所在位置

	float constant;  //计算衰减公式的常数项
	float linear;    //计算衰减公式的一次项
	float quadratic; //计算衰减公式的二次项
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

in vec3 ourNorm;
in vec2 ourTexCoord;
in vec3 fragPos;

out vec4 fragColor;

void main(){
	vec3 fragTex =  vec3(texture(material.diffuse, ourTexCoord));      //纹理贴图
	vec3 fragMapTex =  vec3(texture(material.specular, ourTexCoord));  //光照贴图

	vec3 norm = normalize(ourNorm);  //法线分量
	vec3 lightDir = normalize(light.position - fragPos);  //光线分量  
	
	vec3 viewDir = normalize(viewPos - fragPos);  //视角分量 
	vec3 reflectDir =  reflect(-lightDir, norm);        //反射光线分量

	float distance = length(light.position - fragPos);  //计算光源到物体之间的距离, 用于计算光照衰减
	float attenuation = 1.0 / (light.constant + light.linear * distance
		+ light.quadratic * (distance * distance));   //计算得到衰减分量

	//基础光照
	vec3 ambient = light.ambient * fragTex;	
	ambient *= attenuation;   //环境光照的衰减

	//漫反射
	float diff = max(dot(lightDir, norm), 0.0);      //光线和法线的夹角
	vec3 diffuse = light.diffuse * (diff * fragTex);
	diffuse *= attenuation;     //漫反射的衰减

	//高光反射
	float diff2 = max(dot(reflectDir, viewDir), 0.0);    //反射光线和视角向量的夹角
	float spec = pow(diff2, material.shininess);				//高光分散参数和亮度
	vec3 specular = light.specular * (spec * fragMapTex);
	specular *= attenuation;   //高光反射的衰减
	
	fragColor = vec4( ambient  + diffuse + specular, 1.0);
}