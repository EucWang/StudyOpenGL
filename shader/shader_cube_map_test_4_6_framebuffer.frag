#version 330 core

in vec2 texCoords;

uniform sampler2D screen_texture;

out vec4 fragColor;
void main(){

	fragColor = vec4(texture(screen_texture, texCoords).rgb, 1.0);

}