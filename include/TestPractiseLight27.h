#ifndef  TEST_PRACTISE_LIGHT27_H_
#define TEST_PRACTISE_LIGHT27_H_

#include "Camera.h"
#include "fileUtil.h"
#include "shaders.h"

#include "shaderSource.h"

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
	const char* modelFile = "images/nanosuit/nanosuit.obj";

	const char* lightVertFile = "shader/shader_light_test27_light.vert";
	const char* lightFragFile = "shader/shader_light_test27_light.frag";


	int practiseLight27(const char* projectDir);
};

#endif // ! TEST_PRACTISE_LIGHT27_H_
