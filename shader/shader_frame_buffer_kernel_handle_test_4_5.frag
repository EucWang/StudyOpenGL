#version 330 core

uniform sampler2D screenTexture;

in vec2 texCoords;

out vec4 fragColor;

const float offset = 1.0/300.0;

void main(){

	//9个偏移量的数组, 用于对纹理坐标进行偏移
	vec2 offsets[9]  = {
		vec2( -offset , offset ),  //左上
		vec2( 0.0f , offset ),  //正上
		vec2( offset, offset ),  //右上

		vec2( -offset , 0.0f ),  //左中
		vec2( 0.0f, 0.0f),  //中`
		vec2( offset, 0.0f),  //右

		vec2( -offset, -offset),  //左下
		vec2( 0.0f, -offset ),  //正下
		vec2( offset, -offset ),  //右下
	};

	//锐化(Sharpen)核，它会采样周围的所有像素，锐化每个颜色值。
	float kernel_sharpen[9] = {
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	};

	//创建模糊(Blur)的核
	//由于所有值的和是16，所以直接返回合并的采样颜色将产生非常亮的颜色，所以我们需要将核的每个值都除以16。
	float kernel_blur[9] = {
		1.0f/16, 2.0f/16, 1.0f/16,
		2.0f/16, 4.0f/16, 2.0f/16,
		1.0f/16, 2.0f/16, 1.0f/16
	};

	//边缘检测(Edge-detection)核和锐化核非常相似
	//这个核高亮了所有的边缘，而暗化了其它部分，在我们只关心图像的边角的时候是非常有用的。
	float kernel_edge_detection[9] = {
		1, 1,  1,
		1, -8, 1,
		1, 1,  1
	};

	//然后，在采样时我们将每个偏移量加到当前纹理坐标上，获取需要采样的纹理，
	vec3 sampleTex[9];
	for(int i=0; i<9; i++){
		sampleTex[i] = vec3( texture(screenTexture, texCoords.st + offsets[i]));
	}
  	
	//之后将这些纹理值乘以加权的核值，并将它们加到一起。
	vec3 col =  vec3(0.0);
	for(int i=0; i<9; i++){
		//col += sampleTex[i] * kernel_sharpen[i];  //锐化 
		//col += sampleTex[i] * kernel_blur[i];      //模糊
		col += sampleTex[i] * kernel_edge_detection[i];  //边缘检测
	}

	fragColor = vec4(col, 1.0);
}