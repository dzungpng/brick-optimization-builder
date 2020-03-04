// C++ Implementation of voxelixerNode.py from Maya Developer Help
// Source: https://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__files_GUID_767D6572_552D_4D94_90E2_AE626F276D37_htm

#ifndef VOXELIZER_H
#define VOXELIZER_H

#pragma once
#include <maya/MBoundingBox.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MTypes.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPointArray.h>

#include <vector>


// A quick function to check for maya errors
#define McheckErr(stat, msg) \
    if (MS::kSuccess != stat) { \
        cerr << msg; \
    }

class Voxelizer
{

public:
    Voxelizer();
    Voxelizer(float voxelWidth, float voxelDistance);
    ~Voxelizer();


    MBoundingBox getBoundingBox(const MObject&) const;
    std::vector<MFloatPoint> getVoxels(const MObject&, const MBoundingBox&) const;


private:
    float defaultVoxelWidth; // the width of a cubic voxel
    float defaultVoxelDistance; // the distance which separates the center of two adjacent voxels

};

#endif // VOXELIZER_H
