#pragma once
#include <maya/MPxNode.h>
#include <maya/MGlobal.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnStringData.h>

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "maya/MFloatPoint.h"
#include <random>

#include "voxelizer.h"
#include "grid.h"
#include "graph.h"

#define MNoVersionString
#define F_MAX 100
#define N 10

/// A quick function to check for maya errors
#define McheckErr(stat, msg) \
    if (MS::kSuccess != stat) { \
    cerr << msg; \
    return MS::kFailure; \
    }

struct cmpBrickIds {
    bool operator()(const Brick& a, const Brick& b) const {
        return a.getId() < b.getId();
    }
};

class BobNode: public MPxNode
{
private:
    /// creates bricks through instancing with default colors
    MStatus setupBrickDataHandles(MDataBlock& data);

    /// creates bricks through duplication with mesh colors
    MStatus createBricksWithColor();

    /// function: helper to take two bricks and replace them in the grid with a new brick
    /// newBrick: brick that will replace brick1 and brick2 in grid and have its pos, scale, and type updated
    void mergeBricks(const Brick &brick1, const Brick &brick2, Brick &newBrick, Grid& L);

    /// function: modifies an adjacency list of bricks that are mergeable, given an input set of bricks
    // bricks: input set to check adjacency on
    // adjList: adjacency list to modify given input bricks
    void updateAdjBricks(const std::set<Brick, cmpBrickIds> &bricks,
                         std::map<Brick, std::set<Brick, cmpBrickIds>,
                         cmpBrickIds> &adjList, MString &colorConstraintInput,
                         Grid&);

    /// function: merge bricks on grid until the current grid is maximal (no more bricks can merge)

    void generateInitialMaximalLayout(const std::set<Brick, cmpBrickIds> &brickSet, MString colorConstraintInput, Grid&);

    /// helper that populates colors based on mesh given a list of uvs
    void getMeshColors(const std::vector<glm::vec2> &uvs, const std::vector<MFloatPoint> &points, const MString &texture, std::vector<MColor> &colors);


    /// function: fill out a graph with the initial maximal layout
    void generateGraphFromMaximalLayout(Grid&);

    /// input: a brick layout
    /// output: structure metric sIL (aka number of connected components), critical portion wIL
    void componentAnalysis(int&, Brick&, Grid&);

    Grid layoutReconfiguration(const Grid&, const Brick&, const float, MString);

    /// Randomly merge bricks in a neighborhood set
    void randomRepeatedRemerge(map<glm::vec3, bool, cmpVec3>&, Grid&, MString);

    /// function: create a single connected component
    /// intput: the initial maximal layout
    void generateSingleConnectedComponent(MString, Grid&);
public:
    BobNode() {}
    ~BobNode() override {}
    MStatus compute(const MPlug& plug, MDataBlock& data) override;
    static  void*	creator();
    static  MStatus initialize();

    static MTypeId id;
    Grid grid;
    Graph graph;

    /// inputs
    static MObject inputMesh; /// Input mesh (already voxelized by the voxelizerNode)
    static MObject inputMeshName; /// name of the input mesh for display and use in MEL scripts
    static MObject meshTexture; /// path to the mesh texture file
    static MObject colorConstraint; /// HARD or SOFT
    static MObject useMeshColors; /// if true, iterate until max iterations or stable. otherwise, iterate once
    static MObject exportPath;
    static MObject maxLayer;
    static MObject jpgPath;

    // array attrs data for instanced bricks
    static MObject oneXoneArr;
    static MObject oneXtwoArr;
    static MObject oneXthreeArr;
    static MObject oneXfourArr;
    static MObject oneXsixArr;
    static MObject oneXeightArr;
    static MObject twoXtwoArr;
    static MObject twoXthreeArr;
    static MObject twoXfourArr;
    static MObject twoXsixArr;
    static MObject twoXeightArr;

    /// outputs
    static MObject outputMesh; /// Output stablized mesh
    static MObject stabilityStatus; /// Either stable or unstable depending on the output of refinement step


};

MTypeId BobNode::id(0x80000);
MObject BobNode::inputMesh;
MObject BobNode::inputMeshName;
MObject BobNode::meshTexture;
MObject BobNode::colorConstraint;
MObject BobNode::outputMesh;
MObject BobNode::stabilityStatus;
MObject BobNode::useMeshColors;
MObject BobNode::exportPath;
MObject BobNode::maxLayer;
MObject BobNode::jpgPath;

MObject BobNode::oneXoneArr;
MObject BobNode::oneXtwoArr;
MObject BobNode::oneXthreeArr;
MObject BobNode::oneXfourArr;
MObject BobNode::oneXsixArr;
MObject BobNode::oneXeightArr;
MObject BobNode::twoXtwoArr;
MObject BobNode::twoXthreeArr;
MObject BobNode::twoXfourArr;
MObject BobNode::twoXsixArr;
MObject BobNode::twoXeightArr;




