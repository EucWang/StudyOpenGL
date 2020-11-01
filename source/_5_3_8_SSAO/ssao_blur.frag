#version 330 core


in vec2 texCoords;

out float fragColor;

uniform sampler2D texture_ssaoInput;

void main(){
	
	vec2 texelSize = 1.0 / vec2(textureSize(texture_ssaoInput, 0));

	float result = 0;

	//��������Χ��-2.0 ~ 2.0 ֮���SSAO����ԪTexel
	//��������������ά����ͬ������SSAO����
	//ͨ��ʹ�÷���vec2����ά�ȵ�textureSize,��������Ԫ����ʵ��Сƫ����ÿһ����������
	//Ȼ��ƽ�����õĽ������ȡһ���򵥵�����Ч��ģ��Ч��
	for(int x = -2; x < 2; ++x){
		for(int y = -2; y < 2; ++y) {
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(texture_ssaoInput, texCoords + offset).r;
		}
	}
	fragColor = result / (4.0 * 4.0);

}