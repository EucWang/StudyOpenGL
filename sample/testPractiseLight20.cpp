#include "../include/fileUtil.h"
#include "../include/shaders.h"
#include "../include/shaderSource.h"
#include "../include/test.h"
#include "../include/util.h"
#include "../include/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

static const char* shaderDirName = "shader";
static char* shaderDir;

static const char* vertextFile = "shader_light_test20.vert";
static const char* fragFile = "shader_light_test20.frag";
static const char* lightFragFile = "shader_light_test20_light.frag";

static char* vertextFilePath;
static char* fragFilePath;
static char* lightFragFilePath;

static GLuint VAO, VBO, lightVAO;
static int shaderId, shaderLightId;

static double deltaTime;
static float lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//表示光源在场景的世界空间坐标中的位置
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
 
static float vertices[] = {
     -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,     -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,     -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,     -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,      1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,      1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,        1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,         0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,         0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,         0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,         0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,             0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,             0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,            0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,            0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,             0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,             0.0f,  1.0f,  0.0f
};

static void prepare();

static void render();

static void processInput(GLFWwindow* window);

static void mouse_move_callback(GLFWwindow* window, double posX, double posY);

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

int practiseLight20(const char* projectDir) {

    GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw box with light20");
    if (window == NULL) { return -1; }

    if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }
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
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteProgram(shaderId);
    glDeleteProgram(shaderLightId);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    free(shaderDir);
    free(vertextFilePath);
    free(fragFilePath);
    free(lightFragFilePath);
    return 1;
}

void prepare(){

    //导入箱子数据
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //导入光源数据
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void render(){
    //画箱子-------------------------------------------------------------------------------
    glUseProgram(shaderId);
    glBindVertexArray(VAO);

    //矩阵变换需要的3个Uniform变量
    glm::mat4 model(1.0);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians( 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 view = camera.GetViewMatrix();
    float aspect = SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, false, glm::value_ptr(projection));

    //光照需要的4个Uniform变量
    glUniform3f(glGetUniformLocation(shaderId, "lightPos"),  lightPos.x,  lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shaderId, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(shaderId, "objectColor"), 1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shaderId, "lightColor"), 1.0f, 1.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);   //画箱子

    glm::mat4 model4(1.0);
    model4 = glm::translate(model4, glm::vec3(5.0f, 2.0f, 0.0f));
    model4 = glm::scale(model4, glm::vec3(1.0f, 10.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, false, glm::value_ptr(model4));
    glDrawArrays(GL_TRIANGLES, 0, 36);   //画第二个箱子

    glm::mat4 model5(1.0);
    model5 = glm::translate(model5, glm::vec3(0.0f, 1.0f, 5.0f));
    model5 = glm::scale(model5, glm::vec3(1.0f, 5.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, false, glm::value_ptr(model5));
    glDrawArrays(GL_TRIANGLES, 0, 36);   //画第三个箱子

    //画地板---------------------------------------------------------------------------------------

    glm::mat4 model6(1.0);
    model6 = glm::translate(model6, glm::vec3(0.0f, -0.5f, 0.0f));
    model6 = glm::scale(model6, glm::vec3(50.0f, 0.001f, 50.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, false, glm::value_ptr(model6));
    glUniform3f(glGetUniformLocation(shaderId, "objectColor"), 0.5f, 0.5f, 0.5f);  //地板颜色
    glDrawArrays(GL_TRIANGLES, 0, 36);   // 画地板

    //画光源---------------------------------------------------------------------------------------
    glUseProgram(shaderLightId);
    glBindVertexArray(lightVAO);

    glm::mat4 model2(1.0); 
    model2= glm::translate(model2, lightPos);  //位移到光源位置
    model2 = glm::scale(model2, glm::vec3(0.1));  //缩小为箱子的10%大小

    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "model"), 1, false, glm::value_ptr(model2));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "projection"), 1, false, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);  //画光源
}

void processInput(GLFWwindow* window){
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

void mouse_move_callback(GLFWwindow* window, double posX, double posY){
    if (isMouseFirstIn)
    {
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

void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY){
    camera.ProcessMouseScroll(offsetY);
}
