#include "brick.h"

int Brick::id = 0;

Brick::Brick(glm::vec3 pos, BrickType type): pos(pos), type(type)
{
    id++;
}
