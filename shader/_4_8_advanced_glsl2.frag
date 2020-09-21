#version 330 core

out vec4 fragColor;

uniform vec3 ourColor1;
uniform vec3 ourColor2;
uniform vec3 ourColor3;
uniform vec3 ourColor4;

void main(){

//gl_FragCoord��z�������ڶ�ӦƬ�ε����ֵ��
//gl_FragCoord��x��y������Ƭ�εĴ��ڿռ�(Window-space)���꣬��ԭ��Ϊ���ڵ����½ǡ�
//�����Ѿ�ʹ��glViewport�趨��һ��800x600�Ĵ����ˣ�����Ƭ�δ��ڿռ������x��������0��800֮�䣬y������0��600֮�䡣
	if(gl_FragCoord.x < 400 && gl_FragCoord.y < 400){
		fragColor = vec4(ourColor1, 1.0);
	} else  if(gl_FragCoord.x < 400 && gl_FragCoord.y >= 400){
		fragColor = vec4(ourColor2, 1.0);	
	} else if(gl_FragCoord.x >= 400 && gl_FragCoord.y < 400) {
	fragColor = vec4(ourColor3, 1.0);	
	} else if(gl_FragCoord.x >= 400 && gl_FragCoord.y >= 400){
		fragColor = vec4(ourColor4, 1.0);	
	} else {
		fragColor = vec4(1.0);	
	}
}