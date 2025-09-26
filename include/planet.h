#ifndef PLANET_H
#define PLANET_H

#include "block.h"
#include "shader.h"
#include "FastNoiseLite.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <cstdint>

struct Vertex {
  glm::vec3 position;
  float shade;
  glm::vec3 color;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

extern const float PI;

class Planet {
private:
	// planet
	int positionX, positionY, positionZ;
	int PLANET_SIZE;
	int radius;

	glm::ivec3 center;

	// orbit
	float orbitAngle;
	float orbitSpeed;
	float orbitRadius;
	float orbitInclination;

	// planet rotation
	float currentRotation;
	float rotationSpeed;

	// blocks
	std::vector<uint8_t> blocks;
	std::vector<Block> visibleBlocks;

	// render
	int indexCount;
	unsigned int VAO, VBO, EBO;

	Mesh mesh;
public:
	Planet(float posX, float posY, float posZ, int size, float orbitalSpeed, float orbitalInclination, float orbitalPosition);
	~Planet();

	void fillPlanet();
	void checkNeighbors();
	void buildMesh(const float* cubeVertices, const unsigned int* cubeIndices);
	void upload();
	void render(Shader& shader);
	void render();

	void updatePlanet(float deltaTime);

	void destroy();
	uint8_t createBlock(BlockType type);

	glm::vec3 getCenter() const;
	glm::vec3 getPosition() const;
	glm::vec3 getWorldPosition() const;
	float getRadius() const;
	int getSize() const;

	glm::vec3 getAABBMin() const;
	glm::vec3 getAABBMax() const;

	inline int idx(int x, int y, int z);
	inline bool isBlockSolid(uint8_t id);
};


#endif