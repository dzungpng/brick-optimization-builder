#include "graph.h"

Graph::Graph(const int numVertices)
{
    vertices = vector<shared_ptr<Brick>>(numVertices, nullptr);
    adjList = vector<vector<shared_ptr<Brick>>>(numVertices, vector<shared_ptr<Brick>>(0));
}

void Graph::addVertex(Brick& b) {
    if(b.getId() >= vertices.size()) {
        MGlobal::displayError("out of range in Graph::addVertex!!");
        throw "Out of range error in Graph::addVertex!";
        return;
    }
    vertices[b.getId()] = make_shared<Brick>(b);
}

/***
 * Iterate through the neighbors of brick and add them to brick's adjacency list
 * We only add a neighbor on the top and bottom of the brick (since connectivity is
 * defined as bricks snapped together in their cavities
***/
void Graph::iterateBrickNeighborsAndAddEdges(const Brick& brick, Grid& grid) {
    glm::vec3 pos = brick.getPos();
    glm::vec2 scale = brick.getScale();
    vector<int> addedBricks = vector<int>();
    for(int x = pos.x; x < pos.x + scale[0]; x++) {
        for(int z = pos.z; z < pos.z + scale[1]; z++) {
            // Top neighboring brick
            glm::vec3 neighborTopCoords(x, pos.y + 1, z);
            if(grid.isBrickInBounds(neighborTopCoords)) {
                int indexNeighborTop = grid.flat(x, neighborTopCoords.y, z);
                Brick neighborTop = grid.getBrickWithIndex(indexNeighborTop);
                if(neighborTop.getType() == BRICK) {
                    vector<int>::iterator it = find(
                                addedBricks.begin(), addedBricks.end(), neighborTop.getId());
                    if(it == addedBricks.end()) {
                        // have not added this neighbor
                        addedBricks.push_back(neighborTop.getId());
                        if(neighborTop.getId() >= vertices.size()) {
                            MGlobal::displayError("out of range neighbor top!");
                            throw "Out of range error in Graph::iterateBrickNeighbors!";
                            return;
                        }
                        adjList[brick.getId()].push_back(vertices[neighborTop.getId()]);
                    }
                }
            }
            // Bottom neighboring brick
            glm::vec3 neighborBottomCoords(x, pos.y - 1, z);
            if(grid.isBrickInBounds(neighborBottomCoords)) {
                int indexNeighborBottom = grid.flat(x, neighborBottomCoords.y, z);
                Brick neighborBottom = grid.getBrickWithIndex(indexNeighborBottom);
                if(neighborBottom.getType() == BRICK) {
                    vector<int>::iterator it = find(
                                addedBricks.begin(), addedBricks.end(), neighborBottom.getId());
                    if(it == addedBricks.end()) {
                        // have not added this neighbor
                        addedBricks.push_back(neighborBottom.getId());
                        if(neighborBottom.getId() >= vertices.size()) {
                            MGlobal::displayError("out of range neighbor top!");
                            throw "Out of range error in Graph::iterateBrickNeighbors!";
                            return;
                        }
                        adjList[brick.getId()].push_back(vertices[neighborBottom.getId()]);
                    }
                }
            }
        }
    }
}

void Graph::connectedComponentsHelper(Brick& v, map<int, bool> &visited, const int numComponents) {
    // Mark current vertex as visited
    visited[v.getId()] = true;
    // Set the compId of current vertex
    v.setCompId(numComponents);

//#define DEBUG
#ifdef DEBUG
//    int x = v.getPos().x;
//    int y = v.getPos().y;
//    int z = v.getPos().z;
//    MString idstring = "ID: ";
//    MGlobal::displayInfo(idstring + v.getId());
//    MString coord = "x Neighbor: ";
//    MGlobal::displayInfo(coord + x);
//    coord = "y Neighbor: ";
//    MGlobal::displayInfo(coord + y);
//    coord = "z Neighbor: ";
//    MGlobal::displayInfo(coord + z);
//    int xScale = v.getScale().x;
//    int yScale = v.getScale().y;
//    MString scale = "Scale x: ";
//    MGlobal::displayInfo(scale + xScale);
//    scale = "Scale y: ";
//    MGlobal::displayInfo(scale + yScale);
//    MString space = "**********";
//    MGlobal::displayInfo(space);
#endif

    // Mark all neighboring nodes to this vertices as visited (recursively)
    for(int i = 0; i < adjList[v.getId()].size(); i++)
    {
        if(!visited[adjList[v.getId()][i]->getId()]) {
            connectedComponentsHelper(*adjList[v.getId()][i], visited, numComponents);
        }
    }
}

int Graph::countConnectedComponents() {
    // Mark all the vertices as not visited
    map<int, bool> visited;
    for(auto& brick : vertices) {
        visited[brick->getId()] = false;
    }
    int numComponents = 0;
    for(int i = 0; i < vertices.size(); i++) {
        if(!visited[vertices[i]->getId()]) {
//            MString startNewComp = "---------Starting new component----------";
//            MGlobal::displayInfo(startNewComp);
            connectedComponentsHelper(*vertices[i], visited, numComponents);
            numComponents++;
        }
    }
    return numComponents;
}

int Graph::countNumDistinctComponents(const Brick& b, const Grid& grid, const int totalCompIds) {
    glm::vec3 pos = b.getPos();
    glm::vec2 scale = b.getScale();
    int numCompIds = 1;
    map<int, bool> seenCompId;
    for(int i = 0; i < totalCompIds; i++) {
        seenCompId[i] = false;
    }
    for(int x = pos.x - 1; x < pos.x + scale[0] + 1; x++) {
        for(int y = pos.y - 1; y <= pos.y + 1; y++) {
            for(int z = pos.z - 1; z < pos.z + scale[1] + 1; z++) {
                glm::vec3 neighborCoords(x, y, z);
                if(grid.isBrickInBounds(neighborCoords)) {
                    Brick neighbor = grid.getBrick(neighborCoords);
                    if(neighbor.getType() == BRICK) {
                        int neighborCompId = neighbor.getCompId();
                        if (neighborCompId != b.getCompId() && !seenCompId[neighborCompId]) {
                            numCompIds++;
                            seenCompId[neighborCompId] = true;
                        }
                    }
                }
            }
        }
    }
    return numCompIds;
}


