#version 330 core

struct Material {

	sampler2D diffuse;   //漫反射贴图
	sampler2D specular;  //高光反射贴图

	float shininess;   //高光反射参数， 控制高光度，散射度

};
uniform Material material;

struct Light {

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//vec3 position;
	vec3 direction; //平行光， 使用方向向量来替换position向量, 
};

uniform Light light;

uniform vec3 viewPos;

in vec3 fragNorm;
in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 fragColor;

void main(){

	vec3 ambient = light.ambient * vec3(texture(material.diffuse,fragTexCoord));  //环境光线分量

	vec3 norm = normalize(fragNorm);
	//vec3 lightDir = normalize(light.position - fragPos);

	//这里对light.direction取反，我们计算的时候需求一个从片段至光源的光线方向，而且需要标准化
	//而我们习惯定义的光为一个从光源出发的全局方向。
	vec3 lightDir = normalize(-light.direction);  

	float diff = max(dot(lightDir, norm), 0.0);   //光线向量和法线向量点乘，得到夹角cos比值
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, fragTexCoord)));  //漫反射光线分量

	vec3 viewDir = normalize(viewPos - fragPos);  //视角向量
	vec3 reflectDir = reflect(-lightDir, norm);

	diff = max(dot(reflectDir, viewDir), 0.0);
	float spec = pow(diff, material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, fragTexCoord)));

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result , 1.0);
}