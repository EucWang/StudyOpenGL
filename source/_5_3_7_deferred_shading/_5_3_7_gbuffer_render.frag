#version 330 core

//因为我们使用了多渲染目标，这个布局指示符(Layout Specifier)告诉了OpenGL我们需要渲染到当前的活跃帧缓冲中的哪一个颜色缓冲。
//注意我们并没有储存镜面强度到一个单独的颜色缓冲纹理中，因为我们可以储存它单独的浮点值到其它颜色缓冲纹理的alpha分量中。
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 fragNorm;
} vs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
	//存储第一个G缓冲纹理中的片段位置向量
	gPosition = vs_in.fragPos;
	//同样存储每个逐个片段法线到G缓冲中
	gNormal = normalize(vs_in.fragNorm);
	//存储漫反射的逐个片段颜色
	gAlbedoSpec.rgb = texture(texture_diffuse1, vs_in.texCoords).rgb;
	//存储镜面强度到gAlbedoSpec的alpha分量
	gAlbedoSpec.a = texture(texture_specular1, vs_in.texCoords).a;
}
