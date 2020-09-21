#version 330 core

//in vec2 texCoords;


/**
* 定义一个输入接口块。
* 块名(Block Name)应该是和着色器中一样的（VS_OUT），
* 但实例名(Instance Name)（顶点着色器中用的是vs_out）可以是随意的，
* 但要避免使用误导性的名称，比如对实际上包含输入变量的接口块命名为vs_out。
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