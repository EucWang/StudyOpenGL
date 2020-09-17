#version 330 core

/**
* 当立方体处于原点(0, 0, 0)时，它的每一个位置向量都是从原点出发的方向向量。
* 这个方向向量正是获取立方体上特定位置的纹理值所需要的。正是因为这个，我们只需要提供位置向量而不用纹理坐标了。
*/
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;


/**
* 我们将输入的位置向量作为输出给片段着色器的纹理坐标。
* 片段着色器会将它作为输入来采样samplerCube
*/
out vec3 textureDir;

void main(){

	vec4 pos = projection * view * vec4(aPos, 1.0);
	//这里让z分量一直等于w分量，在执行透视除法时， z分量会变成 w/w = 1.0. 
	//这样最终的标准设备坐标将永远有一个等于1.0的z值， 既最大的深度值。
	//也就是保证了 天空盒子的深度永远是最大的，
	gl_Position = pos.xyww;


	textureDir = aPos;
}