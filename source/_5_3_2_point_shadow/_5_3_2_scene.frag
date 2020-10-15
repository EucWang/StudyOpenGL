#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec3 fragPos;
	vec3 fragNorm;
	vec2 texCoords;
} vs_in;

uniform sampler2D diffuse_texture;
uniform samplerCube depth_map;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

//�Ƿ�Ӧ����Ӱ����
uniform bool hasShadow;

vec3 lightColor = vec3(0.3);

float calcShadow(vec3 fragPos) {
	//�����Ѿ�����ȴ���Ϊfragment�͹�λ��֮��ľ����ˣ���������������ƵĴ���ʽ��
	//�õ���fragment��λ������λ��֮��Ĳ�ͬ������
	vec3 fragToLight = fragPos - lightPos;

	float closestDepth = texture(depth_map, fragToLight).r;
	closestDepth *= far_plane;
	float curDepth = length(fragToLight);

	float bias = 0.05;
	float shadow = curDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

float calcShadow2(vec3 fragPos){

	vec3 fragToLight = fragPos - lightPos;
	float curDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.05;
	float samples = 4.0;
	float offset = 0.1;
	//�������Ǹ���������������̬����������ƫ��������������������������Σ���������������ƽ������
	for(float x = -offset; x < offset; x += offset/(samples*0.5)){
		for(float y= -offset; y < offset; y += offset/(samples*0.5)){
			for(float z = -offset; z < offset; z += offset/(samples*0.5)){
				float closestDepth = texture(depth_map, fragToLight + vec3(x,y,z)).r;
				closestDepth *= far_plane;

				if(curDepth - bias > closestDepth){
					shadow += 1.0;
				}
			}
		}
	}
	shadow /= (samples*samples*samples);


	return shadow;
}

vec3 sampleOffsetDirections[20] = vec3[](
	vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

/**
* ������ԭʼ�������������������������ڲ������������Ĵ�ֱ������в����������塣
* �и����ɿ���ʹ�ã���һ��ƫ�����������飬���ǲ�඼�Ƿֿ��ģ�ÿһ��ָ����ȫ��ͬ�ķ����޳��˴˽ӽ�����Щ�ӷ����������һ������20��ƫ�Ʒ�������飺
* ���ǰ�PCF�㷨���sampleOffsetDirections�õ������������������䣬ʹ�����Ǵ���������ͼ�������
* ��ô���ĺô�����֮ǰ��PCF�㷨��ȣ�������Ҫ���������������ˡ�
*/
float calcShadow3(vec3 fragPos){
	vec3 fragToLight = fragPos - lightPos;
	float curDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	//float diskRadius = 0.05;

	//���Ի��ڹ۲�����һ��fragment�ľ������ı�diskRadius��
	//�������Ǿ��ܸ��ݹ۲��ߵľ���������ƫ�ư뾶�ˣ��������Զ��ʱ����Ӱ����ͣ������˾͸�������
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;

	for(int i=0; i<samples; ++i) {
		float closestDepth = texture(depth_map, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;

		if(curDepth - bias > closestDepth) {
			shadow += 1.0;
		}
	}

	shadow /= float(samples);
	return shadow;
}

void main(){

	vec3 tex1 = texture(diffuse_texture, vs_in.texCoords).rgb;
	vec3 norm = normalize(vs_in.fragNorm);
	vec3 lightDir = normalize(lightPos - vs_in.fragPos);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);

	vec3 halfwayDir = normalize(lightDir + viewDir);

	//ambient
	vec3 ambient = 0.3 * tex1;
	//diffuse
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;
	//specular 
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	
	float shadow = hasShadow ? calcShadow3(vs_in.fragPos) : 0.0;
	vec3 lighting = ( ambient + (1.0 - shadow) * (diffuse + specular) ) * tex1;

	fragColor = vec4(lighting, 1.0f);
}