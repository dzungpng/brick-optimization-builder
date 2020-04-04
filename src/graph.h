#ifndef GRAPH_H
#define GRAPH_H

#pragma once
#include "brick.h"
#include "grid.h"
#include <map>
#include <vector>

using namespace std;

class Graph
{
public:
    Graph(const int);
    ~Graph();

    vector<shared_ptr<Brick>> vertices;

    int countConnectedComponents(); // Returns # of connected components and assign each brick a component id using DFS
                                          // Two bricks are in the same comp if they have the same id
    void iterateBrickNeighborsAndAddEdges(const Brick&, Grid&);
    void addVertex(Brick&);
    int getNumVertices() { return vertices.size(); }
    int getAdjListSize() { return adjList.size(); }

private:
    vector<vector<shared_ptr<Brick>>> adjList;
    void connectedComponentsHelper(Brick& v, map<int, bool> &, const int);
};

#endif // GRAPH_H
