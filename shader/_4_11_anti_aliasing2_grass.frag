#version 330 core
in VS_OUT{
	vec2 texCoords;
} vs_in;

uniform sampler2D texture_diffuse1;

out vec4 fragColor;

void main(){
	vec4 tex = texture(texture_diffuse1, vs_in.texCoords);
	if(tex.a < 0.1){
		discard;
	}
	fragColor = tex;
}

