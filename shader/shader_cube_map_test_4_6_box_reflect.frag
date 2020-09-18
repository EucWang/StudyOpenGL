#version 330 core

out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main(){
	
	//得到观察者/摄像机的方向向量I
	vec3 I = normalize(position - cameraPos);
	
	//由观察者和法线向量计算反射向量
	vec3 R = reflect(I, normalize(normal));

	//使用发射向量从天空立方体贴图中采样。
	fragColor = vec4(texture(skybox, R).rgb, 1.0);
}