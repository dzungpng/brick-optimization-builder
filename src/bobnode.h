#pragma once
#include <maya/MPxNode.h>
#include <maya/MGlobal.h>

#include <maya/MFnMeshData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnStringData.h>

#include "grid.h"

#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include "voxelizer.h"

#define MNoVersionString

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

    MStatus setupBrickDataHandles(MDataBlock& data);

    /// function: helper to take two bricks and replace them in the grid with a new brick
    /// newBrick: brick that will replace brick1 and brick2 in grid and have its pos, scale, and type updated
    void mergeBricks(Brick brick1, Brick brick2, Brick &newBrick);

    /// function: modifies an adjacency list of bricks that are mergeable, given an input set of bricks
    // bricks: input set to check adjacency on
    // adjList: adjacency list to modify given input bricks
    void updateAdjBricks(std::set<Brick, cmpBrickIds> bricks, std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds> &adjList);

    /// function: merge bricks on grid until the current grid is maximal (no more bricks can merge)
    void generateInitialMaximalLayout(std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds> &adjList);
public:
    BobNode() {}
    ~BobNode() override {}
    MStatus compute(const MPlug& plug, MDataBlock& data) override;
    static  void*	creator();
    static  MStatus initialize();

    static MTypeId id;
    Grid grid;

    /// inputs
    static MObject inputMesh; /// Input mesh (already voxelized by the voxelizerNode)
    static MObject iteration; /// Iterations until stable
    static MObject colorConstraint; /// HARD or SOFT
    static MObject iterateUntilStable; // if true, iterate until max iterations or stable. otherwise, iterate once

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
MObject BobNode::iteration;
MObject BobNode::colorConstraint;
MObject BobNode::outputMesh;
MObject BobNode::stabilityStatus;
MObject BobNode::iterateUntilStable;

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




