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
uniform float ao;          // ao �������ڱ�

uniform float metallic;   //������
uniform float roughtness;  //�ֲڶ�

float PI = 3.14159265359;

vec3 Lo = vec3(0.0);
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];

/// Trowbridge-Reitz GGX ��̬�ֲ�����
///�����a �൱�� roughtness * roughtness 
float D_GGX_TR(vec3 N, vec3 H, float a) {
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2= NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return nom / denom;
}

/// ��̬�ֲ�����
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

/// ���κ���
float GeometrySmith(vec3 N, vec3 V, vec3 L, float fRoughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, fRoughness);
	float ggx2 = GeometrySchlickGGX(NdotL, fRoughness);
	
	return ggx1 * ggx2;
}

///����������
///���������̷��ص���һ�����������߱�����İٷֱ�
///vec3 F0 = vec3(0.04);
///F0 = mix(F0, surfaceColor.rgb, metalness);
///���� cosTheta �Ǳ��淨����n��۲췽��v�ĵ�˵Ľ��
/// ���� F0  ����Ϊ0������ǵķ���(surface reflection at zero incidence)��ʾ���ֱ��(��ֱ)�۲�����ʱ���ж��ٹ��߻ᱻ���䡣
/// �������F0����Ϊ���ϲ�ͬ����ͬ�����һ���Ϊ�����ǽ�����������ɫ
/// ��PBR�����������Ǽ򵥵���Ϊ������ľ�Ե����F0Ϊ0.04��ʱ�������Ӿ�������ȷ�ģ�
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

		//���ڷǽ���������˵F0��Զ����0.04���ֵ��
		//���ǻ���ݱ���Ľ��������ı�F0���ֵ�� ������ԭ����F0�ͷ������в�ֵ����F0��
		
		//cook-torrance brdf
		float NDF = DistributionGGX(N, H, roughtness);
		float G = GeometrySmith(N, V, L, roughtness);
		float HdotV = max(dot(H, V), 0.0);
		vec3 F = fresnelSchlick(HdotV, F0);

		//�������Ǿʹչ����㹻���������� Cook-Torrance BRDF
		vec3 nominator = NDF * G * F;
		//ע�������ڷ�ĸ���м���һ��0.001Ϊ�˱�����ֳ������
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator / denominator;

		vec3 kS = F;                 //����������ֱ�Ӹ�����kS
		vec3 kD = vec3(1.0) - kS;    // ��kS���Ǻ����׼�������ı�ֵkD
		kD *= (1.0 - metallic);        //��Ϊ��������������ߣ���˲����������䡣������������ǽ����ģ����ǻ��ϵ��kD��Ϊ0��

		float NdotL = max(dot(N, L), 0.0);
		//���յĽ��Lo������˵�ǳ�����ߵķ����ʣ�ʵ�����Ƿ����ʷ��̵��ڰ������򦸵Ļ��ֵĽ����
		//��������ʵ���ϲ���Ҫȥ�������Ϊ�������п��ܵ�������߷�������֪��ֻ��4�������������߻�Ӱ��Ƭ��(����)����ɫ
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	//��Ҫ����ɫ�������٤�������
	//��٤�����֮ǰ���ǲ���ɫ��ӳ��ʹLo��LDR��ֵӳ��ΪHDR��ֵ��
	//������õ�ɫ��ӳ�䷽��ΪReinhard ����
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	fragColor = vec4(color, 1.0);

	//vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	//fragColor = vec4(tex, 1.0);
}