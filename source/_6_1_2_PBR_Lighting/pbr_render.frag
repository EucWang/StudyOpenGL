#version 330 core

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
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);


void main(){
	vec3 N = normalize(vs_in.fragNormal);
	vec3 V= normalize(viewPos - vs_in.worldPos);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	//reflectance equation
	vec3 Lo = vec3(0.0);
	for(int i = 0; i< LIGHT_COUNT; ++ i) {
		//calculate pre-light radiance
		vec3 L = normalize(lightPos[i] - vs_in.worldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPos[i] - vs_in.worldPos);
		float attenuation = 1.0 / (distance * distance);

		//cook-torrance brdf
		//float NDF = distributionGGX(N, H, roughness);
		//float G = geometrySmith(N, V, L, roughness);
		//vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0);
		//
		//vec3 kS = F;
		//vec3 kD = vec3(1.0) - kS;
		//kD *= 1.0 - metallic;
		//
		//
		//vec3 nominator = NDF * G * F;
		//float denominator = 4.0 * max(dot(N,V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		//vec3 specular = nominator / denominator;
		//
		////add to outgoing radiance Lo
		//float NdotL = max(dot(N, L), 0.0);
		//Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

}