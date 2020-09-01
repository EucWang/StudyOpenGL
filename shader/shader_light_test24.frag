#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
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

	//基础光照
	vec3 ambient = light.ambient * fragTex;	

	//漫反射
	float diff = max(dot(lightDir, norm), 0.0);      //光线和法线的夹角
	vec3 diffuse = light.diffuse * (diff * fragTex);

	//高光反射
	float diff2 = max(dot(reflectDir, viewDir), 0.0);    //反射光线和视角向量的夹角
	float spec = pow(diff2, material.shininess);				//高光分散参数和亮度
	vec3 specular = light.specular * (spec * fragMapTex);
	
	fragColor = vec4( ambient  + diffuse + specular, 1.0);
}