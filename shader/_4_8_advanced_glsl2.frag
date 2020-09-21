#version 330 core

out vec4 fragColor;

uniform vec3 ourColor1;
uniform vec3 ourColor2;
uniform vec3 ourColor3;
uniform vec3 ourColor4;

void main(){

//gl_FragCoord的z分量等于对应片段的深度值。
//gl_FragCoord的x和y分量是片段的窗口空间(Window-space)坐标，其原点为窗口的左下角。
//我们已经使用glViewport设定了一个800x600的窗口了，所以片段窗口空间坐标的x分量将在0到800之间，y分量在0到600之间。
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