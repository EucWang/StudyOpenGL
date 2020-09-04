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
static const char* lightFragFile = "shader/shader_light_test25_light.frag";

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.specular"), 1.0f, 1.0f, 1.0f);
    //glUniform3f(glGetUniformLocation(shaderId, "dirlight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(shaderId, "dirlight.direction"), -0.2f, 1.0f, -0.3f);

    //4个点光源
    //------------------------1
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[0].ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[0].diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[0].specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[0].position"),
        pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);

    glUniform1f(glGetUniformLocation(shaderId, "pointlights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[0].linear"), 0.045f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[0].quadratic"), 0.0075f);
    //------------------------2
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[1].ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[1].diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[1].specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[1].position"),
        pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);

    glUniform1f(glGetUniformLocation(shaderId, "pointlights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[1].linear"), 0.045f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[1].quadratic"), 0.0075f);
    //------------------------3
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[2].ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[2].diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[2].specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[2].position"),
        pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);

    glUniform1f(glGetUniformLocation(shaderId, "pointlights[2].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[2].linear"), 0.045f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[2].quadratic"), 0.0075f);
    //------------------------4
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[3].ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[3].diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[3].specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "pointlights[3].position"),
        pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);

    glUniform1f(glGetUniformLocation(shaderId, "pointlights[3].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[3].linear"), 0.045f);
    glUniform1f(glGetUniformLocation(shaderId, "pointlights[3].quadratic"), 0.0075f);

    //1个聚光
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.position"),
        camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(shaderId, "spotlight.direction"),
        camera.Front.x, camera.Front.y, camera.Front.z);
    glUniform1f(glGetUniformLocation(shaderId, "spotlight.cutoff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(shaderId, "spotlight.outCutoff"), glm::cos(glm::radians(17.5f)));

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
        model = glm::rotate(model, ((float)glfwGetTime()) * glm::radians(i * 10.0f + 10.0f), glm::vec3(0.0f, 0.2f, 0.0f));

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
        glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));

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

