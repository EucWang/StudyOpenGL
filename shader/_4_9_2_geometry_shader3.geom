#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices=6) out;  //画线条

in VS_OUT {
	//vec2 texCoords;
	vec3 normal;   // 法线, 转换到裁剪空间的法向量
} vs_in[];

//out vec2 ourTexCoords;

const float MAGNITUDE = 0.4;

void generateLine(int index){

	//ourTexCoords = vs_in[index].texCoords;

	gl_Position = gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(vs_in[index].normal, 0.0) * MAGNITUDE;
	EmitVertex();

	EndPrimitive();

}

void main(){
	generateLine(0);  //第一个顶点法线
	generateLine(1);  //第二个顶点法线
	generateLine(2);  //第三个顶点法线
}