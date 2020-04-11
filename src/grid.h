#pragma once
#include <vector>
#include <include/glm/vec3.hpp>
#include <brick.h>
#include <maya/MGlobal.h>
#include <maya/MBoundingBox.h>
#include <map>

#include "graph.h"
using namespace std;
class Graph;

struct cmpVec3 {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        return a.x < b.x || a.y < b.y || a.z < b.z;
    }
};

class Grid
{
private:
    // dimension of grid in 3d space
    glm::vec3 dim;
    // lower left corner of the grid
    glm::vec3 origin;
    // the amount of shifting needed to "move" the grid to 0,0,0 so that we can index into our flattened baseGrid
    glm::vec3 shift;

    void splitBrick(const Brick&, map<glm::vec3, bool, cmpVec3>&);
public:
    Grid(glm::vec3 dim, glm::vec3 origin);
    Grid(const Grid&);
    Grid();
    ~Grid();

    std::map<int, Brick> allBricks = std::map<int, Brick>();
    std::vector<Brick> baseGrid;

    void setBrick(Brick& brick);
    // same as setBrick except used when first filling grid. shifts positions such that our layout is at (0, 0, 0)
    void initializeBrick(Brick& brick);
    void setDim(const glm::vec3 newDim) { dim = newDim; }
    void setBaseGridSize() { baseGrid = std::vector<Brick>(dim.x * dim.y * dim.z); }
    void setOrigin(const glm::vec3 newOrigin) { origin = newOrigin; }
    void setShift() { shift = glm::vec3(0) - origin; }
    void setBrickId(const int newId, const Brick& b);

    glm::vec3 getDim() const { return dim; }
    glm::vec3 getOrigin() const { return origin; }
    int getBaseGridLength() const { return baseGrid.size(); }
    const Brick getBrick(const glm::vec3 brickPos) const;
    glm::vec3 getShift() const { return shift; }
    const Brick getBrickWithIndex(const int index) const;
    const std::vector<Brick> getBaseGrid() const { return baseGrid; }

    // flattens 3d coordinates to become grid index
    int flat(int x, int y, int z) const;
    void initialize(const MBoundingBox&);
    bool isBrickInBounds(glm::vec3 brickPos) const; // check if the position of the brick is in bounds
    void setbaseGridCompIds(const Graph&); // set comp ids for all the bricks in baseGrid

    // Layout reconfiguration helpers
    Grid splitBricks(const Brick&, const int, map<glm::vec3, bool, cmpVec3>&) const; // Split the bricks within k-ring of input brick into 1x1 blocks
                                               // Returns a new grid where all of the bricks in k-ring neighbor
                                               // of input brick are splitted
};
