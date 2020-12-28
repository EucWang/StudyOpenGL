#include "RenderUtil.h"



GLFWwindow* RenderUtil::createWindow(int width, int height, const char* title,
	void (*framebuffer_size_callback)(GLFWwindow* window, int width, int height)) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return NULL;
	}

	//两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	glViewport(0, 0, width, height);
	//注册这个回调函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数： 
	//(void * func)(GLFWwindow* window, int width, int height) 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}


/// <summary>
/// 加载2D图片纹理，
/// </summary>
/// <param name="parentDir">图片所在父级目录</param>
/// <param name="imgName">图片子目录以及图片文件名称</param>	
/// <param name="isSRGB">diffuse纹理，这种为物体上色的纹理几乎都是在sRGB空间中的。
/// 而为了获取光照参数的纹理，像specular贴图和法线贴图几乎都在线性空间中，
/// 所以如果你把它们也配置为sRGB纹理的话，光照就坏掉了。指定sRGB纹理时要当心。</param>
/// <returns>  -1：表示加载失败， >=0 表示加载成功,返回图片纹理id</returns>
int RenderUtil::textureLoad2D(std::string parentDir, std::string imgName, bool isSRGB) {
	return textureLoad2D(parentDir, imgName, isSRGB, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}

int RenderUtil::textureLoad2D(std::string parentDir, std::string imgName, bool isSRGB,
	int wrapS, int wrapT, int minFilter, int magFilter) {
	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, channel;
	char* imagePath;
	if (!getChildPath(&imagePath, parentDir.c_str(), imgName.c_str())) {
		std::cout << "textureLoadImg() load failed, because getChildPath() failed." << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &channel, 0);
	if (data == NULL || *data == NULL) {
		std::cout << "textureLoadImg() load failed,  because stbi_load() failed." << std::endl;
		stbi_image_free(data);
		return -1;
	}

	GLenum format = 0, format2 = 0;
	if (channel == 1) { format = GL_RED; }
	else if (channel == 3) { 
		if (isSRGB) {
			format = GL_SRGB;
		}
		else {
			format = GL_RGB;
		}
		format2 = GL_RGB;
	}
	else if (channel == 4) {
		if (isSRGB) {
			format = GL_SRGB_ALPHA;
		}
		else {
			format = GL_RGBA;
		}
		format2 = GL_RGBA;
	}
	else {
		std::cout << "channel is " << channel << ", then format is not right." << std::endl;
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	stbi_image_free(data);
	free(imagePath);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;

}

/// <summary>
/// 加载 天空盒子 的3d纹理
/// </summary>
/// <param name="projectDir">图片所在的父级目录</param>
/// <param name="images">天空纹理的6个面的图片集合</param>
		// order:
		// +X (right)
		// -X (left)
		// +Y (top)
		// -Y (bottom)
		// +Z (front) 
		// -Z (back)
/// <returns>返回天空盒子的3d纹理id, 返回小于0表示加载失败</returns>
int RenderUtil::texureLoadCubmap(const char* projectDir, vector<string> images) {
	unsigned int textCubeMap;
	glGenTextures(1, &textCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textCubeMap);  //绑定立方体纹理贴图
	int width, height, nrChannels;
	unsigned char* data;
	//依顺序
	for (unsigned int i = 0; i < images.size(); i++) {
		char* imagePath;
		if (!getChildPath(&imagePath, projectDir, images[i].c_str())) { return -1; }

		data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
		if (data == NULL) {
			std::cout << "Cubemap texture failed to laod at path : " << std::endl;
			stbi_image_free(data);
			return -1;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
			0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);  //加载完成就可以释放data了

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//我们将环绕方式设置为GL_CLAMP_TO_EDGE，
		//这是因为正好处于两个面之间的纹理坐标可能不能击中一个面（由于一些硬件限制），
		//所以通过使用GL_CLAMP_TO_EDGE，OpenGL将在我们对两个面之间采样的时候，永远返回它们的边界值。

		free(imagePath);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textCubeMap;
}

/// <summary>
/// 
/// 添加颜色缓冲/深度缓冲,颜色附件Attachment
/// 附件是一个内存位置，它能够作为帧缓冲的一个缓冲，可以将它想象为一个图像。当创建一个附件的时候我们有两个选项：
/// 纹理或渲染缓冲对象(Renderbuffer Object)。
///
/// 纹理附件,当把一个纹理附加到帧缓冲的时候，所有的渲染指令将会写入到这个纹理中，就想它是一个普通的颜色/深度或模板缓冲一样。
///	使用纹理的优点是，所有渲染操作的结果将会被储存在一个纹理图像中，我们之后可以在着色器中很方便地使用它。
/// </summary>
/// <param name="texture"> out </param>
static void make_texture_attach(unsigned int* texColorBuffer, int index, int width, int height, int samples) {
	glGenTextures(1, texColorBuffer);

	if (samples <= 0) {
		glBindTexture(GL_TEXTURE_2D, *texColorBuffer);
		//主要的区别就是，我们将维度设置为了屏幕大小（尽管这不是必须的），并且我们给纹理的data参数传递了NULL。
		//对于这个纹理，我们仅仅分配了内存而没有填充它。填充这个纹理将会在我们渲染到帧缓冲之后来进行。
		//同样注意我们并不关心环绕方式或多级渐远纹理，我们在大多数情况下都不会需要它们。
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);  //清空绑定
	}else {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *texColorBuffer);
		//最后一个参数为GL_TRUE，图像将会对每个纹理像素使用相同的样本位置以及相同数量的子采样点个数。
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB,
			width, height, GL_TRUE);

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);  //清空绑定
	}

	std::cout << "make_texture_attach : texture = " << *texColorBuffer << std::endl;
}

