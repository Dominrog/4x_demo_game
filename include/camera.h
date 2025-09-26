#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 dir;
};


class Camera {
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  glm::vec3 Target; 

  float MinDistance;
  float MaxDistance;
  float MinPitch; 
  float MaxPitch; 

  float Yaw;
  float Pitch;
  float Distance;

  bool followTarget;      // ob die Kamera einem Ziel folgt
  glm::vec3 followPosition;       // das Ziel, dem gefolgt wird
  float followSpeed;  

  Camera(glm::vec3 target = glm::vec3(0.0f),
         float distance = 400.0f,
         float yaw = 0.0f,
         float pitch = -25.0f);

  glm::mat4 GetViewMatrix();
  void UpdatePosition(); 
  void ProcessMouseScroll(float yoffset);
  void SetFollowTarget(const glm::vec3& target);
};

#endif
