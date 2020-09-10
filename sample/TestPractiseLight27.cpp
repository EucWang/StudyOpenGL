#include "../include/TestPractiseLight27.h"
#include "../include/Camera.h"

double deltaTime;
float lastFrame;
double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
bool isMouseFirstIn = true;

Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));


static void mouse_move_callback(GLFWwindow* window, double posX, double posY) {
	if (isMouseFirstIn) {
		isMouseFirstIn = false;
		lastX = posX;
		lastY = posY;
	}

	double offsetX = posX - lastX;
	double offsetY = lastY - posY;

	lastX = posX;
	lastY = posY;

	camera.ProcessMouseMovement(offsetX, offsetY);
}

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.ProcessMouseScroll(offsetY);
}


int TestPractiseLight27::practiseLight27(const char * projectDir) {

	GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "draw model practise");
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glEnable(GL_DEPTH_TEST);

	MyShader ourShader(projectDir, vertextFile, fragFile);


	return 1;
}

bool TestPractiseLight27::prepare(const char* projectDir) {
	
	return true;
}

void TestPractiseLight27::render() {
	
}

void TestPractiseLight27::processInput(GLFWwindow* window) {
	
}
 