/// <summary>
/// 生成一个渲染缓冲对象, 带深度和模板缓冲的
/// </summary>
/// <param name="rbo"></param>
static void make_render_buffer_obj_attach(unsigned int* rbo, int width, int height, int samples) {
	glGenRenderbuffers(1, rbo);   //创建渲染缓冲对象

	glBindRenderbuffer(GL_RENDERBUFFER, *rbo);  //绑定渲染缓冲对象

	if (samples <= 0) {
		//创建一个深度和模板渲染缓冲对象
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	} else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8,
			width, height);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0); //清理掉绑定

	std::cout << "make_render_buffer_obj_attach() : renderBuffer = " << *rbo << std::endl;
}


void RenderUtil::blitFrameBuffer(unsigned int* multisampleFramebuffer,
	unsigned int* intermediateFrameBuffer, int width, int height) {

	//-------------- 将自定义的多重采样缓冲和 中间非多重采样缓冲连接
	glBindFramebuffer(GL_READ_FRAMEBUFFER, *multisampleFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *intermediateFrameBuffer);
	glBlitFramebuffer(0, 0, width, height,
		0, 0, width, height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

}

/// <summary>
/// 创建一个普通的一般的 帧缓冲
/// </summary>
/// <param name="framebuffer">帧缓冲</param>
/// <param name="texColorBuffer">纹理, 帧缓冲附件</param>
/// <param name="renderBuffer">渲染缓冲, 帧缓冲附件</param>
/// <param name="bufferWidth">宽度, 纹理和渲染缓冲的高度</param>
/// <param name="bufferHeight">宽度, 纹理和渲染缓冲的宽度</param>
/// <returns></returns>
bool RenderUtil::makeFramebuffer(unsigned int* framebuffer,
	unsigned int* texColorBuffer, unsigned int* renderBuffer,
	int bufferWidth, int bufferHeight, int samples) {

	if (framebuffer == NULL) {
		std::cout << "makeFramebuffer() failed. because parameter1 'framebuffer' is null." << std::endl;
		return false;
	}

	//---------------------------------------------------------------------
	//unsigned int framebuffer;   //一个帧缓冲对象
	glGenFramebuffers(1, framebuffer);  //创建一个帧缓冲对象
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);  //绑定帧缓冲对象 到 GL_FRAMEBUFFER

	if (texColorBuffer != NULL) {
		//unsigned int texColorBuffer;  //一个空的纹理图像
		make_texture_attach(texColorBuffer, 0, bufferWidth, bufferHeight, samples);

		if (samples <= 0) {
			//然后把这个创建好的纹理附加到帧缓冲上, 这就附加了颜色缓冲
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				*texColorBuffer, 0);
		} else {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
				*texColorBuffer, 0);
		}
	}
	//附加带深度和模板缓冲的纹理
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SMALL_SCREEN_WIDTH,
	//	SMALL_SCREEN_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
	//	GL_TEXTURE_2D, texture, 0);

	//unsigned int renderBuffer;//渲染缓冲对象附件
	if (renderBuffer != NULL) {
		make_render_buffer_obj_attach(renderBuffer, bufferWidth, bufferHeight, samples);
		//附加这个渲染缓冲对象
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *renderBuffer);
	}
	//检查帧缓冲是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
		return false;
	}
	//解绑帧缓冲, 激活默认的窗口缓冲, 以便让主窗口能正常的渲染
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	std::cout << "makeFramebuffer() framebuffer = " << *framebuffer << std::endl;
	return true;
	//---------------------------------------------------------------------
}

void RenderUtil::makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, bool isnormal,
	const float* positions, const int positionsSize, const int arr1Offset) {
	makeVertexArrayFromSubData(vao, vbo, isnormal,
		positions, positionsSize, arr1Offset,
		NULL, 0, 0);
}

