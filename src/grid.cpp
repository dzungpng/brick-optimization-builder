#include "grid.h"

Grid::Grid() {}

Grid::Grid(glm::vec3 dim, glm::vec3 origin): dim(dim), origin(origin)
{
    baseGrid = std::vector<Brick>((dim[0] * dim[1] * dim[2]), Brick());
}

Grid::~Grid() {
    //    for(int i = 0; i < baseGrid.size(); i++) {
    //        delete baseGrid[i];
    //    }
}

int Grid::flat(int x, int y, int z) const {
    return x + y * dim.x + z * dim.x * dim.y;
}

bool Grid::isBrickInBounds(glm::vec3 brickPos) const {
    return (brickPos.x < dim.x && brickPos.y < dim.y  && brickPos.z < dim.z &&
            brickPos.x >= 0 && brickPos.y >= 0 && brickPos.z >= 0);
}


void Grid::initializeBrick(Brick& brick) {
    glm::vec3 pos = brick.getPos();
    // loop over all 1x1 units of the brick and fill in grid
    glm::vec3 shiftedBrickPos = pos + shift;
    // permanently change the position of the brick so that we assume an origin of (0,0,0)
    brick.setPos(shiftedBrickPos);
    for (int i = 0; i < brick.getScale().x; i++) {
        for (int j = 0; j < brick.getScale().y; j++) {
            // Shifting brick so that we assume the bottom left corner of the grid starts at 0,0,0 to index into 1D array

            int gridPos = flat(shiftedBrickPos.x + i, shiftedBrickPos.y, shiftedBrickPos.z + j);
            if(gridPos < 0 || gridPos >= baseGrid.size()) {
                MString info = "index: ";
                MGlobal::displayInfo(info + gridPos);
                MGlobal::displayError("ERROR: index out of range in Grid::getBrick!");
                return;
            }

            // update our map containing all bricks
            if (allBricks.count(baseGrid[gridPos].getId())) {
                // erase original brick
                allBricks.erase(baseGrid[gridPos].getId());
            }
            // insert new one if not already present
            if (!allBricks.count(brick.getId())) {
                allBricks.insert(std::pair<int, Brick>(brick.getId(), brick));
            }

            baseGrid[gridPos] = brick;
        }
    }
}

void Grid::setBrick(Brick& brick) {
    glm::vec3 pos = brick.getPos();
    // loop over all 1x1 units of the brick and fill in grid
    for (int i = 0; i < brick.getScale().x; i++) {
        for (int j = 0; j < brick.getScale().y; j++) {
            // Shifting brick so that we assume the bottom left corner of the grid starts at 0,0,0 to index into 1D array
            int gridPos = flat(pos.x + i, pos.y, pos.z + j);

            if(gridPos < 0 || gridPos >= baseGrid.size() || !isBrickInBounds(glm::vec3(pos.x + i, pos.y, pos.z + j))) {
                MString info = "index: ";
                MGlobal::displayInfo(info + gridPos);
                MGlobal::displayError("ERROR: index out of range in Grid::getBrick!");
                return;
            }

            // update our map containing all bricks
            if (allBricks.count(baseGrid[gridPos].getId())) {
                // erase original brick
                allBricks.erase(baseGrid[gridPos].getId());
            }
            // insert new one if not already present
            if (!allBricks.count(brick.getId())) {
                allBricks.insert(std::pair<int, Brick>(brick.getId(), brick));
            }

            baseGrid[gridPos] = brick;
        }
    }
}

const Brick Grid::getBrick(const glm::vec3 brickPos) const {
    if(isBrickInBounds(brickPos)) {
        int index = flat(brickPos[0], brickPos[1], brickPos[2]);
        if(index < 0 || index >= baseGrid.size()) {
            // return empty brick
            MGlobal::displayError("ERROR: index out of range in Grid::getBrick!");
            return Brick();
        }
        return baseGrid[index];
    }
    return Brick();
}

void Grid::initialize(const MBoundingBox& boundingBox) {
    setDim(glm::vec3(ceilf(boundingBox.width()), ceilf(boundingBox.height()), ceilf(boundingBox.depth())));
    setOrigin(glm::vec3(boundingBox.min().x, boundingBox.min().y, boundingBox.min().z));
    setBaseGridSize();
    setShift();
}

