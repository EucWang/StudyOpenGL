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

vec3 lightColor = vec3(1.0);

float calcShadow(vec3 fragPos) {
	//我们已经将深度储存为fragment和光位置之间的距离了；我们这里采用相似的处理方式：
	
	//得到了fragment的位置与光的位置之间的不同的向量
	vec3 fragToLight = fragPos - lightPos;

	//float closestDepth = texture(depth_map, fragToLight).r;
	//closestDepth *= far_plane;
	float closestDepth = 0.0;

	float curDepth = length(fragToLight);

	float bias = 0.05;
	float samples = 4.0;
	float offset = 0.1;
	//float shadow = curDepth - bias > closestDepth ? 1.0 : 0.0;
	float shadow = 0.0;
	//这里我们根据样本的数量动态计算了纹理偏移量，我们在三个轴向采样三次，最后对子样本进行平均化。

	//TODO samples设置为4.0，每个fragment我们会得到总共64个样本，这太多了！
	//TODO需要优化
	for(float x = -offset; x < offset; x += offset/(samples*0.5)) {
		for(float y = -offset; y < offset; y += offset/(samples*0.5)) {
			for(float z = -offset; z < offset; z += offset/(samples*0.5)) {
				float closestDepth = texture(depth_map, fragToLight + vec3(x, y, z)).r;
				closestDepth += far_plane;
				if(curDepth - bias > closestDepth) {
					shadow += 1.0;
				}
			}
		}
	}

	shadow /= (samples * samples * samples);
	return shadow;
}

void main(){

	vec3 tex1 = texture(diffuse_texture, vs_in.texCoords).rgb;
	vec3 norm = normalize(vs_in.fragNorm);
	vec3 lightDir = normalize(lightPos - vs_in.fragPos);
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);

	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = 0.0;
	spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	
	float shadow = calcShadow(vs_in.fragPos);
	vec3 lighting = ( ambient + (1.0 - shadow) * (diffuse + specular) ) * tex1;

	fragColor = vec4(lighting, 1.0f);
}