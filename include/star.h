#ifndef STAR_H
#define STAR_H

#include "planet.h"
#include "shader.h"

#include <memory>


struct Orbit {
    GLuint VAO;
    GLuint VBO;
    int vertexCount;
};


class Star {
private:
	float positionX, positionY, positionZ;
	std::vector<std::unique_ptr<Planet>> planets;
	std::unique_ptr<Planet> star;
	std::vector<Orbit> orbits;
public:
	Star(float posX, float posY, float posZ);
	~Star();

	void addPlanet(float posX, float posY, float posZ, int size, float orbitalSpeed, float orbitalInclination, float orbitalPosition);
	void render(Shader& shader);

	void updateStar(float deltaTime);

	void destroy();

	std::vector<glm::vec3> generateOrbitCircle(float radius, int segments = 64);
	void uploadOrbit(float radius);
	void renderOrbitalLines();

	const std::vector<std::unique_ptr<Planet>>& getPlanets() const;

};

#endif