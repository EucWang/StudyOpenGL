#version 330 core


in vec2 texCoords;

out float fragColor;

uniform sampler2D texture_ssaoInput;

void main(){
	
	vec2 texelSize = 1.0 / vec2(textureSize(texture_ssaoInput, 0));

	float result = 0;

	//遍历了周围在-2.0 ~ 2.0 之间的SSAO纹理单元Texel
	//采样与噪声纹理维度相同数量的SSAO纹理
	//通过使用返回vec2纹理维度的textureSize,根据纹理单元的真实大小偏移了每一个纹理坐标
	//然后平均所得的结果，获取一个简单但是有效的模糊效果
	for(int x = -2; x < 2; ++x){
		for(int y = -2; y < 2; ++y) {
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(texture_ssaoInput, texCoords + offset).r;
		}
	}
	fragColor = result / (4.0 * 4.0);

}