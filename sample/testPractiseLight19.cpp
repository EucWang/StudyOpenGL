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

static const char* vertextFile = "shader_light_test19_vertex.txt";
static const char* fragFile = "shader_light_test19_fragment.txt";
static const char* lightFragFile = "shader_light_test19_fragment_light.txt";
static char* vertextFilePath;
static char* fragFilePath;
static char* lightFragFilePath;

static const char* shaderDirName = "shader";
static char* shaderDir;

static GLuint VAO, VBO;
static int shaderId, shaderLightId;
static GLuint lightVAO;

static Camera camera(glm::vec3(0.0f, 1.5f, 3.0f));

static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

//表示光源在场景的世界空间坐标中的位置
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

static float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

static void prepare();

static void render();

static void processInput(GLFWwindow * window);

static void mouse_move_callback(GLFWwindow * window, double posX, double posY);

static void mouse_scroll_callback(GLFWwindow * window, double offsetX, double offsetY);

int practiseLight19(const char * projectDir) {
    
    if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }

    GLFWwindow * window =  createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with environment light.");
    if (window == NULL) { return -1; }

    if (!getChildPath(&vertextFilePath, shaderDir, vertextFile)) { return -1; }
    if (!getChildPath(&fragFilePath, shaderDir, fragFile)) { return -1; }
    if (!getChildPath(&lightFragFilePath, shaderDir, lightFragFile)) { return -1; }

    if (!createShaderProgram(vertextFilePath, fragFilePath, &shaderId)) { return -1; }
    if (!createShaderProgram(vertextFilePath, lightFragFilePath, &shaderLightId)) { return -1; }

    prepare();

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    glDeleteProgram(shaderId);
    glDeleteProgram(shaderLightId);

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    free(shaderDir);
    free(vertextFilePath);
    free(fragFilePath);
    free(lightFragFilePath);

    return 1;
}

void processInput(GLFWwindow* window) {
    double curTime = glfwGetTime();
    deltaTime = curTime - lastFrame;
    lastFrame = curTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)    {
        glfwSetWindowShouldClose(window, true);
    }    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
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


void prepare() {
    //加载木箱的数据
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //加载光源的数据
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void render() {
    //画木箱
    glBindVertexArray(VAO);
    glUseProgram(shaderId);

    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 projection = glm::mat4(1.0);

    //model = glm::rotate(model,  glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model,  getFVal() *glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = camera.GetViewMatrix();
    float aspect = SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT;
    projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(shaderId, "objectColor"), 1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shaderId, "lightColor"), 1.0f, 1.0f, 1.0f);
    //设置光源的位置
    glUniform3f(glGetUniformLocation(shaderId, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    //设置观察者的位置
    glUniform3f(glGetUniformLocation(shaderId, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //画光源
    glBindVertexArray(lightVAO);
    glUseProgram(shaderLightId);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, lightPos);
    model2 = glm::scale(model2, glm::vec3(0.1f));

    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "model"), 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);
}