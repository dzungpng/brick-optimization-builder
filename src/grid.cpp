#include "grid.h"

Grid::Grid() {}

Grid::Grid(glm::vec3 dim, glm::vec3 origin): dim(dim), origin(origin)
{
  baseGrid = std::vector<Brick>((dim[0] * dim[1] * dim[2]));
}

Grid::~Grid() {
//    for(int i = 0; i < baseGrid.size(); i++) {
//        delete baseGrid[i];
//    }
}

int Grid::flat(int x, int y, int z) const {
    return y * dim[0] + x + z * dim[0] * dim[1];
}

void Grid::setBrick(Brick& brick) {
    glm::vec3 pos = brick.getPos();
    // loop over all 1x1 units of the brick and fill in grid
    for (int i = 0; i < brick.getScale()[0]; i++) {
        for (int j = 0; i < brick.getScale()[1]; j++) {
            // Shifting brick so that we assume the bottom left corner of the grid starts at 0,0,0 to index into 1D array
            glm::vec3 shiftedBrickPos = pos + shift;
            int gridPos = flat(shiftedBrickPos[0] + i, shiftedBrickPos[1], shiftedBrickPos[2] + j);
            baseGrid[gridPos] = brick;
        }
    }
}


