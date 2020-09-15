#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture1;

void main(){

	vec4 texColor = texture(texture1, texCoords);

	if(texColor.a < 0.1){
		discard;   //如果透明度小于0.1， 则丢弃这个片段，就好像它不存在一样
	} 

	fragColor = texColor;
}