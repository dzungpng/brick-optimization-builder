#pragma once
#include <include/glm/vec2.hpp>
#include <include/glm/vec3.hpp>
#include "maya/MColor.h"

enum BrickType {EMPTY, BRICK};

class Brick
{
private:
    int brickId;
    // id of the connected component this brick is associated with
    int compId;
    // assigned color based on input mesh
    MColor color;



    // global transform data
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec2 scale;

public:
    Brick(); // default is one by one brick
    Brick(glm::vec3 pos, BrickType type, glm::vec2 scale);
    ~Brick();

    // unique identifier for each individual brick
    static int id;

    // shape identifier - just used to tell us if a grid cell is empty or a real brick
    BrickType type;

    void setPos(glm::vec3 p) {pos = p;}
    void setRot(glm::vec3 r) {rot = r;}
    void setScale(glm::vec2 s) {scale = s;}
    void setColor(MColor c) {color = c;}
    void setCompId(int i) {compId = i;}
    void setType(BrickType bt) {type = bt;}
    void setBrickId(const int newId) { brickId = newId; }

    glm::vec3 getPos() const {return pos;}
    glm::vec3 getRot() const {return rot;}
    glm::vec2 getScale() const {return scale;}
    MColor getColor() const {return color;}
    int getId() const {return brickId;}
    int getCompId() const {return compId;}
    BrickType getType() const {return type;}
};
