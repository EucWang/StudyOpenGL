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
/// <returns>  -1：表示加载失败， >=0 表示加载成功,返回图片纹理id</returns>
int RenderUtil::textureLoad2D(std::string parentDir, std::string imgName) {
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

	GLenum format = 0;
	if (channel == 1) { format = GL_RED; }
	else if (channel == 3) { format = GL_RGB; }
	else if (channel == 4) { format = GL_RGBA; }
	else {
		std::cout << "channel is " << channel << ", then format is not right." << std::endl;
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
/// 创建 帧缓冲
/// 附件有 2d纹理和渲染缓冲
/// </summary>
/// <param name="framebufferobj"></param>
/// <param name="frametexture"></param>
/// <param name="renderbuffer"></param>
/// <returns>1表示成功,否则失败</returns>
int RenderUtil::createFramebuffer(GLuint* framebufferobj, GLuint* frametexture, GLuint* renderbuffer) {

	glGenFramebuffers(1, framebufferobj);
	glBindFramebuffer(GL_FRAMEBUFFER, *framebufferobj);

	glGenTextures(1, frametexture);
	glBindTexture(GL_TEXTURE_2D, *frametexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *frametexture, 0);
	std::cout << "attach texture to framebuffer." << std::endl;

	glGenRenderbuffers(1, renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
	std::cout << "attach renderbuffer to framebuffer." << std::endl;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: framebuffer is not complete." << std::endl;
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);  //解除帧缓冲的绑定
	std::cout << "createFramebuffer() success. [framebufferobj = " << *framebufferobj
		<< ", frametexture = " << *frametexture
		<< ", renderbuffer = " << *renderbuffer << "]" << std::endl;
	return 1;
}

void RenderUtil::makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo,
	const float* positions, const int positionsSize, const int arr1Offset) {
	makeVertexArrayFromSubData(vao, vbo,
		positions, positionsSize, arr1Offset,
		NULL, 0, 0);
}

void RenderUtil::makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo,
	const float* positions, const int positionsSize, const int arr1Offset,
	const float* normals, int normalsSize, int arr2Offset) {
	makeVertexArrayFromSubData(vao, vbo,
		positions, positionsSize, arr1Offset,
		normals, normalsSize, arr2Offset, NULL, 0, 0);

}

void RenderUtil::makeVertexArrayFromSubData(GLuint* vao, GLuint* vbo, 
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

	glVertexAttribPointer(0, arr1Offset, GL_FLOAT, GL_FALSE, arr1Offset * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	if (normals != NULL) {
		glVertexAttribPointer(1, arr2Offset, GL_FLOAT, GL_FALSE, arr2Offset * sizeof(float), (void*)(positionsSize));
	}
	glEnableVertexAttribArray(1);

	if (texCoords != NULL) {
		glVertexAttribPointer(2, arr3Offset, GL_FLOAT, GL_FALSE, arr3Offset * sizeof(float),
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
