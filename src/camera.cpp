#include "camera.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 target, float distance, float yaw, float pitch)
    : Target(target),
      Distance(distance), 
      Yaw(yaw), 
      Pitch(pitch),
      WorldUp(0.0f, 1.0f, 0.0f),
      MinDistance(100.0f),
      MaxDistance(600.0f),
      MinPitch(-5.0f),
      MaxPitch(-35.0f)
{
    UpdatePosition();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Target, Up);
}

void Camera::UpdatePosition()
{
    float x = Distance * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    float y = -Distance * sin(glm::radians(Pitch));
    float z = Distance * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));

    Position = Target + glm::vec3(x, y, z);
    Front = glm::normalize(Target - Position);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessMouseScroll(float yoffset)
{
  Distance -= yoffset * 20.0f; // Skalierung des Zooms
  if(Distance < MinDistance) Distance = MinDistance;
  if(Distance > MaxDistance) Distance = MaxDistance;

  // Pitch proportional zum Abstand anpassen
  float t = (Distance - MinDistance) / (MaxDistance - MinDistance);
  //t = t*t;
  Pitch = MinPitch + t * (MaxPitch - MinPitch);

  UpdatePosition();
}