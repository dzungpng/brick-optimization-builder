#pragma once
#include <maya/MSimple.h>
#include <maya/MPxNode.h>
#include <maya/MGlobal.h>
#define MNoVersionString

class BobNode: public MPxNode
{
public:
    BobNode() {}
    ~BobNode() override {}
    MStatus compute(const MPlug& plug, MDataBlock& data) override;
    static  void*	creator();
    static  MStatus initialize();

    static MTypeId	id;
};

