#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

//out vec4 fragColor;
//uniform sampler2D texture_diffuse;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
	//vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	//fragColor = vec4(tex, 1.0);

	gPosition = vs_in.fragPos;
	gNormal = normalize(vs_in.fragNorm);
	//gAlbedo = vec3(0.95); //  // and the diffuse per-fragment color
	//存储漫反射的逐个片段颜色
	gAlbedoSpec.rgb = texture(texture_diffuse1, vs_in.texCoords).rgb;
	//存储镜面强度到gAlbedoSpec的alpha分量
	gAlbedoSpec.a = texture(texture_specular1, vs_in.texCoords).a;
}