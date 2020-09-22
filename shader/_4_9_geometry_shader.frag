#version 330 core

out vec4 fragColor;


//in VS_INTERFACE{
//	vec3 ourColor;
//} vs_in;
//

in vec3 fColor;

void main(){
	//fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	fragColor = vec4(fColor, 1.0f);
}