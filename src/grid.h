#pragma once
#pragma once
#include <vector>
#include <include/glm/vec3.hpp>
#include <brick.h>
#include <maya/MGlobal.h>
#include <maya/MBoundingBox.h>
#include <map>

class Grid
{
private:
    // dimension of grid in 3d space
    glm::vec3 dim;
    // lower left corner of the grid
    glm::vec3 origin;
    // the amount of shifting needed to "move" the grid to 0,0,0 so that we can index into our flattened baseGrid
    glm::vec3 shift;

    std::vector<Brick> baseGrid;

    // flattens 3d coordinates to become grid index
    int flat(int x, int y, int z) const;
public:
    Grid(glm::vec3 dim, glm::vec3 origin);
    Grid();
    ~Grid();

    std::map<int, Brick> allBricks = std::map<int, Brick>();

    void setBrick(Brick& brick);
    // same as setBrick except used when first filling grid. shifts positions such that our layout is at (0, 0, 0)
    void initializeBrick(Brick& brick);
    void setDim(const glm::vec3 newDim) { dim = newDim; }
    void setBaseGridSize() { baseGrid = std::vector<Brick>(dim.x * dim.y * dim.z); }
    void setOrigin(const glm::vec3 newOrigin) { origin = newOrigin; }
    void setShift() { shift = glm::vec3(0) - origin; }

    glm::vec3 getDim() const { return dim; }
    glm::vec3 getOrigin() const { return origin; }
    int getBaseGridLength() const { return baseGrid.size(); }
    void setBrick(Brick* brick);
    const Brick getBrick(const glm::vec3 brickPos) const;
    glm::vec3 getShift() const { return shift; }
    const Brick getBrickWithIndex(const int index) const { baseGrid[index]; }
    const std::vector<Brick> getBaseGrid() const { return baseGrid; }

    void initialize(const MBoundingBox&);
};
