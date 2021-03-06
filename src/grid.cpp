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

Grid::Grid(const Grid& grid) : shift(grid.shift), dim(grid.dim), origin(grid.origin) {
    baseGrid = grid.baseGrid;
    allBricks = grid.allBricks;
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
                // MGlobal::displayInfo(info + gridPos);
                // MGlobal::displayError("ERROR: index out of range in Grid::getBrick!");
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
                 MGlobal::displayError("ERROR: index out of range in Grid::setBrick!");
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
        return baseGrid[index];
    } else {
        // MGlobal::displayError("Index out of range in Grid::getBrick!");
        return Brick();
    }
}

void Grid::initialize(const MBoundingBox& boundingBox) {
    setDim(glm::vec3(ceilf(boundingBox.width()), ceilf(boundingBox.height()), ceilf(boundingBox.depth())));
    setOrigin(glm::vec3(boundingBox.min().x, boundingBox.min().y, boundingBox.min().z));
    setBaseGridSize();
    setShift();
}

const Brick Grid::getBrickWithIndex(const int index) const {
    if(index < 0 || index >= baseGrid.size()) {
        MGlobal::displayError("index out of range in Grid::getBrick!");
        return Brick();
    }
    return baseGrid[index];
}

void Grid::setBrickId(const int newId, const Brick& b) {
    glm::vec3 pos = b.getPos();
    glm::vec2 scale = b.getScale();
    for(int x = pos.x; x < pos.x + scale[0]; x++) {
        for(int z = pos.z; z < pos.z + scale[1]; z++) {
            if(!isBrickInBounds(glm::vec3(x, pos.y, z))) {
                MGlobal::displayError("Out of bounds in Grid::setBrickId!");
                return;
            }
            int index = flat(x, pos.y, z);
            baseGrid[index].setBrickId(newId);
        }
    }
}

void Grid::setbaseGridCompIds(const Graph& graph) {
    for (const auto& brick : graph.vertices) {
        glm::vec3 pos = brick->getPos();
        glm::vec2 scale = brick->getScale();
        for(int x = pos.x; x < pos.x + scale[0]; x++) {
            for(int z = pos.z; z < pos.z + scale[1]; z++) {
                if(!isBrickInBounds(glm::vec3(x, pos.y, z))) {
                    MGlobal::displayError("Out of bounds in Grid::setBrickId!");
                    return;
                }
                int index = flat(x, pos.y, z);
                baseGrid[index].setCompId(brick->getCompId());
            }
        }
    }
}

void Grid::splitBrick(const Brick& b, map<glm::vec3, bool, cmpVec3>& seenBricks) {
    glm::vec3 pos = b.getPos();
    glm::vec2 scale = b.getScale();
    // If already 1x1 no need to split
    if(scale[0] == 1 && scale[1] == 1) {
        return;
    }
//#define DEBUG
#ifdef DEBUG
    MString info = "Splitting brick id: -----------------";
    MGlobal::displayInfo(info + b.getId());
    MString position = "At x: ";
    MGlobal::displayInfo(position + pos.x);
    position = "At y: ";
    MGlobal::displayInfo(position + pos.y);
    position = "At z: ";
    MGlobal::displayInfo(position + pos.z);
    MString s = "With scale x: ";
    MGlobal::displayInfo(s + scale.x);
    s = "With scale z: ";
    MGlobal::displayInfo(s + scale.y);
#endif
    for(int x = pos.x; x < pos.x + scale[0]; x++) {
        for(int z = pos.z; z < pos.z + scale[1]; z++) {
            glm::vec3 newPos(x, pos.y, z);
            Brick oneXone(newPos, BRICK, glm::vec2(1));
            setBrick(oneXone);
            seenBricks[newPos] = true;
        }
    }
}


Grid Grid::splitBricks(const Brick& wL, const int k, map<glm::vec3, bool, cmpVec3>& seenBrickPos) const {
    glm::vec3 pos = wL.getPos();
    glm::vec2 scale = wL.getScale();
    Grid L_p(*this);

    for(auto& pair : L_p.allBricks) {
        glm::vec3 bPos = pair.second.getPos();
        glm::vec2 bScale = pair.second.getScale();
        for(int x = bPos.x; x < bPos.x + bScale[0]; x++) {
            for(int z = bPos.z; z < bPos.z + bScale[1]; z++) {
                seenBrickPos[glm::vec3(x, bPos.y, z)] = false;
            }
        }
    }

    glm::vec3 upperBound(pos.x + k + 1 + scale[0],
                         pos.y + k + 1,
                         pos.z + k + 1 + scale[1]);
    for(int x = pos.x - k; x < upperBound.x; x++) {
        for(int y = pos.y - k; y < upperBound.y; y++) {
            for(int z = pos.z - k; z < upperBound.z; z++) {
                glm::vec3 neighborCoords(x, y, z);
                if(L_p.isBrickInBounds(neighborCoords)) {
                    Brick neighbor = L_p.getBrick(neighborCoords);
                    // Only split a brick if it's not wL, not empty,
                    // and we haven't seen the coordinates of all of its parts
                    if(neighbor.getId() != wL.getId() &&
                            neighbor.getType() != EMPTY &&
                            !seenBrickPos[neighborCoords])
                    {
                        L_p.splitBrick(neighbor, seenBrickPos);
                    }
                }
            }
        }
    }
    return L_p;
}
