#version 330 core

//��Ϊ����ʹ���˶���ȾĿ�꣬�������ָʾ��(Layout Specifier)������OpenGL������Ҫ��Ⱦ����ǰ�Ļ�Ծ֡�����е���һ����ɫ���塣
//ע�����ǲ�û�д��澵��ǿ�ȵ�һ����������ɫ���������У���Ϊ���ǿ��Դ����������ĸ���ֵ��������ɫ���������alpha�����С�
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
	//�洢��һ��G���������е�Ƭ��λ������
	gPosition = vs_in.fragPos;
	//ͬ���洢ÿ�����Ƭ�η��ߵ�G������
	gNormal = normalize(vs_in.fragNorm);
	//�洢����������Ƭ����ɫ
	gAlbedoSpec.rgb = texture(texture_diffuse1, vs_in.texCoords).rgb;
	//�洢����ǿ�ȵ�gAlbedoSpec��alpha����
	gAlbedoSpec.a = texture(texture_specular1, vs_in.texCoords).a;
}
