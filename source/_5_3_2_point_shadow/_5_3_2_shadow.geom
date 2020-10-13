#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadow_matrices[6];

out vec4 fragPos;  //fragPos from GS (output per emitvertex)

void main(){

	for(int face = 0; face < 6; ++ face) {
		//几何着色器有一个内建变量叫做gl_Layer，它指定发散出基本图形送到立方体贴图的哪个面。
		//但当我们更新这个变量就能控制每个基本图形将渲染到立方体贴图的哪一个面。
		//当然这只有当我们有了一个附加到激活的帧缓冲的立方体贴图纹理才有效
		gl_Layer = face;
		for(int i = 0; i < 3; ++i) {
			//我们通过把面的光空间变换矩阵乘以FragPos，将每个世界空间顶点变换到相关的光空间，生成每个三角形
			fragPos = gl_in[i].gl_Position;
			gl_Position = shadow_matrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}