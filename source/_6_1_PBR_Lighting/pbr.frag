#version 330 core

in VS_OUT {
	vec2 texCoords;
	vec3 worldPos;
	vec3 fragNorm;
} vs_in;

out vec4 fragColor;

uniform sampler2D texture_diffuse;

uniform vec3 cameraPos;  // 相机位置
uniform vec3 albedo;     //albedo参数, 反照率
uniform float metallic;   //金属度
uniform float roughtness;  //粗糙度
uniform float ao;          // ao 环境光遮蔽

float PI = 3.1415926;

/// Trowbridge-Reitz GGX 正态分布函数
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

/// 几何函数
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);
	
	return ggx1 * ggx2;
}

///菲涅尔方程
///vec3 F0 = vec3(0.04);
///F0 = mix(F0, surfaceColor.rgb, metalness);
///参数 cosTheta 是表面法向量n与观察方向v的点乘的结果
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){
	vec3 N = normalize(vs_in.fragNorm);
	vec3 V = normalize(cameraPos - vs_in.worldPos);



	vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	fragColor = vec4(tex, 1.0);
}