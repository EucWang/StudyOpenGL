#version 330 core

//out vec4 fragColor;

in vec4 fragPos;  //从几何着色器中得到的位置向量

uniform vec3 lightPos;
uniform float far_plane;

void main(){
	float lightDistance = length(fragPos.xyz - lightPos);
	lightDistance = lightDistance / far_plane;
	//这里我们把fragment和光源之间的距离，映射到0到1的范围，把它写入为fragment的深度值。
	gl_FragDepth = lightDistance;
}