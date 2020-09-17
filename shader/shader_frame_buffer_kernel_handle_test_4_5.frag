#version 330 core

uniform sampler2D screenTexture;

in vec2 texCoords;

out vec4 fragColor;

const float offset = 1.0/300.0;

void main(){

	//9��ƫ����������, ���ڶ������������ƫ��
	vec2 offsets[9]  = {
		vec2( -offset , offset ),  //����
		vec2( 0.0f , offset ),  //����
		vec2( offset, offset ),  //����

		vec2( -offset , 0.0f ),  //����
		vec2( 0.0f, 0.0f),  //��`
		vec2( offset, 0.0f),  //��

		vec2( -offset, -offset),  //����
		vec2( 0.0f, -offset ),  //����
		vec2( offset, -offset ),  //����
	};

	//��(Sharpen)�ˣ����������Χ���������أ���ÿ����ɫֵ��
	float kernel_sharpen[9] = {
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	};

	//����ģ��(Blur)�ĺ�
	//��������ֵ�ĺ���16������ֱ�ӷ��غϲ��Ĳ�����ɫ�������ǳ�������ɫ������������Ҫ���˵�ÿ��ֵ������16��
	float kernel_blur[9] = {
		1.0f/16, 2.0f/16, 1.0f/16,
		2.0f/16, 4.0f/16, 2.0f/16,
		1.0f/16, 2.0f/16, 1.0f/16
	};

	//��Ե���(Edge-detection)�˺��񻯺˷ǳ�����
	//����˸��������еı�Ե�����������������֣�������ֻ����ͼ��ı߽ǵ�ʱ���Ƿǳ����õġ�
	float kernel_edge_detection[9] = {
		1, 1,  1,
		1, -8, 1,
		1, 1,  1
	};

	//Ȼ���ڲ���ʱ���ǽ�ÿ��ƫ�����ӵ���ǰ���������ϣ���ȡ��Ҫ����������
	vec3 sampleTex[9];
	for(int i=0; i<9; i++){
		sampleTex[i] = vec3( texture(screenTexture, texCoords.st + offsets[i]));
	}
  	
	//֮����Щ����ֵ���Լ�Ȩ�ĺ�ֵ���������Ǽӵ�һ��
	vec3 col =  vec3(0.0);
	for(int i=0; i<9; i++){
		//col += sampleTex[i] * kernel_sharpen[i];  //�� 
		//col += sampleTex[i] * kernel_blur[i];      //ģ��
		col += sampleTex[i] * kernel_edge_detection[i];  //��Ե���
	}

	fragColor = vec4(col, 1.0);
}