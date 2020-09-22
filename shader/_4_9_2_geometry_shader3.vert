#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices3 {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT {
//	vec2 texCoords;
	vec3 normal;   // 法线, 转换到裁剪空间的法向量
} vs_out;

void main(){
	//vs_out.texCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);

	//为了适配（观察和模型矩阵的）缩放和旋转，
	//在将法线变换到裁剪空间坐标之前， 先使用法线矩阵变换一次
	//因为几何着色器接受的位置向量就是裁剪空间坐标，
	//所以我们应该将法向量变换到相同的空间中
	//经过模型和视角矩阵 的法线变换矩阵
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));

	// 
	vs_out.normal = normalize(vec3(projection * 
								vec4( normalMatrix * aNormal,
								1.0)));
	
}