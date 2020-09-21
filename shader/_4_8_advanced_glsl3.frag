#version 330 core

//in vec2 texCoords;


/**
* ����һ������ӿڿ顣
* ����(Block Name)Ӧ���Ǻ���ɫ����һ���ģ�VS_OUT����
* ��ʵ����(Instance Name)��������ɫ�����õ���vs_out������������ģ�
* ��Ҫ����ʹ�����Ե����ƣ������ʵ���ϰ�����������Ľӿڿ�����Ϊvs_out��
*/
in VS_OUT{
	vec2 texCoords;
} vs_in;


out vec4 fragColor;

uniform sampler2D texture_diffuse1;

uniform sampler2D texture_inside;

void main(){
	if (gl_FrontFacing) {
		fragColor = texture(texture_diffuse1, vs_in.texCoords);
	} else {
		fragColor = texture(texture_inside, vs_in.texCoords);
	}
}