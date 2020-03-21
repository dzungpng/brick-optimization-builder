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
    return x + y * dim.x + z * dim.x * dim.y;
}

void Grid::setBrick(const Brick& brick) {
    glm::vec3 pos = brick.getPos();
    // loop over all 1x1 units of the brick and fill in grid
    for (int i = 0; i < brick.getScale().x; i++) {
        for (int j = 0; j < brick.getScale().y; j++) {
            // Shifting brick so that we assume the bottom left corner of the grid starts at 0,0,0 to index into 1D array
            glm::vec3 shiftedBrickPos = pos + shift;
            int gridPos = flat(shiftedBrickPos.x + i, shiftedBrickPos.y, shiftedBrickPos.z + j);
            if(gridPos < 0 || gridPos > baseGrid.size()) {
                MString info = "index: ";
                MGlobal::displayInfo(info + gridPos);
                MGlobal::displayInfo("ERROR: index out of range in Grid::getBrick!");
                return;
            }
            baseGrid[gridPos] = brick;
        }
    }
}

const Brick Grid::getBrick(const int index) const {
   if(index < 0 || index > baseGrid.size()) {
       MGlobal::displayInfo("ERROR: index out of range in Grid::getBrick!");
   }
   return baseGrid[index];
}

void Grid::initialize(const MBoundingBox& boundingBox) {
    setDim(glm::vec3(ceilf(boundingBox.width()), ceilf(boundingBox.height()), ceilf(boundingBox.depth())));
    setOrigin(glm::vec3(boundingBox.min().x, boundingBox.min().y, boundingBox.min().z));
    setBaseGridSize();
    setShift();
}


