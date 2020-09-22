#version 330 core


//in VS_OUT {
//	vec2 texCoords;
//} vs_in;


//in vec2 ourTexCoords;

out vec4 fragColor;

//uniform sampler2D texture_diffuse1;

void main(){
	//fragColor = texture(texture_diffuse1, vs_in.texCoords);
	//fragColor = texture(texture_diffuse1, ourTexCoords);
	fragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
