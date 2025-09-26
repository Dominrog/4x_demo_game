#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <cstdint>

enum class BlockType {
  Empty,
  Stone,
  Dirt,
  Grass,
  Water,
  Ice
};

enum class BlockSide {
    BACK,  // 0
    FRONT, // 1
    LEFT,  // 2
    RIGHT, // 3
    BOTTOM,// 4
    TOP    // 5
};


struct Block {
	int x, y, z;               
	std::vector<BlockSide> sides; 
    BlockType type;
};


struct BlockInfo {
    bool isSolid;
    glm::vec3 color;

    BlockInfo() : isSolid(false), color(1.0f, 1.0f, 1.0f) {}

    BlockInfo(bool solid, glm::vec3 col)
        : isSolid(solid), color(col) {}
};


extern float cubeVertices[96];
extern unsigned int cubeIndices[36];

extern BlockInfo BlockRegistry[256];
void initBlockRegistry();