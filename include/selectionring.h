#ifndef SELECTIONRING_H
#define SELECTIONRING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "utils.h"
#include "camera.h"

class SelectionRing
{
private:
	unsigned int VAO, VBO;
public:
	SelectionRing();
	~SelectionRing();

	void initSelectionRing();
	void renderSelectionRing(Shader& shader, Camera& camera, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void destroy();
};

#endif