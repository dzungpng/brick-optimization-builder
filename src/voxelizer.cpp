#include "voxelizer.h"
#include "brick.h"

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


/**
Obtain a list of voxels as a set of (x,y,z) coordinates in the mesh local space.

We obtain these voxels by casting rays from points separated pVoxelDistance apart within the
mesh bounding box, and test whether or not these points are contained within the mesh.

A point is contained within a closed mesh if the ray shot from the point intersects an odd
number of times with the surface of the mesh.
**/
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
                /** 2D representation of a ray cast from the point within the bounding box:
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
                **/

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


void Voxelizer::createVoxelMesh(const std::vector<MFloatPoint>& pVoxelPositions, MObject& pOutMeshData, Grid& grid)
{
    // Create a mesh containing one cubic polygon for each voxel in the pVoxelPositions list.
    unsigned int numVoxels = pVoxelPositions.size();

    unsigned int numVerticesPerVoxel     = 8; // a cube has eight vertices.
    unsigned int numPolygonsPerVoxel     = 6; // a cube has six faces.
    unsigned int numVerticesPerPolygon   = 4; // four vertices are required to define a face of a cube.
    unsigned int numPolygonConnectsPerVoxel = numPolygonsPerVoxel * numVerticesPerPolygon; // 24

    // Initialize the required arrays used to create the mesh in MFnMesh.create()
    unsigned int totalVertices = numVoxels * numVerticesPerVoxel;
    MFloatPointArray vertexArray;
    vertexArray.setLength(totalVertices);
    unsigned int vertexIndexOffset = 0;

    unsigned int totalPolygons = numVoxels * numPolygonsPerVoxel;
    MIntArray polygonCounts;
    polygonCounts.setLength(totalPolygons);
    unsigned int polygonCountsIndexOffset = 0;

    unsigned int totalPolygonConnects = numVoxels * numPolygonConnectsPerVoxel;
    MIntArray polygonConnects;
    polygonConnects.setLength(totalPolygonConnects);
    unsigned int polygonConnectsIndexOffset = 0;

    // Populate the required arrays used in MFnMesh.create()
    for(unsigned int i = 0; i < numVoxels; i++) {
        MFloatPoint voxelPosition = pVoxelPositions[i];

        // Add a new cube to the arrays
        createCube(voxelPosition, vertexArray, vertexIndexOffset, numVerticesPerVoxel,
                   polygonCounts, polygonCountsIndexOffset, numPolygonsPerVoxel, numVerticesPerPolygon,
                   polygonConnects, polygonConnectsIndexOffset);


        // Increment the respective index offsets
        vertexIndexOffset += numVerticesPerVoxel;
        polygonCountsIndexOffset += numPolygonsPerVoxel;
        polygonConnectsIndexOffset += numPolygonConnectsPerVoxel;

        // Add 1x1 brick corresponding with this voxel to grid
        Brick brick = Brick();
        float halfWidth = float( defaultVoxelWidth / 2.f );
        MFloatPoint pos(-halfWidth + voxelPosition.x, -halfWidth + voxelPosition.y, -halfWidth + voxelPosition.z);
        brick.setPos(glm::vec3(pos[0], pos[1], pos[2]));
        brick.setScale(glm::vec2(1));
        grid.setBrick(brick);
    }

    // Create the mesh now that the arrays have been populated. The mesh is stored in pOutMeshData
    MFnMesh meshFn;
    MStatus status;
    meshFn.create(totalVertices, totalPolygons, vertexArray, polygonCounts, polygonConnects, pOutMeshData, &status);
    McheckErr(status, "ERROR in creating final voxel mesh in createVoxelMesh!");
}

void Voxelizer::createCube(
        const MFloatPoint pVoxelPosition,
        MFloatPointArray& pVertexArray,
        const unsigned int pVertexIndexOffset,
        const unsigned int pNumVerticesPerVoxel,
        MIntArray& pPolygonCountArray,
        const unsigned int pPolygonCountIndexOffset,
        const unsigned int pNumPolygonsPerVoxel,
        const unsigned int pNumVerticesPerPolygon,
        MIntArray& pPolygonConnectsArray,
        const unsigned int pPolygonConnectsIndexOffset) const
{
    // Add a cubic polygon to the specified arrays.

    // We are using half the given width to compute the vertices of the cube.
    float halfWidth = float( defaultVoxelWidth / 2.f );

    // Declare the eight corners of the cube. The cube is centered at pVoxelPosition.
    MFloatPoint v0(-halfWidth + pVoxelPosition.x, -halfWidth + pVoxelPosition.y, -halfWidth + pVoxelPosition.z);
    MFloatPoint v1(halfWidth + pVoxelPosition.x, -halfWidth + pVoxelPosition.y, -halfWidth + pVoxelPosition.z);
    MFloatPoint v2(halfWidth + pVoxelPosition.x, -halfWidth + pVoxelPosition.y,  halfWidth + pVoxelPosition.z);
    MFloatPoint v3(-halfWidth + pVoxelPosition.x, -halfWidth + pVoxelPosition.y,  halfWidth + pVoxelPosition.z);
    MFloatPoint v4(-halfWidth + pVoxelPosition.x,  halfWidth + pVoxelPosition.y, -halfWidth + pVoxelPosition.z);
    MFloatPoint v5(-halfWidth + pVoxelPosition.x,  halfWidth + pVoxelPosition.y,  halfWidth + pVoxelPosition.z);
    MFloatPoint v6(halfWidth + pVoxelPosition.x,  halfWidth + pVoxelPosition.y,  halfWidth + pVoxelPosition.z);
    MFloatPoint v7(halfWidth + pVoxelPosition.x,  halfWidth + pVoxelPosition.y, -halfWidth + pVoxelPosition.z);

    std::vector<MFloatPoint> vertices;
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);

    // Declare the data structure which binds each vertex to a polygon corner
    std::vector<glm::vec3> polygonConnections;
    // the vertex indexed at 0 corresponds to the polygon corners whose indexes are (0, 12, 16) in pPolygonConnectsArray.
    polygonConnections.push_back(glm::vec3(0, 12, 16));
    polygonConnections.push_back(glm::vec3(1, 19, 20));
    polygonConnections.push_back(glm::vec3(2,  9, 23));
    polygonConnections.push_back(glm::vec3(3,  8, 13));
    polygonConnections.push_back(glm::vec3(4, 15, 17));
    polygonConnections.push_back(glm::vec3(5, 11, 14));
    polygonConnections.push_back(glm::vec3(6, 10, 22));
    polygonConnections.push_back(glm::vec3(7, 18, 21));

    // Store the eight corners of the cube in the vertex array.
    for(unsigned int i = 0; i < pNumVerticesPerVoxel; i++){
        // Store the vertex in the passed vertex array.
        pVertexArray[pVertexIndexOffset + i] = vertices[i];

        // Assign the vertex in the pVertexArray to the relevant polygons.
        for(unsigned int j = 0; j < 3; j++) {
            int polygonConnectionIndex = polygonConnections[i][j];
            pPolygonConnectsArray[pPolygonConnectsIndexOffset + polygonConnectionIndex] = pVertexIndexOffset + i;
        }
    }

    // Declare the number of vertices for each face.
    for(unsigned int i = 0; i < pNumPolygonsPerVoxel; i++) {
        // Set the number of vertices for the polygon at the given index.
        pPolygonCountArray[pPolygonCountIndexOffset + i] =  pNumVerticesPerPolygon;
    }
}
