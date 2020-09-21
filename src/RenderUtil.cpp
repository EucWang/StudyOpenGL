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

	//�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, width, height);
	//ע������ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ�������������� 
	//(void * func)(GLFWwindow* window, int width, int height) 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}


/// <summary>
/// ����2DͼƬ����
/// </summary>
/// <param name="parentDir">ͼƬ���ڸ���Ŀ¼</param>
/// <param name="imgName">ͼƬ��Ŀ¼�Լ�ͼƬ�ļ�����</param>
/// <returns>  -1����ʾ����ʧ�ܣ� >=0 ��ʾ���سɹ�,����ͼƬ����id</returns>
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
/// ���� ��պ��� ��3d����
/// </summary>
/// <param name="projectDir">ͼƬ���ڵĸ���Ŀ¼</param>
/// <param name="images">��������6�����ͼƬ����</param>
		// order:
		// +X (right)
		// -X (left)
		// +Y (top)
		// -Y (bottom)
		// +Z (front) 
		// -Z (back)
/// <returns>������պ��ӵ�3d����id, ����С��0��ʾ����ʧ��</returns>
int RenderUtil::texureLoadCubmap(const char* projectDir, vector<string> images) {
	unsigned int textCubeMap;
	glGenTextures(1, &textCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textCubeMap);  //��������������ͼ
	int width, height, nrChannels;
	unsigned char* data;
	//��˳��
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
		stbi_image_free(data);  //������ɾͿ����ͷ�data��

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//���ǽ����Ʒ�ʽ����ΪGL_CLAMP_TO_EDGE��
		//������Ϊ���ô���������֮�������������ܲ��ܻ���һ���棨����һЩӲ�����ƣ���
		//����ͨ��ʹ��GL_CLAMP_TO_EDGE��OpenGL�������Ƕ�������֮�������ʱ����Զ�������ǵı߽�ֵ��

		free(imagePath);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textCubeMap;
}

/// <summary>
/// ���� ֡����
/// ������ 2d�������Ⱦ����
/// </summary>
/// <param name="framebufferobj"></param>
/// <param name="frametexture"></param>
/// <param name="renderbuffer"></param>
/// <returns>1��ʾ�ɹ�,����ʧ��</returns>
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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);  //���֡����İ�
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
/// ���� VAO VBO 
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
	else if (layoutSize == 4) {  //�������, ����д���������, �д��Ż�
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
