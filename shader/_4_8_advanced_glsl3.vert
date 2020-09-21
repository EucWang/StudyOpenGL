#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

/**
* Uniform缓冲对象(Uniform Buffer Object)的工具，它允许我们定义一系列在多个着色器中相同的全局Uniform变量。
* 当使用Uniform缓冲对象的时候，我们只需要设置相关的uniform一次。
* 当然，我们仍需要手动设置每个着色器中不同的uniform。
*
* Uniform缓冲对象仍是一个缓冲，我们可以使用glGenBuffers来创建它，
* 将它绑定到GL_UNIFORM_BUFFER缓冲目标，并将所有相关的uniform数据存入缓冲。
*
* layout (std140) 的意思是
* 当前定义的Uniform块对它的内容使用一个特定的内存布局。
* 这个语句设置了Uniform块布局(Uniform Block Layout)。
* std140布局声明了每个变量的偏移量都是由一系列规则所决定的，这显式地声明了每个变量类型的内存布局。
* 由于这是显式提及的，我们可以手动计算出每个变量的偏移量。

* Uniform块的内容是储存在一个缓冲对象中的，它实际上只是一块预留内存。
* 因为这块内存并不会保存它具体保存的是什么类型的数据，我们还需要告诉OpenGL内存的哪一部分对应着着色器中的哪一个uniform变量。
*
* 每个变量都有一个基准对齐量(Base Alignment)，它等于一个变量在Uniform块中所占据的空间（包括填充量(Padding)），
* 这个基准对齐量是使用std140布局的规则计算出来的。
* 接下来，对每个变量，我们再计算它的对齐偏移量(Aligned Offset)，它是一个变量从块起始位置的字节偏移量。
* 一个变量的对齐字节偏移量必须等于基准对齐量的倍数。
*
* GLSL中的每个变量，比如说int、float和bool，都被定义为4字节量。每4个字节将会用一个N来表示。
* 
*-------------------------------------------------------------------------------
*|	类型						| 布局规则
*|-------------------------------------------------------------------------------
*|	标量，比如int和bool		| 每个标量的基准对齐量为N。
*|	向量						| 2N或者4N。这意味着vec3的基准对齐量为4N。
*|	标量或向量的数组			| 每个元素的基准对齐量与vec4的相同。
*|	矩阵	储存为列向量的数组，	| 每个向量的基准对齐量与vec4的相同。
*|	结构体					| 等于所有元素根据规则计算后的大小，但会填充到vec4大小的倍数。
*-------------------------------------------------------------------------------
*
* 例子如下：
*
* layout (std140) uniform ExampleBlock {
*                      // 基准对齐量       // 对齐偏移量
*     float value;     // 4               // 0 
*     vec3 vector;     // 16              // 16  (必须是16的倍数，所以 4->16)
*     mat4 matrix;     // 16              // 32  (列 0)
*                      // 16              // 48  (列 1)
*                      // 16              // 64  (列 2)
*                      // 16              // 80  (列 3)
*     float values[3]; // 16              // 96  (values[0])
*                      // 16              // 112 (values[1])
*                      // 16              // 128 (values[2])
*     bool boolean;    // 4               // 144
*     int integer;     // 4               // 148
* }; 
*
*/
layout (std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

/*
声明了一个叫做VS_OUT的接口块，它打包了我们希望发送到下一个着色器中的所有输出变量。
*/
out VS_OUT{
	vec2 texCoords;
} vs_out;

void main(){

	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.texCoords = aTexCoords;
}
