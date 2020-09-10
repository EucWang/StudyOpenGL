#version 330 core

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

in vec2 ourTexCoord;
void main(){

	fragColor = texture(texture_diffuse1, ourTexCoord); 

}