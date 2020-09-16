#version 330 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main(){

	fragColor = texture(screenTexture, texCoords);
	//���۶���ɫ��������,����ɫ����ô����,
	//����ʹ�ü�Ȩ��ͨ��,�ﵽ������ʵ�ĻҶ�Ч��
	float average = (0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b);

	fragColor = vec4(average, average, average, 1.0);

}