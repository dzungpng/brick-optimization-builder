#ifndef GRAPH_H
#define GRAPH_H

#pragma once
#include "brick.h"
#include "grid.h"
#include <map>
#include <vector>

using namespace std;


class Grid;

class Graph
{
public:
    Graph(const int);
    Graph() {}
    ~Graph() {}

    vector<shared_ptr<Brick>> vertices;

    int countConnectedComponents(); // Returns # of connected components and assign each brick a component id using DFS
                                    // Two bricks are in the same comp if they have the same id
    void iterateBrickNeighborsAndAddEdges(const Brick&, Grid&);
    void addVertex(Brick&);
    int countNumDistinctComponents(const Brick&, const Grid&, const int); // Count the number of distinctive component
                                                                          // IDS in the 1-ring neighbors
                                                                          // of b that are different from b

    int getNumVertices() { return vertices.size(); }
    int getAdjListSize() { return adjList.size(); }
    shared_ptr<Brick> getBrickWithId(const int id) { return vertices[id]; }

private:
    vector<vector<shared_ptr<Brick>>> adjList;
    void connectedComponentsHelper(Brick& v, map<int, bool> &, const int);
};

#endif // GRAPH_H
