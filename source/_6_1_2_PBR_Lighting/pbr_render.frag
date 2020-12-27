#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec2 texCoords;
	vec3 worldPos;
	vec3 fragNormal;
} vs_in;

//material parameters
uniform vec3 albedo;
uniform float ao;
uniform float roughness;
uniform float metallic;

//lights
const int LIGHT_COUNT = 4;
uniform vec3 lightPos[LIGHT_COUNT];
uniform vec3 lightColor[LIGHT_COUNT];

uniform vec3 viewPos;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
//vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);


void main(){
	vec3 N = normalize(vs_in.fragNormal);
	vec3 V= normalize(viewPos - vs_in.worldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	//reflectance equation
	vec3 Lo = vec3(0.0);
	for(int i = 0; i< LIGHT_COUNT; ++ i) {
		
		//calculate pre-light radiance
		vec3 L = normalize(lightPos[i] - vs_in.worldPos);
		vec3 H = normalize(V + L);
		float distance0 = length(lightPos[i] - vs_in.worldPos);
		float attenuation = 1.0 / (distance0 * distance0);
		vec3 radiance = lightColor[i] * attenuation;
		
		//cook-torrance BRDF
		 float NDF = distributionGGX(N, H, roughness);
		 float G = geometrySmith(N, V, L, roughness);
		 float cosTheta = clamp(dot(H, V), 0.0, 1.0);
		 vec3 F = fresnelSchlick(cosTheta, F0);

		 vec3 nominator = NDF * G * F;
		 float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		 // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		 vec3 specular = nominator / max(denominator, 0.001);


		 //kS is equal to Fresnel
		 vec3 kS = F;

		 //for energy conservation, the diffuse and specular light can't
		 // be above 1.0 (unless the surface emits light);
		 // to preserve this  relationship the diffuse component (kD) should 
		 // equal 1.0 - kS
		 vec3 kD = vec3(1.0) - kS;
		 // multiply kD by the inverse metalness such that only non-metals 
		 //have diffuse lighting, or a linear blend if partly metal (pure metals
		 // have no diffuse light)
		 kD *= 1.0 - metallic;

		//scale light by NdotL		 
		 float NdotL = max(dot(N, L), 0.0);
		 
		 //add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	// ambient lighting (note that the next IBL tutorial will replace 
	// this ambient lighting with environment lighting)
	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + Lo;

	//HDR tonemapping
	color = color / (color + vec3(1.0));
	//gamma correct
	color = pow(color, vec3(1.0/2.2));

	fragColor = vec4(color, 1.0);
}


/// 正态分布函数
float distributionGGX(vec3 N, vec3 H, float roughness) {
	float result = 0.0;

	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom  = a2;
	float denom  = (NdotH2 * (a2 - 1.0) + 1.0);

	denom = PI * denom * denom;

	//// prevent divide by zero for roughness=0.0 and NdotH=1.0
	result = nom / max(denom, 0.001);

	return result;
}


float geometrySchlickGGX(float NdotV, float roughness) {
	float result = 0.0;

	float r = (roughness + 1.0);
	float k  = (r * r) / 8.0;
	
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	result = nom / denom;

	return result;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float result = 0.0;

	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	result = ggx1 * ggx2;

	return result;
}


vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
