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

	float closestDepth = texture(depth_map, fragToLight).r;

	closestDepth *= far_plane;

	float curDepth = length(fragToLight);

	float bias = 0.05;

	float shadow = curDepth - bias > closestDepth ? 1.0 : 0.0;

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