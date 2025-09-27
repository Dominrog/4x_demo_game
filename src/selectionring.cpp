#include "selectionring.h"

SelectionRing::SelectionRing()
{
	VAO = 0;
	VBO = 0;
}

SelectionRing::~SelectionRing()
{
	
}

void SelectionRing::initSelectionRing()
{
	std::vector<glm::vec3> ringVertices = createCircleRing(1.0f, 64);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, ringVertices.size() * sizeof(glm::vec3), ringVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SelectionRing::renderSelectionRing(Shader& shader, Camera& camera, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	Planet* planetToHighlight = hoveredPlanet ? hoveredPlanet : (cameraLockedOnObject ? lockedPlanet : nullptr);

    if (planetToHighlight)
    {
        glm::vec3 planetPos = planetToHighlight->getCenter();
        glm::vec3 toCamera = glm::normalize(camera.Position - planetPos);

        glm::vec3 right = glm::normalize(glm::cross(toCamera, camera.WorldUp));
        glm::vec3 up    = glm::cross(right, toCamera);

        float r = planetToHighlight->getRadius() * 1.5f;
        int segments = 64;

        std::vector<glm::vec3> ringVerts;
        for(int i = 0; i < segments; ++i)
        {
            float angle1 = 2.0f * glm::pi<float>() * i / segments;
            float angle2 = 2.0f * glm::pi<float>() * (i + 1) / segments;

            glm::vec3 p1 = planetPos + right * (r * (float)cos(angle1)) + up * (r * (float)sin(angle1));
            glm::vec3 p2 = planetPos + right * (r * (float)cos(angle2)) + up * (r * (float)sin(angle2));

            ringVerts.push_back(p1);
            ringVerts.push_back(p2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, ringVerts.size() * sizeof(glm::vec3), ringVerts.data(), GL_DYNAMIC_DRAW);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lineColor", 1.0f, 1.0f, 1.0f);

        glLineWidth(3.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, (GLsizei)ringVerts.size());
        glBindVertexArray(0);
        glLineWidth(1.0f);
    }
}

void SelectionRing::destroy()
{
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
	}
}