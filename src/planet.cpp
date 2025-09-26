#include "planet.h"

const float PI = 3.14159265358979323846f;

Planet::Planet(float posX, float posY, float posZ, int size, float orbitalSpeed, float orbitalInclination, float orbitalPosition)
    :   positionX(posX),
        positionY(posY),
        positionZ(posZ),
        PLANET_SIZE(size * 2),
        radius(size / 2),
        center(PLANET_SIZE/2, PLANET_SIZE/2, PLANET_SIZE/2),
        indexCount(0),
        orbitAngle(orbitalPosition),
        orbitSpeed(orbitalSpeed),
        orbitRadius(posX),
        orbitInclination(orbitalInclination),
        currentRotation(0.0f),
        rotationSpeed(0.15f) // z.B. 0.5 rad/s
{

}

Planet::~Planet()
{

}


void Planet::fillPlanet()
{
    blocks.resize(PLANET_SIZE * PLANET_SIZE * PLANET_SIZE, static_cast<uint8_t>(BlockType::Empty));

    FastNoiseLite noise;
    noise.SetSeed(1337223);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFrequency(0.005f);

    float amplitude = 1.0f; 
    float waterLevel = radius * 1.0f;
    float iceThreshold = 0.85f;

    for (int x = -int(waterLevel); x <= int(waterLevel); x++) {
        for (int y = -int(waterLevel); y <= int(waterLevel); y++) {
            for (int z = -int(waterLevel); z <= int(waterLevel); z++) {
                
                float distance = sqrt(float(x*x + y*y + z*z));
                float relativeY = float(y) / radius;
                bool isIce = (relativeY > iceThreshold || relativeY < -iceThreshold);

                float scale = 200.0f;
                float nx = float(x) / radius * scale;
                float ny = float(y) / radius * scale;
                float nz = float(z) / radius * scale;

                float noiseValue = noise.GetNoise(nx, ny, nz) * amplitude;

                float localRadius = radius + noiseValue;

                if (localRadius < waterLevel) {
                    localRadius = waterLevel;
                }

                if (distance <= localRadius) {
                    int wx = x + center.x;
                    int wy = y + center.y;
                    int wz = z + center.z;

                    if (localRadius == waterLevel)
                    {
                        if (!isIce)
                        {
                            blocks[idx(wx, wy, wz)] = createBlock(BlockType::Water);
                        }
                        else 
                        {
                            blocks[idx(wx, wy, wz)] = createBlock(BlockType::Ice);
                        }
                    } 
                    else
                    {   
                        if (!isIce)
                        {
                            blocks[idx(wx, wy, wz)] = createBlock(BlockType::Grass);
                        }
                        else 
                        {
                            blocks[idx(wx, wy, wz)] = createBlock(BlockType::Ice);
                        }
                    }
                }
            }
        }
    }
}

void Planet::checkNeighbors() {
    visibleBlocks.clear();

    for (int x = 0; x < PLANET_SIZE; x++) {
        for (int y = 0; y < PLANET_SIZE; y++) {
            for (int z = 0; z < PLANET_SIZE; z++) {

                uint8_t id = blocks[idx(x, y, z)];
                if(id == static_cast<uint8_t>(BlockType::Empty)) continue;

                auto getBlock = [&](int nx, int ny, int nz) -> uint8_t {
                    if(nx < 0 || nx >= PLANET_SIZE || ny < 0 || ny >= PLANET_SIZE || nz < 0 || nz >= PLANET_SIZE)
                        return static_cast<uint8_t>(BlockType::Empty);
                    return blocks[idx(nx, ny, nz)];
                };

                int worldX = x + center.x;
				int worldY = y + center.y;
				int worldZ = z + center.z;

                uint8_t left   = getBlock(x-1, y, z);
                uint8_t right  = getBlock(x+1, y, z);
                uint8_t back   = getBlock(x, y, z-1);
                uint8_t front  = getBlock(x, y, z+1);
                uint8_t bottom = getBlock(x, y-1, z);
                uint8_t top    = getBlock(x, y+1, z);

                std::vector<BlockSide> sides;

                if(!isBlockSolid(left))   sides.push_back(BlockSide::LEFT);
                if(!isBlockSolid(right))  sides.push_back(BlockSide::RIGHT);
                if(!isBlockSolid(bottom)) sides.push_back(BlockSide::BOTTOM);
                if(!isBlockSolid(top))    sides.push_back(BlockSide::TOP);
                if(!isBlockSolid(back))   sides.push_back(BlockSide::BACK);
                if(!isBlockSolid(front))  sides.push_back(BlockSide::FRONT);

                if(!sides.empty()){
					glm::vec3 localPos = glm::vec3(x, y, z) - glm::vec3(center); 
                    visibleBlocks.push_back({ 
                        static_cast<int>(localPos.x), 
                        static_cast<int>(localPos.y), 
                        static_cast<int>(localPos.z), 
                        sides,
                        static_cast<BlockType>(id)
                    });
				}
            }
        }
    }
}

void Planet::buildMesh(const float* cubeVertices, const unsigned int* cubeIndices)
{
    mesh.vertices.clear();
    mesh.indices.clear();

    int vertexOffset = 0;

    for (const Block& block : visibleBlocks)
    {
        glm::vec3 blockPos(block.x, block.y, block.z);
        glm::vec3 blockColor = BlockRegistry[static_cast<uint8_t>(block.type)].color;

        for (BlockSide side : block.sides)
        {
            // 4 Vertices pro Face
            int faceStart = static_cast<int>(side) * 4;

            for (int corner = 0; corner < 4; ++corner)
            {
                int vi = faceStart + corner;

                glm::vec3 pos(cubeVertices[vi * 4 + 0],
                              cubeVertices[vi * 4 + 1],
                              cubeVertices[vi * 4 + 2]);
                float shade = cubeVertices[vi * 4 + 3];

                mesh.vertices.push_back({pos + blockPos, shade, blockColor});
            }

            int idxOffset = static_cast<int>(side) * 6;
            for (int i = 0; i < 6; ++i)
                mesh.indices.push_back(vertexOffset + cubeIndices[idxOffset + i]);

            vertexOffset += 4;
        }
    }
}



