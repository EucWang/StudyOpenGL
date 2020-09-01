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

static const char* vertextFile = "shader/shader_light_test24.vert";
static const char* fragFile = "shader/shader_light_test24.frag";
static const char* lightVertFile = "shader/shader_light_test24_light.vert";
static const char* lightFragFile = "shader/shader_light_test24_light.frag";

static const char* imageContainer2File = "images/container2.png";
static const char* imageContainer2SpecularFile = "images/container2_specular.png";
//static const char* imageContainer2ColorSpecularFile = "images/lighting_maps_specular_color.png";
//static const char* imageContainer2MatrixSpecularFile = "images/matrix.jpg";

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
//static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
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

static bool prepare(const char* projectDir);

static void render();

static void processInput(GLFWwindow* window);

static void mouse_move_callback(GLFWwindow* window, double posX, double posY);

static void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

int practiseLight24(const char * projectDir) {

    if (!getChildPath(&vertextFilePath, projectDir, vertextFile)) { return -1; }
    if (!getChildPath(&fragFilePath, projectDir,fragFile)) { return -1; }
    if (!getChildPath(&lightVertFilePath, projectDir, lightVertFile)) { return -1; }
    if (!getChildPath(&lightFragFilePath, projectDir, lightFragFile)) { return -1; }

   GLFWwindow * window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw boxes with dot light.");
   if (window == NULL)   {       return -1;   }

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
       deltaTime =curFrame - lastFrame;
       lastFrame = curFrame;

       processInput(window);
       glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
   glDeleteBuffers(1, &VBO);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)( 3 *sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)( 6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    texture1 = textureLoad(projectDir, imageContainer2File);
    std::cout << "texture1 = " << texture1 << std::endl;
    if(texture1 < 0){ return false; }

    texture2 = textureLoad(projectDir, imageContainer2SpecularFile);
    std::cout << "texture2 = " << texture2 << std::endl;
    if (texture2 < 0) { return false; }

    glUseProgram(shaderId);
    glUniform1i(glGetUniformLocation(shaderId, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderId, "material.specular"), 1);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}

void render() {
    glUseProgram(shaderId);
    glBindVertexArray(VAO);

    textureActiveAndBind(texture1, 0);
    textureActiveAndBind(texture2, 1);

    glm::mat4 view(1.0);
    glm::mat4 projection(1.0);

    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), 
        SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT, 0.1f, 100.0f);

    glUniform3f(glGetUniformLocation(shaderId, "light.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderId, "light.diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderId, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "light.position"), lightPos.x, lightPos.y, lightPos.z);

    glUniform1f(glGetUniformLocation(shaderId, "material.shininess"), 32.0f);
    glUniform3f(glGetUniformLocation(shaderId, "viewPos"), 
        camera.Position.x, camera.Position.y, camera.Position.z);

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"),
        1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));

    for (size_t i = 0; i < 10; i++) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePositions[0]);
        model = glm::rotate(model, 
            ((float)glfwGetTime()) * glm::radians(1 * 20.0f + 10.0f), 
            glm::vec3(0.0f, 0.1f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    ///////////////////// ///////////////////

    glUseProgram(shaderLightId);
    glBindVertexArray(lightVAO);

    glm::mat4 model2(1.0f);
    model2 = glm::translate(model2, lightPos);
    model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "model"),
        1, GL_FALSE, glm::value_ptr(model2));

    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "view"),
        1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightId, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));

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
    if (isMouseFirstIn)
    {
        isMouseFirstIn = false;
        lastX = posX;
        lastY = posY;
    }

    double offsetX = posX - lastX;
    double offsetY =  lastY - posY;

    lastX = posX;
    lastY = posY;

   camera.ProcessMouseMovement(offsetX, offsetY);
}

void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
    camera.ProcessMouseScroll(offsetY);
}