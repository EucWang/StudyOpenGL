#version 330 core

in vec2 texCoords;

//输出的是float
out float fragColor;

uniform sampler2D texture_g_pos_depth;
uniform sampler2D texture_g_normal;
//uniform sampler2D texture_g_albedospec;
uniform sampler2D texture_g_noise;

const int SAMPLE_SIZE = 64;
uniform vec3 samples[SAMPLE_SIZE];
uniform mat4 projection;

//屏幕的平铺噪声纹理会根据屏幕分辨率除以噪声大小的值来决定
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

	//要将噪声纹理平铺到屏幕上， 通过将屏幕分辨率除以噪声纹理大小的方式计算texCoords的缩放大小
	//由于我们为texNoise设置的平铺参数设置为GL_REPEAT， 随机的值将会在全屏幕上不断重复
	vec3 randomVec = normalize(texture(texture_g_noise, texCoords * noiseScale).xyz);

	//加上fragPos，fragNorm， 可以将向量从切线空间变换到观察者空间
	vec3 tangent = normalize(randomVec - fragNorm * dot(randomVec, fragNorm));
	vec3 bitangent = cross(fragNorm, tangent);
	mat3 TBN = mat3(tangent, bitangent, fragNorm);

	float occlusion  = 0.0;
	for(int i = 0; i< kernelSize; i++){
		//获取样本位置
		vec3 sample = TBN * samples[i]; //切线-》 观察者空间
		sample = fragPos + sample * radius;

		//变换sample到屏幕空间
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset;      //观察 -》 裁剪空间
		offset.xyz /= offset.w;           //透视划分 // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5;    //变换到 0.0 - 1.0 的值域
		//使用offset向量的x和y分量采样线性深度纹理从而获取样本位置
		//从观察者视角的深度值（第一个不被遮蔽的可见片段）
		float sampleDepth = texture(texture_g_pos_depth, offset.xy).w;
		//occlusion += sampleDepth > sample.z ? 1.0 : 0.0;

		//引入一个范围测试，从而保证只有当被测深度值在取样半径内时才影响遮蔽因子
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