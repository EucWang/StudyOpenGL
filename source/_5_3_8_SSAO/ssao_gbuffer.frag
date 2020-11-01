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

	//����Ƭ�ε�λ��ʸ������һ��G��������
	gPositionDepth.xyz = vs_in.fragPos;
	//����������ȵ�gPositionDepth.alpha����
	gPositionDepth.a = linearize_depth(gl_FragCoord.z);
	//���淨����Ϣ��G���� 
	gNormal = normalize(vs_in.fragNorm);
	//����������ɫ
	gAldedoSpec.rgb = vec3(0.95);

}