#include "utils.h"


Planet* lockedPlanet = nullptr;
bool firstMouse = true;
bool cameraLockedOnObject = false;
double mouseX, mouseY;
Planet* hoveredPlanet = nullptr;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera& camera, float deltaTime)
{
    static bool leftMousePressedLastFrame = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float rotationSpeed = 80.0f;
    float moveSpeed = 200.0f;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.Yaw += rotationSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.Yaw -= rotationSpeed * deltaTime;

    glm::vec3 flatFront = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
    glm::vec3 right = glm::normalize(glm::cross(flatFront, camera.WorldUp));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Target += flatFront * moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Target -= flatFront * moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Target -= right * moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Target += right * moveSpeed * deltaTime;

    camera.UpdatePosition();

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraLockedOnObject = false;
        lockedPlanet = nullptr;
    }

    bool leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (leftPressed && !leftMousePressedLastFrame)
    {
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (camera)
        camera->ProcessMouseScroll((float)yoffset);
}

void countFPS()
{
  static double lastTime = glfwGetTime();
  static int frameCount = 0;

  double currentTime = glfwGetTime();
  frameCount++;

  if (currentTime - lastTime >= 1.0)
  {
    std::cout << "FPS: " << frameCount << std::endl;
    frameCount = 0;
    lastTime = currentTime;
  }
}

Ray getMouseRay(GLFWwindow* window, Camera& camera, glm::mat4 projection, glm::mat4 view)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float x = (2.0f * mouseX) / SCR_WIDTH - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / SCR_HEIGHT;

    glm::vec4 ray_clip(x, y, -1.0f, 1.0f);
    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));

    return Ray{camera.Position, ray_world};
}

bool rayIntersectsAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax)
{
    float tMin = (boxMin.x - ray.origin.x) / ray.dir.x;
    float tMax = (boxMax.x - ray.origin.x) / ray.dir.x;
    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (boxMin.y - ray.origin.y) / ray.dir.y;
    float tyMax = (boxMax.y - ray.origin.y) / ray.dir.y;
    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (boxMin.z - ray.origin.z) / ray.dir.z;
    float tzMax = (boxMax.z - ray.origin.z) / ray.dir.z;
    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

std::vector<glm::vec3> createCircleRing(float radius, int segments)
{
    std::vector<glm::vec3> vertices;
    float delta = 2.0f * glm::pi<float>() / segments;

    for(int i = 0; i < segments; ++i)
    {
        float angle1 = i * delta;
        float angle2 = (i + 1) * delta;

        vertices.push_back(glm::vec3(radius * cos(angle1), 0.0f, radius * sin(angle1)));
        vertices.push_back(glm::vec3(radius * cos(angle2), 0.0f, radius * sin(angle2)));
    }

    return vertices;
}