
#include "../include/test.h"
#include "../include/camera.h"
#include "../include/fileUtil.h"
#include "../include/shaders.h"
#include "../include/shaderSource.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>

using namespace std;

//空间三角形的点集坐标
static float vertices[] = {

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f

};

/**
* 空间中箱子的位置
*/
static glm::vec3 cubePositions[] = {

    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),

    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),

    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),

    glm::vec3(-1.3f,  1.0f, -1.5f)

};

static const char* vertexShaderFileName = "shader_camera_test17_vertex.txt";
static const char* fragShaderFileName = "shader_camera_test17_fragment.txt";

static const char* image1FileName = "container.jpg";
static const char* image2FileName = "awesomeface.png";

static const char* shaderDirName = "shader";
static const char* imagesDirName = "images";

static char* shaderDir;
static char* imagesDir;

static GLuint VAO, VBO, texture1, texture2;
static int shaderId;

static bool prepare();

static void render();

static void processInput(GLFWwindow * window);
static void mouse_move_callback(GLFWwindow * window, double xpos, double ypos);
static void mouse_scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

int practiseDrawWithCamera(char * projectDir) {

    //获取shader， iamges目录的路径
    if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }
    if (!getChildPath(&imagesDir, projectDir, imagesDirName)) { return -1; }

    GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with camera control");
    if (window == NULL) { return -1; }

    //获取shader文件路径
    char* vertexShaderPath;
    char* fragShaderPath;
    if (!getChildPath(&vertexShaderPath, shaderDir, vertexShaderFileName)) { return -1; }
    if (!getChildPath(&fragShaderPath, shaderDir, fragShaderFileName)) { return -1; }
    //创建shader
    if (!createShaderProgram(vertexShaderPath, fragShaderPath, &shaderId)) { return -1; }

    prepare();

    glEnable(GL_DEPTH_TEST);   //允许深度测试，保证显示的时候正常遮挡视线
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  //获取光标，隐藏光标
    glfwSetCursorPosCallback(window, mouse_move_callback);     //鼠标移动监听注册
    glfwSetScrollCallback(window, mouse_scroll_callback);  //鼠标滚轮活动监听注册

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glDeleteProgram(shaderId);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    free(vertexShaderPath);
    free(fragShaderPath);
    free(shaderDir);
    free(imagesDir);
    return 1;
}

//坐标系转换中间变量
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

//控制坐标系转换在着色器中的3个uniform单元
static int uniformLocModel;
static int uniformLocView;
static int uniformLocProjection;

static Camera camera;   //相机镜头对象

//中间变量
static double lastFrameTime = 0.0f;          //上一帧绘制的时间
static GLboolean isMouseFirstMove = true;   //是否是第一次鼠标移动进入窗口
static double deltaTime = 0.0f;              //和上一帧绘制的时间差

bool prepare() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    //加载点集数据到缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderId);
    //加载纹理
    textureGenSets(&texture1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    if (!textureLoadImg(imagesDir, image1FileName, GL_RGB)) { return false;}
    glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);

    textureGenSets(&texture2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    if (!textureLoadImg(imagesDir, image2FileName, GL_RGBA)) { return false; }
    glUniform1i(glGetUniformLocation(shaderId, "texture2"), 1);

    //获取着色器中的uniform变量
    uniformLocModel = glGetUniformLocation(shaderId, "model");
    uniformLocView = glGetUniformLocation(shaderId, "view");
    uniformLocProjection = glGetUniformLocation(shaderId, "projection");

    camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //创建Camera对象

    return true;
}

static int index = 0;

void render() {

    glUseProgram(shaderId);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    view = glm::mat4(1.0f);
    view = camera.GetViewMatrix();
    //view = glm::rotate(view, glm::radians(50.0f), glm::vec3(0.2f, 0.5f, 0.9f));

    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera.Zoom), SMALL_SCREEN_WIDTH / SMALL_SCREEN_HEIGHT * 1.0f, 0.1f, 100.0f);
    //projection = glm::perspective(glm::radians(45.0f), SMALL_SCREEN_WIDTH / SMALL_SCREEN_HEIGHT * 1.0f, 0.1f, 100.0f);

    for (index = 0; index < sizeof(cubePositions) / sizeof(cubePositions[0]); index++){
    //for (index = 0; index < 10; index++){
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[index]);

        glUniformMatrix4fv(uniformLocModel,1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformLocView,1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniformLocProjection,1, GL_FALSE, glm::value_ptr(projection));
    
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) );
    }

}


/**
* 键盘监控， 获取4个方向的移动
*/
void processInput(GLFWwindow* window) {
    //获取绘制每帧的时间间隔
    double currentTime = glfwGetTime();
    deltaTime = currentTime = lastFrameTime;
    lastFrameTime = currentTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
}

static double lastX, lastY;

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isMouseFirstMove) { 
        isMouseFirstMove = false;
        lastX = xpos;
        lastY = ypos;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xpos, ypos);
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
