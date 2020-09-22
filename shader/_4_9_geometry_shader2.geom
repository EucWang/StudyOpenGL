#version 330 core

layout (points) in;

layout (triangle_strip, max_vertices=5) out;

// 从顶点着色器发来输入数据到几何着色器
// 总是会以数组的形式表示出来，即便我们现在只有一个顶点。
in VS_INTERFACE{
	vec3 ourColor;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position){
	fColor = gs_in[0].ourColor; //// gs_in[0] 因为只有一个输入顶点
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:左下
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);  // 2:右下
	//fColor = vec3(0.0, 1.0, 1.0);  //绘制不同的顶点颜色
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);  // 3:左上
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);   // 4:右上
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);   // 5:顶部
	fColor = vec3(1.0, 1.0, 1.0);  //绘制不同的顶点颜色
	EmitVertex();

	EndPrimitive();
}

void main(){
	
	build_house(gl_in[0].gl_Position);
}