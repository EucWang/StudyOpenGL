#version 330 core

layout (points) in;

layout (triangle_strip, max_vertices=5) out;

// �Ӷ�����ɫ�������������ݵ�������ɫ��
// ���ǻ����������ʽ��ʾ������������������ֻ��һ�����㡣
in VS_INTERFACE{
	vec3 ourColor;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position){
	fColor = gs_in[0].ourColor; //// gs_in[0] ��Ϊֻ��һ�����붥��
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:����
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);  // 2:����
	//fColor = vec3(0.0, 1.0, 1.0);  //���Ʋ�ͬ�Ķ�����ɫ
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);  // 3:����
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);   // 4:����
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);   // 5:����
	fColor = vec3(1.0, 1.0, 1.0);  //���Ʋ�ͬ�Ķ�����ɫ
	EmitVertex();

	EndPrimitive();
}

void main(){
	
	build_house(gl_in[0].gl_Position);
}