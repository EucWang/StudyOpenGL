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

//是否应用阴影计算
uniform bool hasShadow;

vec3 lightColor = vec3(0.3);

float calcShadow(vec3 fragPos) {
	//我们已经将深度储存为fragment和光位置之间的距离了；我们这里采用相似的处理方式：
	//得到了fragment的位置与光的位置之间的不同的向量
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
	//这里我们根据样本的数量动态计算了纹理偏移量，我们在三个轴向采样三次，最后对子样本进行平均化。
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
* 与其在原始方向向量附近处采样，不如在采样方向向量的垂直方向进行采样更有意义。
* 有个技巧可以使用，用一个偏移量方向数组，它们差不多都是分开的，每一个指向完全不同的方向，剔除彼此接近的那些子方向。下面就是一个有着20个偏移方向的数组：
* 我们把PCF算法与从sampleOffsetDirections得到的样本数量进行适配，使用它们从立方体贴图里采样。
* 这么做的好处是与之前的PCF算法相比，我们需要的样本数量变少了。
*/
float calcShadow3(vec3 fragPos){
	vec3 fragToLight = fragPos - lightPos;
	float curDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	//float diskRadius = 0.05;

	//可以基于观察者里一个fragment的距离来改变diskRadius；
	//这样我们就能根据观察者的距离来增加偏移半径了，当距离更远的时候阴影更柔和，更近了就更锐利。
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