void RenderUtil::makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, bool isnormal,
	const float* positions, const int positionsSize, const int arr1Offset,
	const float* normals, int normalsSize, int arr2Offset) {
	makeVertexArrayFromSubData(vao, vbo, isnormal,
		positions, positionsSize, arr1Offset,
		normals, normalsSize, arr2Offset, NULL, 0, 0);

}

void RenderUtil::makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, bool isnormal,
	const float* positions, const int positionsSize, const int arr1Offset,
	const float* normals, int normalsSize, int arr2Offset,
	const float* texCoords, int texCoordsSize, int arr3Offset) {

	if (positions == NULL)	{
		std::cout << "ERROR::RenderUtil::makeVertexArrayFromSubData():: the first param positions is null, that is not correct." << std::endl;
		return;
	}
	if (normals == NULL) {
		normals = 0;
		arr2Offset = 0;
	}
	if (texCoords == NULL) {
		texCoordsSize = 0;
		arr3Offset = 0;
	}

	int isNormalPos;
	if (isnormal) {
		isNormalPos = GL_TRUE;
	}
	else {
		isNormalPos = GL_FALSE;
	}
	

	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);

	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, positionsSize + normalsSize + texCoordsSize, nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize, positions);

	if (normals != NULL) {
		glBufferSubData(GL_ARRAY_BUFFER, positionsSize, normalsSize, normals);
	}

	if	(texCoords != NULL) {
		glBufferSubData(GL_ARRAY_BUFFER, positionsSize + normalsSize, texCoordsSize, texCoords);
	}

	glVertexAttribPointer(0, arr1Offset, GL_FLOAT, isNormalPos, arr1Offset * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	if (normals != NULL) {
		glVertexAttribPointer(1, arr2Offset, GL_FLOAT, isNormalPos, arr2Offset * sizeof(float), (void*)(positionsSize));
	}
	glEnableVertexAttribArray(1);

	if (texCoords != NULL) {
		glVertexAttribPointer(2, arr3Offset, GL_FLOAT, isNormalPos, arr3Offset * sizeof(float),
			(void*)(positionsSize + normalsSize));
		glEnableVertexAttribArray(2);
	}

	glBindVertexArray(0);
}

/// <summary>
/// 创建 VAO VBO 
/// </summary>
/// <param name="vao"> in/out </param>
/// <param name="dataArr"> float data array </param>
/// <param name="dataArrSize"> float data array size </param>
/// <param name="layoutSize"> vertex file layout size , must be values of 3/5/8</param>
void RenderUtil::makeVertexArrayAndBuffer(GLuint* vao, GLuint* vbo, const float* dataArr,
	int dataArrSize, int layoutSize) {

	std::cout << "dataArrSize = " << dataArrSize << ", layoutSize = " << layoutSize << std::endl;
	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);
	std::cout << "generate vao =" << *vao << ", vbo = " << *vbo << std::endl;

	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, dataArrSize, dataArr, GL_STATIC_DRAW);

	if (layoutSize == 3) {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)0);
	}
	else if (layoutSize == 4) {  //特殊情况, 这样写是有问题的, 有待优化
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)(2 * sizeof(float)));
	}
	else if (layoutSize == 6) {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)(3 * sizeof(float)));

	}
	else if (layoutSize == 5 || layoutSize == 8) {
		int len = layoutSize / 3;
		std::cout << "len = " << len << std::endl;
		for (int i = 0; i <= len; i++) {
			glEnableVertexAttribArray(i);

			int size = 3;
			if (i == len) { size = layoutSize % 3; }
			std::cout << "i = " << i << ", size = " << size << std::endl;
			glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, layoutSize * sizeof(float), (void*)(i * 3 * sizeof(float)));
		}
	}
}

/**
* 输出打印 OpenGL错误信息
 */
GLenum RenderUtil::glCheckError(const char* file, int line) {
	int errorCode = 0;
	try {
		while ((errorCode = glGetError()) != GL_NO_ERROR) {
			std::string error;
			switch (errorCode) {
			case GL_INVALID_ENUM:
				error = "INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				error = "INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				error = "INVALID_OPERATION";
				break;
				//#define GL_STACK_OVERFLOW                 0x0503
				//#define GL_STACK_UNDERFLOW                0x0504
				//case GL_STACK_OVERFLOW:
				//	error = "STACK_OVERFLOW";
				//	break;
				//case GL_STACK_UNDERFLOW:
				//	error = "STACK_UNDERFLOW";
				//	break;
			case GL_OUT_OF_MEMORY:
				error = "OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error = "INVALID_FRAMEBUFFER_OPERATION";
				break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
	}
	catch (exception &ex) {
		std::cout << "EXCEPTION:  " << ex.what() << std::endl;
	}
	return errorCode;
}
