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

vec3 getTriangleNormal(){
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

	return normalize(cross(a, b));
}

//TODO
//TODO
//TODO 

void main(){

	//vs_out[0].texCoords = vs_in[0].texCoords;
	//vs_out[0].fragPos = vs_in[0].fragPos;
	//vs_out[0].fragNormal = vs_in[0].fragNormal;
	
	gl_Position = gl_in[0].gl_Position;
	ourTexCoords = vs_in[0].texCoords;
	EmitVertex();
	EndPrimitive();
}