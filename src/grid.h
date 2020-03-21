#pragma once
#include <vector>
#include <include/glm/vec3.hpp>
#include <brick.h>

class Grid
{
private:
    // dimension of grid in 3d space
    glm::vec3 dim;
    // lower left corner of the grid
    glm::vec3 origin;

    std::vector<Brick*> baseGrid;

    // flattens 3d coordinates to become grid index
    int flat(int x, int y, int z) const;
public:
    Grid(glm::vec3 dim, glm::vec3 origin);

    void setBrick(Brick* brick);

    Brick* getBrick(glm::vec3 pos);
};
