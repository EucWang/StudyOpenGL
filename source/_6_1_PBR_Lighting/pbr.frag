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
uniform float ao;          // ao 环境光遮蔽

uniform float metallic;   //金属度
uniform float roughtness;  //粗糙度

float PI = 3.14159265359;

vec3 Lo = vec3(0.0);
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];

/// Trowbridge-Reitz GGX 正态分布函数
///这里的a 相当于 roughtness * roughtness 
float D_GGX_TR(vec3 N, vec3 H, float a) {
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2= NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return nom / denom;
}

/// 正态分布函数
float DistributionGGX(vec3 N, vec3 H, float fRoughtness) {
	float a = fRoughtness * fRoughtness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return nom/denom;
}

float GeometrySchlickGGX(float NdotV, float fRoughness){
	float r = (fRoughness + 1.0);
	float k  = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom /denom;
}

/// 几何函数
float GeometrySmith(vec3 N, vec3 V, vec3 L, float fRoughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, fRoughness);
	float ggx2 = GeometrySchlickGGX(NdotL, fRoughness);
	
	return ggx1 * ggx2;
}

///菲涅尔方程
///菲涅尔方程返回的是一个物体表面光线被反射的百分比
///vec3 F0 = vec3(0.04);
///F0 = mix(F0, surfaceColor.rgb, metalness);
///参数 cosTheta 是表面法向量n与观察方向v的点乘的结果
/// 参数 F0  被称为0°入射角的反射(surface reflection at zero incidence)表示如果直接(垂直)观察表面的时候有多少光线会被反射。
/// 这个参数F0会因为材料不同而不同，而且会因为材质是金属而发生变色
/// 在PBR金属流中我们简单地认为大多数的绝缘体在F0为0.04的时候看起来视觉上是正确的，
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){
	vec3 N = normalize(vs_in.fragNorm);
	vec3 V = normalize(cameraPos - vs_in.worldPos);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	
	for(int i=0; i<4;i++){
		//calculate pre-light radiance
		vec3 L = normalize(lightPos[i] - vs_in.worldPos);
		vec3 H = normalize(V + L);

		float distance = length(lightPos[i] - vs_in.worldPos);
		float attenuation  = 1.0 / (distance * distance);
		vec3 radiance = lightColor[i] * attenuation;

		//对于非金属材质来说F0永远保持0.04这个值，
		//我们会根据表面的金属性来改变F0这个值， 并且在原来的F0和反射率中插值计算F0。
		
		//cook-torrance brdf
		float NDF = DistributionGGX(N, H, roughtness);
		float G = GeometrySmith(N, V, L, roughtness);
		float HdotV = max(dot(H, V), 0.0);
		vec3 F = fresnelSchlick(HdotV, F0);

		//这样我们就凑够了足够的项来计算 Cook-Torrance BRDF
		vec3 nominator = NDF * G * F;
		//注意我们在分母项中加了一个0.001为了避免出现除零错误。
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator / denominator;

		vec3 kS = F;                 //菲涅尔方程直接给出了kS
		vec3 kD = vec3(1.0) - kS;    // 从kS我们很容易计算折射的比值kD
		kD *= (1.0 - metallic);        //因为金属不会折射光线，因此不会有漫反射。所以如果表面是金属的，我们会把系数kD变为0。

		float NdotL = max(dot(N, L), 0.0);
		//最终的结果Lo，或者说是出射光线的辐射率，实际上是反射率方程的在半球领域Ω的积分的结果。
		//但是我们实际上不需要去求积，因为对于所有可能的入射光线方向我们知道只有4个方向的入射光线会影响片段(像素)的着色
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	//需要在着色器最后做伽马矫正。
	//在伽马矫正之前我们采用色调映射使Lo从LDR的值映射为HDR的值。
	//这里采用的色调映射方法为Reinhard 操作
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	fragColor = vec4(color, 1.0);

	//vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	//fragColor = vec4(tex, 1.0);
}