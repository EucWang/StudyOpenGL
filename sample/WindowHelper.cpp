#include "WindowHelper.h"

static double deltaTime;
static double lastFrame;

static double localLastX = DEFAULT_SCREEN_WIDTH / 2;
static double localLastY = DEFAULT_SCREEN_HEIGHT / 2;

static bool isMouseFirstIn = true;

static bool blinnKeyPressed = false;  //是否按下了B键
static bool blinn = false;  //x

static bool spotKeyPressed = false;   //是否按下了V键
static bool spot = false;

static bool keyNPressed = false; //是否按下了N键
static bool keyN = false;

static Camera localCamera = Camera(glm::vec3(0.0f, 2.0f, 4.0f));

static void buffer_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/// <summary>
/// 处理单个的点击事件
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case  GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
		break;
		case GLFW_KEY_B:
			if (!blinnKeyPressed) blinn = !blinn;
			blinnKeyPressed = true;
			break;
		case GLFW_KEY_V:	
			if (!spotKeyPressed) spot = !spot;
			spotKeyPressed = true;
			break;
		case GLFW_KEY_N:
			if (!keyNPressed) keyN = !keyN;
			keyNPressed = true;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_B:
			blinnKeyPressed = false;
			break;
		case GLFW_KEY_V:
			spotKeyPressed = false;
			break;
		case GLFW_KEY_N:
			keyNPressed = false;
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 在每一帧中处理键盘事件， 这个用于处理长时间连续的按键最好了
/// </summary>
/// <param name="window"></param>
static void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		localCamera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		localCamera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		localCamera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		localCamera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

bool WindowHelper::switchByClickKeyB() {
	return blinn;
}

bool WindowHelper::switchByClickKeyV() {
	return spot;
}


/// <summary>
/// 预设按键N， 第一次点击V键时， 返回true， 再次点击V键时，返回false
/// </summary>
/// <returns></returns>
bool WindowHelper::switchByClickKeyN() {
	return keyN;
}

static void mouse_move_callback(GLFWwindow* window, double posX, double posY) {
	if (isMouseFirstIn) {
		isMouseFirstIn = false;
		localLastX = posX;
		localLastY = posY;
	}

	double offsetX = posX - localLastX;
	double offsetY = localLastY - posY;

	localLastX = posX;
	localLastY = posY;

	localCamera.ProcessMouseMovement(offsetX, offsetY);
}

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	localCamera.ProcessMouseScroll(offsetY);
}

void (*WindowHelper::getMouseScrollCallbackFunc())(GLFWwindow*, double, double) {
	void (*retVal)(GLFWwindow*, double, double) = mouse_scroll_callback;
	return retVal;
}

void(*WindowHelper::getBufferWindowCallbackFunc())(GLFWwindow* window, int width, int height) {
	void (*retVal)(GLFWwindow * window, int width, int height) = buffer_window_callback;
	return retVal;
}

void(*WindowHelper::getMouseMoveCallbackFunc())(GLFWwindow*, double, double) {
	void (*retVal)(GLFWwindow*, double, double) = mouse_move_callback;
	return retVal;
}

void (*WindowHelper::getProcessInputFunc())(GLFWwindow*) {
	void (*retVal)(GLFWwindow*) = processInput;
	return retVal;
}

WindowHelper::WindowHelper(const char* title,	Camera camera,
	int antiAliasing,	int width,	int height,
	int majorVersion,	int minorVersion,	bool resize) {

	this->mTitle = string(title);

	this->mMajorVersion = majorVersion;
	this->mMinorVersion = minorVersion;
	this->mResize = resize;

	this->mScreenWidth = width;
	this->mScreenHeight = height;
	localCamera = camera;

	this->mAntiAliasing = antiAliasing;

	isMouseFirstIn = true;

	localLastX = width / 2;
	localLastY = height / 2;
}

void WindowHelper::calcProcessInput() {
	calcDeltaTime();
	processInput(mWindow);
}

int  WindowHelper::getScreenWidth() {
	return this->mScreenWidth;
}

int  WindowHelper::getScreenHeight() {
	return this->mScreenHeight;
}


void WindowHelper::calcDeltaTime() {

	double curFrame = glfwGetTime();
	deltaTime = curFrame - lastFrame;
	lastFrame = curFrame;
}

GLFWwindow* WindowHelper::getWindow() {
	return mWindow;
}

Camera WindowHelper::getCamera() {
	return localCamera;
}

void WindowHelper::create() {
	//-----------------------------window 
	const char* title = "render with anti aliasing in custom frame buffer";
	//GLFWwindow * window = RenderUtil::createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
		//"render with anti aliasing in custom frame buffer.", buffer_window_callback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (this->mResize) {
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	} else {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

	if (this->mAntiAliasing > 0) {
		glfwWindowHint(GLFW_SAMPLES, this->mAntiAliasing);
	}

	//mWindow = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, title, nullptr, nullptr);
	mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, title, nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR::WINDOW_LOAD::FAILED\t gladLoadGLLoader failed." << std::endl;
		return;
	}

	//glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	glViewport(0, 0, mScreenWidth, mScreenHeight);

	glfwSetFramebufferSizeCallback(mWindow, buffer_window_callback);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//void (*GLFWcursorposfun)(GLFWwindow*, double, double);
	//GLFWcursorposfun = mouse_move_callback;

	glfwSetCursorPosCallback(mWindow, mouse_move_callback);
	glfwSetScrollCallback(mWindow, mouse_scroll_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	//-----------------------------window done

	//启用多重采样
	if (this->mAntiAliasing > 0) {
		glEnable(GL_MULTISAMPLE);
	}
}

//TODO 
//void WindowHelper::run(void (*render)(void)) {
//
//	while (!glfwWindowShouldClose(mWindow)) {
//		double curFrame = glfwGetTime();
//		deltaTime = curFrame - lastFrame;
//		lastFrame = curFrame;
//
//		processInput(mWindow);
//
//		glClearColor(mBackgroundColor.x, mBackgroundColor.y, mBackgroundColor.z, 1.0f);
//		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		//------render 
//		render();
//		//------render done
//
//		glfwSwapBuffers(mWindow);
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//}