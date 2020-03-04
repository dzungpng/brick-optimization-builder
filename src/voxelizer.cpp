#include "voxelizer.h"

Voxelizer::Voxelizer() : defaultVoxelWidth(1.f), defaultVoxelDistance(1.f) {}


Voxelizer::Voxelizer(float voxelWidth, float voxelDistance) :
    defaultVoxelWidth(voxelWidth), defaultVoxelDistance(voxelDistance) {}


Voxelizer::~Voxelizer() {}


// Compute the bounding box around the mesh vertices
MBoundingBox Voxelizer::getBoundingBox(const MObject& pMeshObj) const
{
    // Create the bounding box object we will populate with the points of the mesh.
    MBoundingBox boundingBox = MBoundingBox();

    MFnMesh meshFn(pMeshObj);

    // Get the points of the mesh in its local coordinate space
    MPointArray pointArray;
    meshFn.getPoints(pointArray, MSpace::kTransform);

    for(unsigned int i = 0; i < pointArray.length(); i++) {
        MPoint point = pointArray[i];
        boundingBox.expand(point);
    }
    return boundingBox;
}


/*
Obtain a list of voxels as a set of (x,y,z) coordinates in the mesh local space.

We obtain these voxels by casting rays from points separated pVoxelDistance apart within the
mesh bounding box, and test whether or not these points are contained within the mesh.

A point is contained within a closed mesh if the ray shot from the point intersects an odd
number of times with the surface of the mesh.
*/
std::vector<MFloatPoint> Voxelizer::getVoxels(const MObject& pMeshObj, const MBoundingBox& pBoundingBox) const
{

    // Initialize a list of voxels contained within the mesh.
    std::vector<MFloatPoint> voxels;

    // Get a reference to the MFnMesh function set, and use it on the given mesh object.
    MFnMesh meshFn(pMeshObj);

    // Compute an offset which we will apply to the min and max corners of the bounding box.
    float halfVoxelDist = 0.5f * defaultVoxelDistance;

    // Offset the position of the minimum point to account for the inter-voxel distance.
    MPoint minPoint = pBoundingBox.min();
    minPoint.x += halfVoxelDist;
    minPoint.y += halfVoxelDist;
    minPoint.z += halfVoxelDist;

    // Offset the position of the maximum point to account for the inter-voxel distance.
    MPoint maxPoint = pBoundingBox.max();
    maxPoint.x += halfVoxelDist;
    maxPoint.y += halfVoxelDist;
    maxPoint.z += halfVoxelDist;


    // Iterate over every point in the bounding box, stepping by pVoxelDistance...
    for(float xCoord = minPoint.x; xCoord < maxPoint.x; xCoord += defaultVoxelDistance) {
        for(float yCoord = minPoint.y; yCoord < maxPoint.y; yCoord += defaultVoxelDistance) {
            for(float zCoord = minPoint.z; zCoord < maxPoint.z; zCoord += defaultVoxelDistance) {
                /* 2D representation of a ray cast from the point within the bounding box:
                #
                #  (+) ^-----------------
                #      |                |
                #  y   |                |  - We are shooting the ray from the point: [*]
                # axis | <======[*]     |  - The direction of the ray is parallel to the -Z axis.
                #      |                |
                #      |                |
                #  (-) ------------------>
                #     (-)    z axis     (+)
                #
                # If the ray intersects with an odd number of points along the surface of the mesh, the
                # point is contained within the mesh (assuming a closed mesh).
                */

                MFloatPoint raySource(xCoord, yCoord, zCoord);
                MFloatVector rayDirection(0.f, 0.f, -1.f);

                float tolerance = 1e-4f;

                MFloatPointArray hitPoints;
                MStatus status;
                bool hit = meshFn.allIntersections(
                            raySource,
                            rayDirection,
                            NULL, // faceIds
                            NULL, // triIds
                            false, // idsSorted
                            MSpace::kTransform, // space = the mesh's local coordinate space
                            float(9999), // maxParam = the range of the ray
                            false, // testBothDirections = we are not checking both directions from the raySouce
                            NULL, // accelParams
                            false, // sortHits
                            hitPoints, // hitPoint = number of intersection points calculated
                            NULL, // hitRayParam
                            NULL, // hitFace
                            NULL, // hitTriangle
                            NULL, // hitBary1
                            NULL, // hitBary2
                            tolerance, // tolerance = a numberic tolerance threshold which allow intersections to occur just outside the mesh
                            &status);
                McheckErr(status, "ERROR in getting hit points!\n");


                // If there is an odd number of intersection points, then the point lies within the mesh. Otherwise,
                // the point lies outside the mesh. We are only concerned with voxels whose centerpoint lies within the mesh
                if(hit && hitPoints.length() % 2 == 1) {
                    voxels.push_back(raySource);
                }
            }
        }
    }

    // return the list of voxel coordinates which lie within the mesh
    return voxels;
}
