#version  330 core

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

//out vec4 fragColor;
layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAldedoSpec; 

//uniform sampler2D texture_diffuse;

const float NEAR = 0.1f;
const float FAR = 50.0f;

float linearize_depth(float depth) {
	float z = depth * 2.0 - 1.0;
	return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}


void main() {
	//vec3 tex = texture(texture_diffuse, vs_in.texCoords).rgb;
	//fragColor = vec4(tex, 1.0);

	//储存片段的位置矢量到第一个G缓冲纹理
	gPositionDepth.xyz = vs_in.fragPos;
	//储存线性深度到gPositionDepth.alpha分量
	gPositionDepth.a = linearize_depth(gl_FragCoord.z);
	//储存法线信息到G缓冲 
	gNormal = normalize(vs_in.fragNorm);
	//和漫反射颜色
	gAldedoSpec.rgb = vec3(0.95);

}