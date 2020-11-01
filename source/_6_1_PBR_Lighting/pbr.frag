#version 330 core

in VS_OUT {
	vec2 texCoords;
	vec3 worldPos;
	vec3 fragNorm;
} vs_in;

out vec4 fragColor;

uniform sampler2D texture_diffuse;

uniform vec3 cameraPos;  // ���λ��
uniform vec3 albedo;     //albedo����, ������
uniform float metallic;   //������
uniform float roughtness;  //�ֲڶ�
uniform float ao;          // ao �������ڱ�

float PI = 3.1415926;

/// Trowbridge-Reitz GGX ��̬�ֲ�����
float D_GGX_TR(vec3 N, vec3 H, float a) {
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2= NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k){
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom /denom;
}

/// ���κ���
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);
	
	return ggx1 * ggx2;
}

///����������
///vec3 F0 = vec3(0.04);
///F0 = mix(F0, surfaceColor.rgb, metalness);
///���� cosTheta �Ǳ��淨����n��۲췽��v�ĵ�˵Ľ��
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){
	vec3 N = normalize(vs_in.fragNorm);
	vec3 V = normalize(cameraPos - vs_in.worldPos);



	vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	fragColor = vec4(tex, 1.0);
}