#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "planet.h"

extern Planet* lockedPlanet;
extern bool firstMouse;
extern bool cameraLockedOnObject;
extern double mouseX, mouseY;
extern Planet* hoveredPlanet;

extern float deltaTime;
extern float lastFrame;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Camera camera);