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

static const char* vertextFile = "shader/shader_light_test22.vert";
static const char* fragFile = "shader/shader_light_test22.frag";
static const char* lightVertFile = "shader/shader_light_test22_light.vert";
static const char* lightFragFile = "shader/shader_light_test22_light.frag";

static const char* imageContainer2File = "images/container2.png";
static const char* imageContainer2SpecularFile = "images/container2_specular.png";
static const char* imageContainer2ColorSpecularFile = "images/lighting_maps_specular_color.png";
static const char* imageContainer2MatrixSpecularFile = "images/matrix.jpg";

static char* vertextFilePath;
static char* fragFilePath;
static char* lightVertFilePath;
static char* lightFragFilePath;

static GLuint VAO, VBO, lightVAO;
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

static int prepare(const char * projectDir);

static void render();

static void processInput(GLFWwindow* window);

static void mouse_move_callback(GLFWwindow* window, double posX, double posY);

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

int practiseLight22(const char * projectDir) {
    if (!getChildPath(&vertextFilePath, projectDir, vertextFile)) { return -1; }
    if (!getChildPath(&fragFilePath, projectDir, fragFile)) { return -1; }
    if (!getChildPath(&lightFragFilePath, projectDir, lightFragFile)) { return -1; }
    if (!getChildPath(&lightVertFilePath, projectDir, lightVertFile)) { return -1; }

    GLFWwindow* window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "practise draw lighting maps");
    if (window == NULL)    { return -1; }

    if (!createShaderProgram(vertextFilePath, fragFilePath, &shaderId)) { return -1; }
    if (!createShaderProgram(lightVertFilePath, lightFragFilePath, &shaderLightId)) { return -1; }

    if (!prepare(projectDir)) { return -1; }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    while (!glfwWindowShouldClose(window)) {
        double curFrame = glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;
        
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

    free(vertextFilePath);
    free(fragFilePath);
    free(lightFragFilePath);

    return -1;
}


int prepare(const char* projectDir) {
    //箱子的数据
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    textureGenSet(&texture1);
    if (!textureLoadImg(projectDir, imageContainer2File, GL_RGBA)) { return -1; }
    //glUniform1i(glGetUniformLocation(shaderId, "material.diffuse"), 0);
    //使用漫反射贴图 代替箱子的原来的颜色贴图
    textureBind(shaderId, "material.diffuse", 0);

    textureGenSet(&texture2);
    if (!textureLoadImg(projectDir, imageContainer2SpecularFile, GL_RGBA)) { return -1; }
    //if (!textureLoadImg(projectDir, imageContainer2ColorSpecularFile, GL_RGBA)) { return -1; }
    //if (!textureLoadImg(projectDir, imageContainer2MatrixSpecularFile, GL_RGB)) { return -1; }
    //使用光照贴图 区分箱子镶边的金属光泽和箱子中间的木头的无光泽
    textureBind(shaderId, "material.specular", 1);

    //光照的数据
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return 1;
}

void render() {

    glUseProgram(shaderId);
    glBindVertexArray(VAO);
    textureUse(texture1, 0);
    textureUse(texture2, 1);

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE,  glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE,  glm::value_ptr(projection));
    
    glUniform3f(glGetUniformLocation(shaderId, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    //设置箱子的物体材质
    //glUniform3f(glGetUniformLocation(shaderId, "material.ambient"), 0.0f, 0.1f, 0.06f);
    //glUniform3f(glGetUniformLocation(shaderId, "material.diffuse"), 0.0f, 0.50980392f, 0.50980392f);
    //glUniform3f(glGetUniformLocation(shaderId, "material.specular"), 0.50196078f, 0.50196078f, 0.50196078f);

    glUniform1f(glGetUniformLocation(shaderId, "material.shininess"), 32.0f);

    glUniform3f(glGetUniformLocation(shaderId, "light.ambient"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "light.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "light.position"), lightPos.x, lightPos.y, lightPos.z);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //-----------------------------------------------------------------------------------------------------------
    glUseProgram(shaderLightId);
    glBindVertexArray(lightVAO);

    glm::mat4 model2(1.0f);
    model2 = glm::translate(model2, lightPos);
    model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);
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
