#version 330 core


//物体的材质
struct Material {
 //ambient材质向量定义了在环境光照下这个物体反射得是什么颜色，
 //通常这是和物体颜色相同的颜色。
	vec3 ambient; 
//diffuse材质向量定义了在漫反射光照下物体的颜色。
//（和环境光照一样）漫反射颜色也要设置为我们需要的物体颜色。
	vec3 diffuse;
//specular材质向量设置的是镜面光照对物体的颜色影响
//（或者甚至可能反射一个物体特定的镜面高光颜色）。
	vec3 specular;
//shininess影响镜面高光的散射/半径。
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

in vec3 fragNorm;   //物体的法向量

out vec4 fragColor;

//float ambientStrength = 0.1;  //漫反射光强度
//float specularStrength = 0.5;  //物体的镜面高光强度
//int shininess = 32;    //物体高光反射度

void main(){

	//环境光照
	vec3 ambient = light.ambient * material.ambient;  

	//漫反射光照
	vec3 norm = normalize(fragNorm);
	vec3 lightDir = normalize(light.position - fragPos);	
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//物体镜面高光
	vec3 viewDir = normalize(viewPos - fragPos);  //视线向量
	vec3 reflectDir = reflect(-lightDir, norm);  //反射光向量
	float diff2 = max(dot(viewDir, reflectDir), 0.0);  //视线向量和反射向量的夹角
	float spec = pow(diff2, material.shininess);   //镜面分量参数
	vec3 specular = light.specular * (spec * material.specular);  

	//综合的结果
	vec3 result= ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}