#include "brick.h"

int Brick::id = 0;

Brick::Brick() : type(EMPTY), pos(glm::vec3(0)), scale(glm::vec2(1)), brickId(id), color(MColor(0.0, 0.0, 0.0))
{
    id++;
}

Brick::Brick(glm::vec3 pos, BrickType type, glm::vec2 scale): type(type), pos(pos), scale(scale), brickId(id), color(MColor(0.0, 0.0, 0.0))
{
    id++;
}

Brick::~Brick() {}

