#include "star.h"

Star::Star(float posX, float posY, float posZ)
	:	positionX(posX),
		positionY(posY),
		positionZ(posZ)
{
	auto planet = std::make_unique<Planet>(posX, posY, posZ, 64, 0.0f, 0.0f, 0.0f);
    planet->fillPlanet();
	planet->checkNeighbors();
	planet->buildMesh(cubeVertices, cubeIndices);
	planet->upload();

	star = std::move(planet);
}

Star::~Star()
{
	
}

void Star::addPlanet(float posX, float posY, float posZ, int size, float orbitalSpeed, float orbitalInclination, float orbitalPosition)
{
	auto planet = std::make_unique<Planet>(posX, posY, posZ, size, orbitalSpeed, orbitalInclination, orbitalPosition);
    planet->fillPlanet();
	planet->checkNeighbors();
	planet->buildMesh(cubeVertices, cubeIndices);
	planet->upload();

	planets.push_back(std::move(planet));

	uploadOrbit(posX);
}

void Star::render(Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

    shader.setMat4("model", model);

    star->render();

    for (auto& p : planets)
    {
        p->render(shader);
    }
}

void Star::renderOrbitalLines()
{
	for (const auto& orbit : orbits)
    {
        glBindVertexArray(orbit.VAO);
        glDrawArrays(GL_LINE_STRIP, 0, orbit.vertexCount);
    }
    glBindVertexArray(0);
}

void Star::updateStar(float deltaTime)
{
	for (auto& p : planets)
	{
		p->updatePlanet(deltaTime);
	}
}

void Star::destroy()
{
	for (auto& p : planets)
	{
		p->destroy();
	}

}

std::vector<glm::vec3> Star::generateOrbitCircle(float radius, int segments)
{
    std::vector<glm::vec3> vertices;
    float angleStep = 2.0f * glm::pi<float>() / segments;

    for (int i = 0; i <= segments; ++i)  // <= damit der Kreis geschlossen wird
    {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back(glm::vec3(x, 0.0f, z)); // Y=0, Kreis in XZ-Ebene
    }

    return vertices;
}

void Star::uploadOrbit(float radius)
{
	auto vertices = generateOrbitCircle(radius);
	Orbit orbit;
	orbit.vertexCount = vertices.size();

	glGenVertexArrays(1, &orbit.VAO);
	glGenBuffers(1, &orbit.VBO);

	glBindVertexArray(orbit.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, orbit.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);

	orbits.push_back(orbit);

}

const std::vector<std::unique_ptr<Planet>>& Star::getPlanets() const 
{ 
	return planets; 
}