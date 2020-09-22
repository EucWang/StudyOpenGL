#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;


in VS_INTERFACE {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNormal;
} vs_in[];

//out VS_INTERFACE {
//	vec2 texCoords;
//	vec3 fragPos;
//	vec3 fragNormal;
//} vs_out[];

out vec2 ourTexCoords;

uniform float time;

/**
* ��������εķ�����
*/
vec3 getTriangleNormal(){
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

	return normalize(cross(a, b));
}

/**
* ����ʱ��ı仯���������Ļ����ϣ� ��̬�ı�����������
*/
vec4 explode(vec4 position, vec3 normal) {
	
	float magnitude = 2.0;

	float scope = (sin(time) + 1.0) / 2.0; //�仯��Χ�ڡ�0�� 1��֮��
	vec3 direction = normal * scope * magnitude;

	return position + vec4(direction, 0.0);
}

void main(){

	//vs_out[0].texCoords = vs_in[0].texCoords;
	//vs_out[0].fragPos = vs_in[0].fragPos;
	//vs_out[0].fragNormal = vs_in[0].fragNormal;

	vec3 normal = getTriangleNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	ourTexCoords = vs_in[0].texCoords;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal);
	ourTexCoords = vs_in[1].texCoords;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal);
	ourTexCoords = vs_in[2].texCoords;
	EmitVertex();
	
	EndPrimitive();
}