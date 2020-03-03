#pragma once
#include <maya/MSimple.h>
#include <maya/MPxNode.h>
#include <maya/MGlobal.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnStringData.h>

#include <iostream>
#include <fstream>
#include <string>

#define MNoVersionString

// A quick function to check for maya errors
#define McheckErr(stat, msg) \
    if (MS::kSuccess != stat) { \
        cerr << msg; \
        return MS::kFailure; \
    }

class BobNode: public MPxNode
{
public:
    BobNode() {}
    ~BobNode() override {}
    MStatus compute(const MPlug& plug, MDataBlock& data) override;
    static  void*	creator();
    static  MStatus initialize();

    static MTypeId id;

    // inputs
    static MObject inputMesh;
    static MObject iteration;
    static MObject colorConstraint;

    // outputs
    static MObject outputMesh;
    static MObject stabilityStatus;
};

MTypeId BobNode::id(0x80000);
MObject BobNode::inputMesh;
MObject BobNode::iteration;
MObject BobNode::colorConstraint;
MObject BobNode::outputMesh;
MObject BobNode::stabilityStatus;





