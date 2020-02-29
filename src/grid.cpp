#include "grid.h"

Grid::Grid(glm::vec3 dim, glm::vec3 origin): dim(dim), origin(origin)
{
  baseGrid = std::vector<Brick*>((dim[0] * dim[1] * dim[2]), nullptr);
}

int Grid::flat(int x, int y, int z) const {
    return y * dim[0] + x + z * dim[0] * dim[1];
}

void Grid::setBrick(Brick* brick) {
    glm::vec3 pos = brick->getPos();
    // loop over all 1x1 units of the brick and fill in grid
    for (int i = 0; i < brick->getScale()[0]; i++) {
        for (int j = 0; i < brick->getScale()[1]; j++) {
            int gridPos = flat(pos[0] + i, pos[1], pos[2] + j);
            baseGrid[gridPos] = brick;
        }
    }
}


