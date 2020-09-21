#version 330 core

out vec4 fragColor;

uniform vec3 cubeColor;


void main(){

	fragColor = vec4(cubeColor, 1.0);

}