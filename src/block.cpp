#include "block.h"


float cubeVertices[96] = {
    // positions            // shade
    -0.5f, -0.5f, -0.5f,    0.9f, // back face
     0.5f, -0.5f, -0.5f,    0.9f,
     0.5f,  0.5f, -0.5f,    0.9f,
    -0.5f,  0.5f, -0.5f,    0.9f,

    -0.5f, -0.5f,  0.5f,    0.8f, // front face
     0.5f, -0.5f,  0.5f,    0.8f,
     0.5f,  0.5f,  0.5f,    0.8f,
    -0.5f,  0.5f,  0.5f,    0.8f,

    -0.5f,  0.5f,  0.5f,    0.6f, // left face
    -0.5f,  0.5f, -0.5f,    0.6f,
    -0.5f, -0.5f, -0.5f,    0.6f,
    -0.5f, -0.5f,  0.5f,    0.6f,

     0.5f,  0.5f,  0.5f,    0.7f, // right face
     0.5f,  0.5f, -0.5f,    0.7f,
     0.5f, -0.5f, -0.5f,    0.7f,
     0.5f, -0.5f,  0.5f,    0.7f,

    -0.5f, -0.5f, -0.5f,    0.5f, // bottom face
     0.5f, -0.5f, -0.5f,    0.5f,
     0.5f, -0.5f,  0.5f,    0.5f,
    -0.5f, -0.5f,  0.5f,    0.5f,

    -0.5f,  0.5f, -0.5f,    1.0f, // top face
     0.5f,  0.5f, -0.5f,    1.0f,
     0.5f,  0.5f,  0.5f,    1.0f,
    -0.5f,  0.5f,  0.5f,    1.0f
};

unsigned int cubeIndices[36] = {
    0, 3, 2, 2, 1, 0, // top 
    0, 1, 2, 2, 3, 0, // bottom
    0, 1, 2, 2, 3, 0, // left
    0, 3, 2, 2, 1, 0, // right 
    0, 1, 2, 2, 3, 0, // front 
    0, 3, 2, 2, 1, 0  // back
};

BlockInfo BlockRegistry[256];

void initBlockRegistry()
{
    BlockRegistry[static_cast<uint8_t>(BlockType::Empty)]  = {false, glm::vec3(0.0f,0.0f,0.0f)};
    BlockRegistry[static_cast<uint8_t>(BlockType::Stone)]  = {true,  glm::vec3(0.5f,0.5f,0.5f)};
    BlockRegistry[static_cast<uint8_t>(BlockType::Dirt)]   = {true,  glm::vec3(0.55f,0.27f,0.07f)};
    BlockRegistry[static_cast<uint8_t>(BlockType::Grass)]  = {true,  glm::vec3(0.0f,0.6f,0.2f)};
    BlockRegistry[static_cast<uint8_t>(BlockType::Water)]  = {true,  glm::vec3(0.0f,0.3f,0.9f)};
    BlockRegistry[static_cast<uint8_t>(BlockType::Ice)]    = {true,  glm::vec3(0.9f,0.9f,0.9f)};
}