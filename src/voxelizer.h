// C++ Implementation of voxelixerNode.py from Maya Developer Help
// Source: https://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__files_GUID_767D6572_552D_4D94_90E2_AE626F276D37_htm

#pragma once
#include <maya/MBoundingBox.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MTypes.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <grid.h>
#include "maya/MGlobal.h"

#include <vector>
#include <include/glm/vec3.hpp>
#include "maya/MObject.h"
#include "maya/MFnDependencyNode.h"
#include "maya/MFnSet.h"

/// A quick function to check for maya errors
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
    void getVoxels(const MObject&, const MBoundingBox&, std::vector<MFloatPoint>&, std::vector<MColor>&);
    void createVoxelMesh(const std::vector<MFloatPoint>&, const std::vector<MColor> &colors, MObject&, Grid&);

    MObject findShader(const Autodesk::Maya::OpenMaya20180000::MObject &mesh);
private:


    float defaultVoxelWidth; /// the width of a cubic voxel
    float defaultVoxelDistance; /// the distance which separates the center of two adjacent voxels

    void createCube(const MFloatPoint,
                    MFloatPointArray&,
                    const unsigned int,
                    const unsigned int,
                    MIntArray&,
                    const unsigned int,
                    const unsigned int,
                    const unsigned int,
                    MIntArray&,
                    const unsigned int) const;

};
