#ifndef  TEST_PRACTISE_LIGHT27_H_
#define TEST_PRACTISE_LIGHT27_H_

#include "Camera.h"
#include "fileUtil.h"
#include "shaders.h"

#include "shaderSource.h"
#include "util.h"

#include "Model.h"
#include "Mesh.h"
#include "MyShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;


class TestPractiseLight27 {
public:

	const char* vertextFile = "shader/shader_light_test27.vert";
	const char* fragFile = "shader/shader_light_test27.frag";

	GLuint VAO, VBO;// , lightVAO, lightVBO;
	int shaderId;// , shaderLightId;

	//表示光源在场景的世界空间坐标中的位置
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	int practiseLight27(const char* projectDir);
private:
	bool prepare(const char* projectDir);
	void render();

	void processInput(GLFWwindow* window);
};

#endif // ! TEST_PRACTISE_LIGHT27_H_
