#version 330 core

in vec2 texCoords;

//�������float
out float fragColor;

uniform sampler2D texture_g_pos_depth;
uniform sampler2D texture_g_normal;
//uniform sampler2D texture_g_albedospec;
uniform sampler2D texture_g_noise;

const int SAMPLE_SIZE = 64;
uniform vec3 samples[SAMPLE_SIZE];
uniform mat4 projection;

//��Ļ��ƽ����������������Ļ�ֱ��ʳ���������С��ֵ������
const vec2 noiseScale = vec2(800/4.0, 600/4.0);

//struct Light {
//	vec3 pos;
//	vec3 color;
//	
//	float linear;
//	float quadratic;
//	//float radius;
//} ;

//const int NR_LIGHTS = 1;
//uniform Light lights[NR_LIGHTS];
//uniform vec3 viewPos;

//float shininess = 16.0;

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

void main(){
	//fragColor = vec4(texture(texture_diffuse, texCoords).rgb, 1.0);

	vec3 fragPos = texture(texture_g_pos_depth, texCoords).rgb;
	//float depth = texture(texture_g_pos_depth, texCoords).a;
	vec3 fragNorm = normalize(texture(texture_g_normal, texCoords).rgb);

	//Ҫ����������ƽ�̵���Ļ�ϣ� ͨ������Ļ�ֱ��ʳ������������С�ķ�ʽ����texCoords�����Ŵ�С
	//��������ΪtexNoise���õ�ƽ�̲�������ΪGL_REPEAT�� �����ֵ������ȫ��Ļ�ϲ����ظ�
	vec3 randomVec = normalize(texture(texture_g_noise, texCoords * noiseScale).xyz);

	//����fragPos��fragNorm�� ���Խ����������߿ռ�任���۲��߿ռ�
	vec3 tangent = normalize(randomVec - fragNorm * dot(randomVec, fragNorm));
	vec3 bitangent = cross(fragNorm, tangent);
	mat3 TBN = mat3(tangent, bitangent, fragNorm);

	float occlusion  = 0.0;
	for(int i = 0; i< kernelSize; i++){
		//��ȡ����λ��
		vec3 sample = TBN * samples[i]; //����-�� �۲��߿ռ�
		sample = fragPos + sample * radius;

		//�任sample����Ļ�ռ�
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset;      //�۲� -�� �ü��ռ�
		offset.xyz /= offset.w;           //͸�ӻ��� // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5;    //�任�� 0.0 - 1.0 ��ֵ��
		//ʹ��offset������x��y�������������������Ӷ���ȡ����λ��
		//�ӹ۲����ӽǵ����ֵ����һ�������ڱεĿɼ�Ƭ�Σ�
		float sampleDepth = texture(texture_g_pos_depth, offset.xy).w;
		//occlusion += sampleDepth > sample.z ? 1.0 : 0.0;

		//����һ����Χ���ԣ��Ӷ���ֻ֤�е��������ֵ��ȡ���뾶��ʱ��Ӱ���ڱ�����
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion/kernelSize);
	fragColor = occlusion;

	//vec3 fragDiffuse = texture(texture_g_albedospec, texCoords).rgb;
	//float fragSpecular = texture(texture_g_albedospec, texCoords).a;

	//vec3 lighting = fragDiffuse * 0.1;// hard-coded ambient component
	//vec3 viewDir = normalize(viewPos - fragPos);

	//for(int i=0; i< NR_LIGHTS; i++){
	//	vec3 lightDir = normalize(lights[i].pos - fragPos);
	//	//diffuse
	//	float diff = max(dot(fragNorm, lightDir), 0.0);
	//	vec3 diffuse = diff * fragDiffuse * lights[i].color;
	//	//specular
	//	vec3 halfwayDir = normalize(lightDir + viewDir);
	//	float spec = pow(max(dot(fragNorm, halfwayDir), 0.0), shininess);
	//	vec3 specular = lights[i].color * spec * fragSpecular;
	//	//attenuation
	//	float distance = length(lights[i].pos - fragPos);
	//	float attenuation = 1.0 / (1.0 + 
	//		lights[i].linear * distance + lights[i].quadratic * distance * distance);
	//	specular *= attenuation;
	//	lighting += (diffuse + specular);
	//}

	//fragColor = vec4(lighting, 1.0);
}