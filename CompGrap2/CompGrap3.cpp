#define GLEW_DLL
#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.h"
#include <iostream>

// Параметры камеры
glm::vec3 cameraPos = glm::vec3(0.0f, -1.5f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Углы Эйлера
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 512.0f / 2.0f;
float lastY = 512.0f / 2.0f;
bool firstMouse = true;
float sensitivity = 0.03f;
float cameraSpeed = 0.001f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


float OX1 = 0.0f;
float OX2 = 0.0f;
float OX3 = 0.0f;

float OX_Z = 0.0f; 
float OX_X = 0.0f; 
float OX_Y = 0.0f;


float OX1_X = 0.0f, OX1_Y = 0.0f, OX1_Z = 0.0f;
glm::mat4 translateOX1 = glm::mat4(1.0f);


float OX2_Y = 0.0f; 
glm::mat4 translateOX2 = glm::mat4(1.0f);


float OX3_X = 0.0f, OX3_Z = 0.0f;
glm::mat4 translateOX3 = glm::mat4(1.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 0.001f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

   
    float animationSpeed = 3.0f * deltaTime;

    // Движение Платформы со Столом (вперед назад) Кнопки N M
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        OX1_Z -= animationSpeed;
        OX1_Z = glm::clamp(OX1_Z, -0.11f, 0.06f);
        translateOX1 = glm::translate(glm::mat4(1.0f), glm::vec3(OX1_X, 0, OX1_Z));

        OX3_Z -= animationSpeed;
        OX3_Z = glm::clamp(OX3_Z, -0.11f, 0.06f);
        translateOX3 = glm::translate(glm::mat4(1.0f), glm::vec3(OX3_X, 0, OX1_Z));

    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        OX1_Z += animationSpeed;
        OX1_Z = glm::clamp(OX1_Z, -0.11f, 0.06f);
        translateOX1 = glm::translate(glm::mat4(1.0f), glm::vec3(OX1_X, 0, OX1_Z));

        OX3_Z == animationSpeed;
        OX3_Z = glm::clamp(OX3_Z, -0.11f, 0.06f);
        translateOX3 = glm::translate(glm::mat4(1.0f), glm::vec3(OX3_X, 0, OX1_Z));

    }
    

    // Движение Головы (вверх и вниз) Кнопик I O
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        OX2_Y += animationSpeed;
        OX2_Y = glm::clamp(OX2_Y, -0.4875f, 0.0f);
        translateOX2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, OX2_Y, 0));
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        OX2_Y -= animationSpeed;
        OX2_Y = glm::clamp(OX2_Y, -0.24f, 0.0f);
        translateOX2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, OX2_Y, 0));
    }

    // Движение Стола (влево вправо) Кнопки J K
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        OX3_X -= animationSpeed;
        OX3_X = glm::clamp(OX3_X, -0.25f, 0.27f);
        translateOX3 = glm::translate(glm::mat4(1.0f), glm::vec3(OX3_X, 0, OX1_Z));
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        OX3_X += animationSpeed;
        OX3_X = glm::clamp(OX3_X, -0.25f, 0.27f);
        translateOX3 = glm::translate(glm::mat4(1.0f), glm::vec3(OX3_X, 0, OX1_Z));
    }

}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Zauzolkov Vladislav ASUb-23-2", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    ShaderLoader shader("vertex_shader.glsl", "fragment_shader.glsl");


    Model ourModel("model.obj");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 1.0f, 1.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 baseModel = glm::mat4(1.0f);
        baseModel = glm::translate(baseModel, glm::vec3(0.0f, -1.75f, 0.0f));
        baseModel = glm::scale(baseModel, glm::vec3(0.2f, 0.2f, 0.2f));

       
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(baseModel)));
        shader.SetUniformMatrix3fv("normalMatrix", glm::value_ptr(normalMatrix));
        shader.SetUniformMatrix4fv("projection", projection);
        shader.SetUniformMatrix4fv("view", view);

       

        // Настройка материал
        shader.SetUniform3f("material.ambient", 0.24725f, 0.1995f, 0.0745f);
        shader.SetUniform3f("material.diffuse", 0.75164f, 0.60648f, 0.22648f);
        shader.SetUniform3f("material.specular", 0.628281f, 0.555802f, 0.366065f);
        shader.SetUniform1f("material.shininess", 51.2f);

        // Настройка источника света
        shader.SetUniform3f("light_1.Position", 1.2f, 1.0f, 2.0f);
        shader.SetUniform3f("light_1.ambient", 0.2f, 0.2f, 0.2f);
        shader.SetUniform3f("light_1.diffuse", 0.5f, 0.5f, 0.5f);
        shader.SetUniform3f("light_1.specular", 1.0f, 1.0f, 1.0f);

        // Позиция камерв
        shader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

        ourModel.Draw(shader,
            translateOX1, 
            translateOX2, 
            translateOX3, 
            baseModel
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}