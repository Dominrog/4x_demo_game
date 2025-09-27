#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "camera.h"
#include "planet.h"
#include "settings.h"

extern Planet* lockedPlanet;
extern bool firstMouse;
extern bool cameraLockedOnObject;
extern double mouseX, mouseY;
extern Planet* hoveredPlanet;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Camera& camera, float deltaTime);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void countFPS();
bool rayIntersectsAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax);
std::vector<glm::vec3> createCircleRing(float radius, int segments);
Ray getMouseRay(GLFWwindow* window, Camera& camera, glm::mat4 projection, glm::mat4 view);
