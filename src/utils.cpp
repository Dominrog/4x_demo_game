#include "utils.h"


Planet* lockedPlanet = nullptr;
bool firstMouse = true;
bool cameraLockedOnObject = false;
double mouseX, mouseY;
Planet* hoveredPlanet = nullptr;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera camera)
{
    static bool leftMousePressedLastFrame = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float rotationSpeed = 80.0f; // Grad pro Sekunde
    float moveSpeed = 200.0f;    // Einheiten pro Sekunde

    // Kamera drehen
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.Yaw += rotationSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.Yaw -= rotationSpeed * deltaTime;

    // flache Front (nur X/Z)
    glm::vec3 flatFront = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
    glm::vec3 right = glm::normalize(glm::cross(flatFront, camera.WorldUp));

    // Target verschieben relativ zur Kamera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Target += flatFront * moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Target -= flatFront * moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Target -= right * moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Target += right * moveSpeed * deltaTime;

    // Kamera-Position neu berechnen
    camera.UpdatePosition();

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraLockedOnObject = false; // Spieler übernimmt die Kontrolle
        lockedPlanet = nullptr;
    }

    bool leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (leftPressed && !leftMousePressedLastFrame)
    {
        // Hier Linksklick-Logik, z.B. prüfen auf hoveredPlanet
        if (hoveredPlanet)
        {
            lockedPlanet = hoveredPlanet;
            camera.Target = hoveredPlanet->getCenter();
            camera.ProcessMouseScroll(30.0f);
            cameraLockedOnObject = true;
        }
    }

    if (cameraLockedOnObject && lockedPlanet)
    {
        camera.Target = lockedPlanet->getCenter();
    }

    leftMousePressedLastFrame = leftPressed;
}