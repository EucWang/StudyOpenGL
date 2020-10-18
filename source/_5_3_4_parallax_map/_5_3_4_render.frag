#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec2 texCoords;
	
	vec3 tangentFragPos;
	vec3 tangnetViewPos;
	vec3 tangnetLightPos;
}vs_in;

vec3 lightColor = vec3(0.5);
float shininess = 64.0;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_depth;

///�Ӳ�Ч�����û��һ�����Ų������п��ƣ�ͨ�������ǿ��
uniform float height_scale;

///���ؾ���λ��֮�����������
vec2 parallax_mapping(vec2 texCoords, vec3 viewDir) {
	//�õ���ǰfragment�ĸ߶�H(A)
	float height = texture(texture_depth, texCoords).r;
	//����õ� Pλ�ã�
	//vec2 p  =viewDir.xy / viewDir.z * (height * height_scale);
	//return texCoords - p;

	return texCoords - viewDir.xy * (height * height_scale);
}
	
const float minLayers = 8;
const float maxLayers = 32;

//**  �����Ӳ�ӳ��
vec2 parallax_mapping2(vec2 texCoords, vec3 viewDir) {
	//number of depth layers
	//const float numLayers = 10;

	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	
	//calculate the size of each layer
	float layerDepth = 1.0 / numLayers;
	//depth of current layerDepth
	float curLayerDepth = 0.0;
	//the amount to shift the texture coordinates per layer (from vector p)
	vec2 p = viewDir.xy * height_scale;
	vec2 deltaTexCoords = p / numLayers;

	//get initial values
	vec2 curTexCoords = texCoords;
	float curDepthMapValue = texture(texture_depth, curTexCoords).r;

	while(curLayerDepth < curDepthMapValue) {
		//shift texture coordinates along direction of p
		curTexCoords -= deltaTexCoords;
		//get depthMap value at current texture coordinates
		curDepthMapValue = texture(texture_depth, curTexCoords).r;
		//get depth of next layer
		curLayerDepth += layerDepth;
	}

	return texCoords - curTexCoords;
}


void main(){
	vec3 lightDir = normalize(vs_in.tangnetLightPos - vs_in.tangentFragPos);
	vec3 viewDir = normalize(vs_in.tangnetViewPos - vs_in.tangentFragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir );

	//�����ӽ������� �������꣬���¼��㣬�õ��Ӳ�֮��� ��������
	vec2 texCoords = parallax_mapping(vs_in.texCoords, viewDir);
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
		discard;
	}
	
	vec3 tex1 = texture(texture_diffuse, texCoords).rgb;
	vec3 norm = texture(texture_normal, texCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);

	vec3 ambient = 0.3 * tex1;

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(norm, halfwayDir),0.0), shininess);
	vec3 specular = spec * lightColor;

	fragColor = vec4((ambient + diffuse + specular) * tex1, 1.0);

}