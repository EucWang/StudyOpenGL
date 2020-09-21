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

static const char* shaderDirName = "shader";
static char* shaderDir;

static const char* vertextFile = "shader_light_test21.vert";
static const char* fragFile = "shader_light_test21_2.frag";
static const char* lightFragFile = "shader_light_test21_light.frag";

static char* vertextFilePath;
static char* fragFilePath;
static char* lightFragFilePath;

static GLuint VAO, VBO, lightVAO;
static int shaderId, shaderLightId;

static double deltaTime;
static double lastFrame;
static double lastX = SMALL_SCREEN_WIDTH / 2, lastY = SMALL_SCREEN_HEIGHT / 2;
static bool isMouseFirstIn = true;

static Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
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

int practiseLight21(const char * projectDir) {
    if (!getChildPath(&shaderDir, projectDir, shaderDirName)) { return -1; }

    GLFWwindow* window = createGLWindow(SMALL_SCREEN_WIDTH, SMALL_SCREEN_HEIGHT, "Draw with light and Materials");
    if (window == NULL)  {  return -1;  }

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
        double curTime = glfwGetTime();
        deltaTime = curTime - lastFrame;
        lastFrame = curTime;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderId);
    glDeleteProgram(shaderLightId);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    free(shaderDir);
    free(vertextFilePath);
    free(fragFilePath);
    free(lightFragFilePath);
    return 1;
}

void prepare() {
    //木箱的数据
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

     //光照数据
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void render() {
    //加载木箱
    glUseProgram(shaderId);
    glBindVertexArray(VAO);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
    model = glm::rotate(model, ((float)glfwGetTime()) * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view = camera.GetViewMatrix();

    float aspect = SMALL_SCREEN_WIDTH * 1.0f / SMALL_SCREEN_HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
    //设置坐标变换
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    //设置视角（相机）位置
    glUniform3f(glGetUniformLocation(shaderId, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    //设置箱子的物体材质
    glUniform3f(glGetUniformLocation(shaderId, "material.ambient"),  0.0f, 0.1f, 0.06f);
    glUniform3f(glGetUniformLocation(shaderId, "material.diffuse"),  0.0f, 0.50980392f, 0.50980392f);
    glUniform3f(glGetUniformLocation(shaderId, "material.specular"), 0.50196078f, 0.50196078f, 0.50196078f);
    glUniform1f(glGetUniformLocation(shaderId, "material.shininess"),  32.0f);
    //设置光照的光亮材质
    glm::vec3 lightColor;
    lightColor.x = sin((float)glfwGetTime()) * 2.0f;
    lightColor.y = sin((float)glfwGetTime()) * 0.7f;
    lightColor.z = sin((float)glfwGetTime()) * 1.3f;
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);  // 降低影响
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  // 降低影响

    //glUniform3f(glGetUniformLocation(shaderId, "light.ambient"), 0.2f, 0.2f, 0.2f);
    //glUniform3f(glGetUniformLocation(shaderId, "light.diffuse"), 0.5f, 0.5f, 0.5f);
    //glUniform3f(glGetUniformLocation(shaderId, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    //glUniform3f(glGetUniformLocation(shaderId, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform3f(glGetUniformLocation(shaderId, "light.ambient"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "light.diffuse"), 1.0f, 1.0f, 1.0f);

    glUniform3f(glGetUniformLocation(shaderId, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderId, "light.position"), lightPos.x, lightPos.y, lightPos.z);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //画第二个箱子， 用不同的材质
   // green plastic  	Ambient(0.0, 	0.0,	0.0)	Diffuse(0.1,	0.35,	0.1)	Specular(0.45,	0.55,	0.45)	 Shininess(25)

    glm::mat4 model4(1.0f);
    model4 = glm::translate(model4, glm::vec3(2.0f, 1.0f, -1.0f));
    model4 = glm::rotate(model4, ((float)glfwGetTime()) * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model4));
    //设置箱子的物体材质
    glUniform3f(glGetUniformLocation(shaderId, "material.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(shaderId, "material.diffuse"), 0.1f, 0.35f, 0.1f);
    glUniform3f(glGetUniformLocation(shaderId, "material.specular"), 0.45f, 0.55f, 0.45f);
    glUniform1f(glGetUniformLocation(shaderId, "material.shininess"), 25.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);


    //画个地板
    glm::mat4 model3(1.0f);
    model3 = glm::scale(model3, glm::vec3(100.0f, 0.01f, 100.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model3));
    //设置地板的材质
    glUniform3f(glGetUniformLocation(shaderId, "material.ambient"), 0.1f, 0.1f, 0.1f);
    glUniform3f(glGetUniformLocation(shaderId, "material.diffuse"), 0.3f, 0.3f, 0.3f);
    glUniform3f(glGetUniformLocation(shaderId, "material.specular"), 0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(shaderId, "material.shininess"), 32.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //加载光源
    glUseProgram(shaderLightId);
    glBindVertexArray(lightVAO);

    glm::mat4 model2(1.0f);
    model2 = glm::translate(model2, lightPos);
    model2 = glm::scale(model2, glm::vec3(0.01f, 0.01f, 0.01f));

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
    camera.ProcessMouseScroll((float)offsetY);
}