void Planet::upload()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Shade
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, shade));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);

    indexCount = mesh.indices.size();

    if (!glIsVertexArray(VAO)) {
        std::cout << "[upload] Warning: VAO is invalid!" << std::endl;
        return;
    }
}

void Planet::render(Shader& shader)
{
	if (glIsVertexArray(VAO) == GL_FALSE) {
		std::cout << "[render] Warning: VAO is invalid!" << std::endl;
		return;
	}

	if (indexCount == 0) {
		std::cout << "[render] Warning: indexCount = 0 for Planet at (" 
		          << positionX << "," << positionY << "," << positionZ <<")" << std::endl;
		return;
	}

    glm::mat4 model = glm::mat4(1.0f);

    // 1. Orbit um Stern (0,0,0)
    model = glm::rotate(model, orbitAngle, glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(orbitRadius, 0, 0));

    // 2. Eigenrotation + Neigung
    model = glm::rotate(model, orbitInclination, glm::vec3(0, 0, 1)); // kippt nur den Planeten
    model = glm::rotate(model, currentRotation, glm::vec3(0, 1, 0));   // dreht ihn um eigene Achse

    shader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Planet::render()
{
    if (glIsVertexArray(VAO) == GL_FALSE) {
        std::cout << "[render] Warning: VAO is invalid!" << std::endl;
        return;
    }

    if (indexCount == 0) {
        std::cout << "[render] Warning: indexCount = 0 for Planet at (" 
                  << positionX << "," << positionY << "," << positionZ <<")" << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Planet::updatePlanet(float deltaTime)
{
    // Eigendrehung
    currentRotation += rotationSpeed * deltaTime;
    if (currentRotation > 2.0f * PI)
        currentRotation -= 2.0f * PI;

    // Orbit
    orbitAngle += orbitSpeed * deltaTime;
    if (orbitAngle > 2.0f * PI)
        orbitAngle -= 2.0f * PI;
}

void Planet::destroy()
{
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
    }

    if (VBO != 0)
    {
    	glDeleteBuffers(1, &VBO);
    }

    if (EBO != 0)
    {
    	glDeleteBuffers(1, &EBO);
    }
}

uint8_t Planet::createBlock(BlockType type)
{
	return static_cast<uint8_t>(type);
}

glm::vec3 Planet::getPosition() const {
    return glm::vec3(positionX, positionY, positionZ);
}

float Planet::getRadius() const {
    return static_cast<float>(radius);
}

int Planet::getSize() const 
{ 
    return PLANET_SIZE; 
}


glm::vec3 Planet::getCenter() const {
    float x = cos(orbitAngle) * orbitRadius;
    float z = sin(orbitAngle) * orbitRadius;
    float y = positionY;

    return glm::vec3(x, y, -z);
}

glm::vec3 Planet::getWorldPosition() const {
    glm::vec3 pos(orbitRadius, 0.0f, 0.0f);
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), orbitAngle, glm::vec3(0, 1, 0));
    pos = glm::vec3(rot * glm::vec4(pos, 1.0f));

    rot = glm::rotate(glm::mat4(1.0f), orbitInclination, glm::vec3(0, 0, 1));
    pos = glm::vec3(rot * glm::vec4(pos, 1.0f));

    return pos;
}

glm::vec3 Planet::getAABBMin() const {
    glm::vec3 minBlock(1e9f);
    for(const auto& block : visibleBlocks) {
        glm::vec3 blockPos(block.x, block.y, block.z);
        if(blockPos.x < minBlock.x) minBlock.x = blockPos.x;
        if(blockPos.y < minBlock.y) minBlock.y = blockPos.y;
        if(blockPos.z < minBlock.z) minBlock.z = blockPos.z;
    }

    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), orbitAngle, glm::vec3(0,1,0));
    transform = glm::translate(transform, glm::vec3(orbitRadius, 0, 0));
    transform = glm::rotate(transform, orbitInclination, glm::vec3(0,0,1));

    return glm::vec3(transform * glm::vec4(minBlock, 1.0f));
}

glm::vec3 Planet::getAABBMax() const {
    glm::vec3 maxBlock(-1e9f);
    for(const auto& block : visibleBlocks) {
        glm::vec3 blockPos(block.x, block.y, block.z);
        if(blockPos.x > maxBlock.x) maxBlock.x = blockPos.x;
        if(blockPos.y > maxBlock.y) maxBlock.y = blockPos.y;
        if(blockPos.z > maxBlock.z) maxBlock.z = blockPos.z;
    }

    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), orbitAngle, glm::vec3(0,1,0));
    transform = glm::translate(transform, glm::vec3(orbitRadius, 0, 0));
    transform = glm::rotate(transform, orbitInclination, glm::vec3(0,0,1));

    return glm::vec3(transform * glm::vec4(maxBlock, 1.0f));
}



inline int Planet::idx(int x, int y, int z) {
    return x + PLANET_SIZE * (y + PLANET_SIZE * z);
}

inline bool Planet::isBlockSolid(uint8_t id) {
    const BlockInfo& info = BlockRegistry[id];
    return info.isSolid;
}