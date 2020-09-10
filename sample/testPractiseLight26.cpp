#include "../include/camera.h"
#include "../include/fileUtil.h"
#include "../include/shaders.h"

#include "../include/shaderSource.h"
#include "../include/test.h"
#include "../include/util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

static const char* vertextFile = "shader/shader_light_test26.vert";
static const char* fragFile = "shader/shader_light_test26.frag";
static const char* lightVertFile = "shader/shader_light_test25_light.vert";
static const char* lightFragFile = "shader/shader_light_test26_light.frag";

static const char* imageContainer2File = "images/container2.png";
static const char* imageContainer2SpecularFile = "images/container2_specular.png";

static char* vertextFilePath;
static char* fragFilePath;
static char* lightVertFilePath;
static char* lightFragFilePath;

static GLuint VAO, VBO, lightVAO, lightVBO;
static int shaderId, shaderLightId;

static GLuint texture1, texture2;

static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
//表示光源在场景的世界空间坐标中的位置
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

static float vertices[] = {
    // positions          // normals           // texture coords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

static float lightVertices[] = {
    // positions    
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

// 箱子位置
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

//4个点光源的位置
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};


//----------------普通场景------------------------------
//定向光的4个数据
//glm::vec3 dirLightArgs[4] = {
//    glm::vec3(0.05f, 0.05f, 0.05f),  //ambient
//    glm::vec3(0.4f, 0.4f, 0.4f),     //diffuse
//    glm::vec3(0.5f, 0.5f, 0.5f),     //specular
//    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
//};
//
////点光源的6个数据
//glm::vec3 pointLightArgs[] = {
//    glm::vec3(0.05f, 0.05f, 0.05f),  //ambient
//    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
//    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
////聚光的6个数据
//glm::vec3 spotlightArgs[] = {
//    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
//    glm::vec3(0.8f, 0.8f, 0.8f),     //diffuse
//    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};

//----------------沙漠场景------------------------------
//定向光的4个数据
//static glm::vec3 dirLightArgs[4] = {
//    glm::vec3(0.3f, 0.24f, 0.14f),  //ambient
//    glm::vec3(0.7f, 0.42f, 0.26f),     //diffuse
//    glm::vec3(0.5f, 0.5f, 0.5f),     //specular
//    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
//};
//
////点光源的6个数据
//glm::vec3 pointLightColors[] = {
//    glm::vec3(1.0f, 0.6f, 0.0f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
//    glm::vec3(1.0f, 0.0f, 0.0f),  //第二个点光源的 (ambient, diffuse, specular)  红色
//    glm::vec3(1.0f, 1.0, 0.0),    //第三个点光源的 (ambient, diffuse, specular)  黄色
//    glm::vec3(0.2f, 0.2f, 1.0f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色
//
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
////聚光的6个数据
//static glm::vec3 spotlightArgs[] = {
//    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
//    glm::vec3(0.8f, 0.8f, 0.8f),     //diffuse
//    glm::vec3(0.8f, 0.8f, 0.8f),     //specular
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
//static glm::vec3 clearColor(0.75f, 0.52f, 0.3f);   //背景颜色
//
//static glm::vec2 spotlight_cutoff(12.5f, 13.0f);  //聚光的切角和外切角
//
//--------------------------------------------------------------------------//


//----------------工厂场景------------------------------>>>>>>>>>>>>>>>>>>>>>>>>>>
//定向光的4个数据
//static glm::vec3 dirLightArgs[4] = {
//    glm::vec3(0.05f, 0.05f, 0.1f),  //ambient
//    glm::vec3(0.2f, 0.2f, 0.7f),     //diffuse
//    glm::vec3(0.7f, 0.7f, 0.7f),     //specular
//    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
//};
//
////点光源的6个数据
//glm::vec3 pointLightColors[] = {
//    glm::vec3(0.2f, 0.2f, 0.6f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
//    glm::vec3(0.3f, 0.3f, 0.7f),  //第二个点光源的 (ambient, diffuse, specular)  红色
//    glm::vec3(0.0f, 0.0f, 0.3f),   //第三个点光源的 (ambient, diffuse, specular)  黄色
//    glm::vec3(0.4f, 0.4f, 0.4f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色
//
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
////聚光的6个数据
//static glm::vec3 spotlightArgs[] = {
//    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
//    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
//    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
//    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
//};
//
//static glm::vec3 clearColor(0.1f, 0.1f, 0.1f);   //背景颜色
//
//static glm::vec2 spotlight_cutoff(10.0f, 12.5f);  //聚光的切角和外切角

//---------------------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<


//----------------恐怖场景------------------------------------------>>>>>>>>>>>>
//定向光的4个数据
static glm::vec3 dirLightArgs[4] = {
    glm::vec3(0.0f, 0.0f, 0.0f),  //ambient
    glm::vec3(0.05f, 0.05f, 0.05),     //diffuse
    glm::vec3(0.2f, 0.2f, 0.2f),     //specular
    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
};

//点光源的6个数据
glm::vec3 pointLightColors[] = {
    glm::vec3(0.1f, 0.1f, 0.1f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
    glm::vec3(0.1f, 0.1f, 0.1f),  //第二个点光源的 (ambient, diffuse, specular)  红色
    glm::vec3(0.1f, 0.1f, 0.1f),   //第三个点光源的 (ambient, diffuse, specular)  黄色
    glm::vec3(0.3f, 0.1f, 0.1f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色

    glm::vec3(1.0f, 0.14f, 0.07f),   //constant,linear, quadratic
};

//聚光的6个数据
static glm::vec3 spotlightArgs[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
    glm::vec3(1.0f, 0.09f, 0.032f),   //constant,linear, quadratic
};

static glm::vec3 clearColor(0.0f, 0.0f, 0.0f);   //背景颜色

static glm::vec2 spotlight_cutoff(10.0f, 15.0f);  //聚光的切角和外切角

//-----------------------------------------------------------------<<<<<<<<<<<<<<




//----------------化学实验室场景------------------------------------------>>>>>>>>>>>>
//定向光的4个数据
//static glm::vec3 dirLightArgs[4] = {
//    glm::vec3(0.5f, 0.5f, 0.5f),  //ambient
//    glm::vec3(1.0f, 1.0f, 1.0f),     //diffuse
//    glm::vec3(1.0f, 1.0f, 1.0f),     //specular
//    glm::vec3(-0.2f, -1.0f, -0.3f)   //direction
//};
//
////点光源的6个数据
//glm::vec3 pointLightColors[] = {
//    glm::vec3(0.4f, 0.7f, 0.1f),  //第一个点光源的 (ambient, diffuse, specular)  淡黄色
//    glm::vec3(0.4f, 0.7f, 0.1f),  //第二个点光源的 (ambient, diffuse, specular)  红色
//    glm::vec3(0.4f, 0.7f, 0.1f),   //第三个点光源的 (ambient, diffuse, specular)  黄色
//    glm::vec3(0.4f, 0.7f, 0.1f),  //第四个点光源的 (ambient, diffuse, specular)  蓝色
//
//    glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
//};
//
////聚光的6个数据
//static glm::vec3 spotlightArgs[] = {
//    glm::vec3(0.0f, 0.0f, 0.0f),     //ambient
//    glm::vec3(0.0f, 1.0f, 0.0f),     //diffuse
//    glm::vec3(0.0f, 1.0f, 0.0f),     //specular
//    glm::vec3(1.0f, 0.07f, 0.017f),   //constant,linear, quadratic
//};
//
//static glm::vec3 clearColor(0.9f, 0.9f, 0.9f);   //背景颜色
//
//static glm::vec2 spotlight_cutoff(7.0f, 10.0f);  //聚光的切角和外切角

//-----------------------------------------------------------------<<<<<<<<<<<<<<


static bool prepare(const char* projectDir);

static void render();

static void processInput(GLFWwindow* window);

static void mouse_move_callback(GLFWwindow* window, double posX, double posY);

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

/// <summary>
/// 多光源
/// 我们将结合之前学过的所有知识，创建一个包含六个光源的场景。我们将模拟一个类似太阳的定向光(Directional Light)光源，
/// 四个分散在场景中的点光源(Point Light)，以及一个手电筒(Flashlight)。
/// 为了在场景中使用多个光源，我们希望将光照计算封装到GLSL函数中。
/// 这样做的原因是，每一种光源都需要一种不同的计算方法，而一旦我们想对多个光源进行光照计算时，代码很快就会变得非常复杂。
/// 如果我们只在main函数中进行所有的这些计算，代码很快就会变得难以理解。
/// </summary>
/// <param name="projectDir">当前工程所在目录</param>
/// <returns>1：成功；-1：失败</returns>
int practiseLight26(const char* projectDir) {
    if (!getChildPath(&vertextFilePath, projectDir, vertextFile)) { return -1; }
    if (!getChildPath(&fragFilePath, projectDir, fragFile)) { return -1; }
    if (!getChildPath(&lightVertFilePath, projectDir, lightVertFile)) { return -1; }
    if (!getChildPath(&lightFragFilePath, projectDir, lightFragFile)) { return -1; }

    GLFWwindow* window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw boxes with spot light.");
    if (window == NULL) { return -1; }

    if (!createShaderProgram(vertextFilePath, fragFilePath, &shaderId)) { return -1; }
    if (!createShaderProgram(lightVertFilePath, lightFragFilePath, &shaderLightId)) { return -1; }
    std::cout << "shaderId = " << shaderId << std::endl;
    std::cout << "shaderLightId = " << shaderLightId << std::endl;

    if (!prepare(projectDir)) { return -1; }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);

    while (!glfwWindowShouldClose(window)) {
        double curFrame = glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;

        processInput(window);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    free(vertextFilePath);
    free(fragFilePath);
    free(lightVertFilePath);
    free(lightFragFilePath);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &lightVBO);

    glDeleteProgram(shaderId);
    glDeleteProgram(shaderLightId);

    return 1;
}

bool prepare(const char* projectDir) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * (sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * (sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * (sizeof(float)), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    texture1 = textureLoad(projectDir, imageContainer2File);
    if (texture1 < 0) { return false; }
    texture2 = textureLoad(projectDir, imageContainer2SpecularFile);
    if (texture2 < 0) { return false; }

    glUseProgram(shaderId);
    glUniform1i(glGetUniformLocation(shaderId, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderId, "material.specular"), 1);

    //------------------------------------------------------------------------------------------------------

    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}

void render() {
    glUseProgram(shaderId);
    glBindVertexArray(VAO);
    
    glm::mat4 view(1.0);
    glm::mat4 projection(1.0);

    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom),
        SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

    //定向光
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.ambient"), dirLightArgs[0].x, dirLightArgs[0].y, dirLightArgs[0].z);
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.diffuse"), dirLightArgs[1].x, dirLightArgs[1].y, dirLightArgs[1].z);
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.specular"), dirLightArgs[2].x, dirLightArgs[2].y, dirLightArgs[2].z);
    //glUniform3f(glGetUniformLocation(shaderId, "dirlight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.direction"), dirLightArgs[3].x, dirLightArgs[3].y, dirLightArgs[3].z);

    //4个点光源
    //------------------------1
    for (int i = 0; i < 4; i++) {
        char ambient[23]{ 0 };
        char diffuse[23]{ 0 };
        char specular[24]{ 0 };
             
        char position[24]{ 0 };
        char constant[24]{ 0 };
        char linear[22]{ 0 };
        char quadratic[25]{ 0 };

        sprintf_s(ambient, 23, "pointlights[%d].ambient", i);
        sprintf_s(diffuse, 23, "pointlights[%d].diffuse", i);
        sprintf_s(specular, 24,"pointlights[%d].specular", i);
               
        sprintf_s(position,24, "pointlights[%d].position", i);
               
        sprintf_s(constant,24, "pointlights[%d].constant", i);
        sprintf_s(linear, 22, "pointlights[%d].linear", i);
        sprintf_s(quadratic, 25, "pointlights[%d].quadratic", i);
        glUniform3f(glGetUniformLocation(shaderId, position), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

        glUniform3f(glGetUniformLocation(shaderId, ambient),  pointLightColors[i].x * 0.1f, pointLightColors[i].y * 0.1f, pointLightColors[i].z * 0.1f);
        glUniform3f(glGetUniformLocation(shaderId, diffuse),  pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);
        glUniform3f(glGetUniformLocation(shaderId, specular), pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);

        glUniform1f(glGetUniformLocation(shaderId, constant), pointLightColors[4].x);
        glUniform1f(glGetUniformLocation(shaderId, linear), pointLightColors[4].y);
        glUniform1f(glGetUniformLocation(shaderId, quadratic), pointLightColors[4].z);
    }

    //1个聚光
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.ambient"), spotlightArgs[0].x, spotlightArgs[0].y, spotlightArgs[0].z);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.diffuse"), spotlightArgs[1].x, spotlightArgs[1].y, spotlightArgs[1].z);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.specular"), spotlightArgs[2].x, spotlightArgs[2].y, spotlightArgs[2].z);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.position"),
        camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.direction"),
        camera.Front.x, camera.Front.y, camera.Front.z);
    glUniform1f(glGetUniformLocation(shaderId, "spotlight.cutoff"), glm::cos(glm::radians(spotlight_cutoff.x)));
    glUniform1f(glGetUniformLocation(shaderId, "spotlight.outCutoff"), glm::cos(glm::radians(spotlight_cutoff.y)));

    glUniform1f(glGetUniformLocation(shaderId, "spotlight.constant"), spotlightArgs[3].x);
    glUniform1f(glGetUniformLocation(shaderId, "spotlight.linear"), spotlightArgs[3].y);
    glUniform1f(glGetUniformLocation(shaderId, "spotlight.quadratic"), spotlightArgs[3].z);


    //世界坐标矩阵
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //视角位置
    glUniform3f(glGetUniformLocation(shaderId, "viewPos"), 
        camera.Position.x, camera.Position.y, camera.Position.z);
    //材质高光系数    
    glUniform1f(glGetUniformLocation(shaderId, "material.shininess"), 32.0f);
    textureActiveAndBind(texture1, 0);
    textureActiveAndBind(texture2, 1);

    for (size_t i = 0; i < 10; i++) {
        glm::mat4 model(1.0);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, ((float)glfwGetTime()) * glm::radians(i * 10.0f), glm::vec3(0.0f, 0.2f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //------------------------------------------------------------------------------------------------------

    glBindVertexArray(lightVAO);
    glUseProgram(shaderLightId);

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    for (int i = 0; i < 4; i++) {
        glm::mat4 model(1.0);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.1));
        glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(glGetUniformLocation(shaderLightId, "lightColor"), 
            pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
}

void mouse_move_callback(GLFWwindow* window, double posX, double posY) {
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

void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
    camera.ProcessMouseScroll(offsetY);
}

