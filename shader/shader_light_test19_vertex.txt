#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec3 FragPos;

void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	//通过把顶点位置属性乘以模型矩阵（不是观察和投影矩阵）来把它变换到世界空间坐标
	FragPos = vec3(model * vec4(aPos, 1.0));

	//ourNormal = aNormal;
	//法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」
	//大部分的资源都会将法线矩阵定义为应用到模型-观察矩阵(Model-view Matrix)上的操作，
	//但是由于我们只在世界空间中进行操作（不是在观察空间），我们只使用模型矩阵。
	ourNormal = mat3(transpose(inverse(model))) * aNormal;
}