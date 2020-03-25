#include "bobnode.h"
#include <stdio.h>
#include <maya/MFnArrayAttrsData.h>

// helper for printing numbers
static void print(MString label, int i) {
    MString s = "";
    s += i;
    MGlobal::displayInfo(label + " " + i);
}

void* BobNode::creator()
{
    return new BobNode;
}

MStatus BobNode::initialize()
{

    // INPUT ATTRIBUTES
    MFnTypedAttribute inputMeshAttr; // Input mesh (already voxelized by the voxelizerNode)
    MFnTypedAttribute colorContraintAttr; // HARD or SOFT
    MFnNumericAttribute iterAttr; // max Iterations or until stable
    MFnNumericAttribute untilStableAttr; // bool for iterating until stable or just once

    // OUTPUT ATTRIBUTES
    MFnTypedAttribute statusAttr; // Either stable or unstable
    MFnTypedAttribute outputMeshAttr; // Output stablized mesh

    statusAttr.setWritable(true);
    outputMeshAttr.setWritable(false);
    outputMeshAttr.setStorable(false);
    outputMeshAttr.setHidden(true);

    MStatus returnStatus;

    // CREATE ATTRIBUTES
    BobNode::inputMesh = inputMeshAttr.create("inputMesh", "inMesh", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating input mesh attribute!\n");

    MString defaultColorConstraint = "HARD";
    BobNode::colorConstraint = colorContraintAttr.create(
                "colorConstraint", "col", MFnData::kString, MFnStringData().create(defaultColorConstraint), &returnStatus);
    colorContraintAttr.setHidden(false);
    McheckErr(returnStatus, "ERROR in creating color contraint attribute!\n");

    BobNode::iteration = iterAttr.create("iterations", "itr", MFnNumericData::kInt, 1, &returnStatus);
    iterAttr.setHidden(true);
    McheckErr(returnStatus, "ERROR in creating iteration attribute!\n");

    BobNode::iterateUntilStable = untilStableAttr.create("iterateUntilStable", "itrSt", MFnNumericData::kBoolean, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating iterate until stable attribute!\n");

    MString defaultStatus = "Uninitialized";
    //MString defaultStatus = "Initializing...";
    BobNode::stabilityStatus = statusAttr.create(
                "stabilityStatus", "stableStat", MFnData::kString, MFnStringData().create(defaultStatus), &returnStatus);
    McheckErr(returnStatus, "ERROR in creating stability status attribute!\n");

    BobNode::outputMesh = outputMeshAttr.create("outputMesh", "outMesh", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating output mesh attribute!\n");

    BobNode::oneXoneArr = outputMeshAttr.create("oneXone", "oo", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 1x1 bricks!\n");
    BobNode::oneXtwoArr = outputMeshAttr.create("oneXtwo", "ot", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 1x2 bricks!\n");
    BobNode::oneXthreeArr = outputMeshAttr.create("oneXthree", "otr", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 1x3 bricks!\n");
    BobNode::oneXfourArr = outputMeshAttr.create("oneXfour", "of", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 1x4 bricks!\n");
    BobNode::oneXsixArr = outputMeshAttr.create("oneXsix", "os", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 1x6 bricks!\n");
    BobNode::oneXeightArr = outputMeshAttr.create("oneXeight", "oe", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 1x8 bricks!\n");

    BobNode::twoXtwoArr = outputMeshAttr.create("twoXtwo", "tt", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 2x2 bricks!\n");
    BobNode::twoXthreeArr = outputMeshAttr.create("twoXthree", "ttr", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 2x3 bricks!\n");
    BobNode::twoXfourArr = outputMeshAttr.create("twoXfour", "tf", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 2x4 bricks!\n");
    BobNode::twoXsixArr = outputMeshAttr.create("twoXsix", "ts", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 2x6 bricks!\n");
    BobNode::twoXeightArr = outputMeshAttr.create("twoXeight", "te", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating array attr for 2x8 bricks!\n");


    // ADD ATTRIBUTES
    returnStatus = addAttribute(BobNode::inputMesh);
    McheckErr(returnStatus, "ERROR in adding input mesh attribute!\n");

    returnStatus = addAttribute(BobNode::colorConstraint);
    McheckErr(returnStatus, "ERROR in adding color constraint attribute!\n");

    returnStatus = addAttribute(BobNode::iteration);
    McheckErr(returnStatus, "ERROR in adding iteration attribute!\n");

    returnStatus = addAttribute(BobNode::iterateUntilStable);
    McheckErr(returnStatus, "ERROR in adding iterate until stable attribute!\n");

    returnStatus = addAttribute(BobNode::stabilityStatus);
    McheckErr(returnStatus, "ERROR in adding statbility status attribute!\n")

            returnStatus = addAttribute(BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in creating output mesh attribute!\n");

    returnStatus = addAttribute(BobNode::oneXoneArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::oneXtwoArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::oneXthreeArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::oneXfourArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::oneXsixArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::oneXeightArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::twoXtwoArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::twoXthreeArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::twoXfourArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::twoXsixArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");
    returnStatus = addAttribute(BobNode::twoXeightArr);
    McheckErr(returnStatus, "ERROR in creating output attribute!\n");

    // ADD ATTRIBUTE AFFECTS
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to output mesh!\n");

    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::oneXoneArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to oneXoneArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::oneXtwoArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to oneXtwoArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::oneXthreeArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to oneXthreeArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::oneXfourArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to oneXfourArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::oneXsixArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to oneXsixArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::oneXeightArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to oneXeightArr!\n");

    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::twoXtwoArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to twoXtwoArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::twoXthreeArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to twoXthreeArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::twoXfourArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to twoXfourArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::twoXsixArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to twoXsixArr!\n");
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::twoXeightArr);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to twoXeightArr!\n");

    return MS::kSuccess;
}

static bool isValidBrick(glm::vec2 scale) {
    if (scale[0] == 1 || scale[0] == 2) {
        if (scale[1] >= 1 && scale[1] <= 8 && scale[1] != 7 && scale[1] != 5) {
            return true;
        }
    }
    if (scale[1] == 1 || scale[1] == 2) {
        if (scale[0] >= 1 && scale[0] <= 8 && scale[0] != 7 && scale[0] != 5) {
            return true;
        }
    }
    return false;
}

static void addBricksAdjList(std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds> &adjList, const Brick &brick1, const Brick &brick2) {
    if (adjList.count(brick1) == 0) {
        // add brick1 as key to adjacency list
        adjList.insert(std::pair<Brick, std::set<Brick, cmpBrickIds>>(brick1, std::set<Brick, cmpBrickIds>()));
    }

    if (adjList.count(brick2) == 0) {
        // add brick2 as key to adjacency list
        adjList.insert(std::pair<Brick, std::set<Brick, cmpBrickIds>>(brick2, std::set<Brick, cmpBrickIds>()));
    }

    adjList[brick1].insert(brick2);
    adjList[brick2].insert(brick1);
}

void BobNode::updateAdjBricks(const std::set<Brick, cmpBrickIds> &bricks, std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds> &adjList) {
    for (Brick brick: bricks) {
        if (adjList.count(brick) == 0) {
            // add brick as key to adjacency list
            adjList.insert(std::pair<Brick, std::set<Brick, cmpBrickIds>>(brick, std::set<Brick, cmpBrickIds>()));
        }

        glm::vec3 pos = brick.getPos();
        glm::vec2 scale = brick.getScale();

        Brick left = grid.getBrick(glm::vec3(pos[0] - 1, pos[1], pos[2]));
        Brick right = grid.getBrick(glm::vec3(pos[0] + scale[0], pos[1], pos[2]));
        Brick front = grid.getBrick(glm::vec3(pos[0], pos[1], pos[2] + scale[1]));
        Brick back = grid.getBrick(glm::vec3(pos[0], pos[1], pos[2] - 1));

        if (left.type != EMPTY) {
            glm::vec2 leftScale = left.getScale();
            if (leftScale[1] == scale[1] && left.getPos()[2] == pos[2]) {
                // check if mergeable
                glm::vec2 newScale = glm::vec2(leftScale[0] + scale[0], scale[1]);
                if(isValidBrick(newScale)) {
                    addBricksAdjList(adjList, left, brick);
                }
            }
        }
        if (right.type != EMPTY) {
            glm::vec2 rightScale = right.getScale();
            if (rightScale[1] == scale[1] && right.getPos()[2] == pos[2]) {
                // check if mergeable
                glm::vec2 newScale = glm::vec2(rightScale[0] + scale[0], scale[1]);
                if(isValidBrick(newScale)) {
                    addBricksAdjList(adjList, right, brick);
                }
            }
        }
        if (front.type != EMPTY) {
            glm::vec2 frontScale = front.getScale();
            if (frontScale[0] == scale[0] && front.getPos()[0] == pos[0]) {
                // check if mergeable
                glm::vec2 newScale = glm::vec2(scale[0], frontScale[1] + scale[1]);
                if(isValidBrick(newScale)) {
                    addBricksAdjList(adjList, front, brick);
                }
            }
        }
        if (back.type != EMPTY) {
            glm::vec2 backScale = back.getScale();
            if (backScale[0] == scale[0] && back.getPos()[0] == pos[0]) {
                // check if mergeable
                glm::vec2 newScale = glm::vec2(scale[0], backScale[1] + scale[1]);
                if(isValidBrick(newScale)) {
                    addBricksAdjList(adjList, back, brick);
                }
            }
        }
    }

}

void BobNode::mergeBricks(const Brick &brick1, const Brick &brick2, Brick &newBrick) {
    glm::vec3 pos1 = brick1.getPos();
    glm::vec3 pos2 = brick2.getPos();
    glm::vec3 newPos = glm::vec3();
    glm::vec2 newScale = glm::vec2();
    if(pos1[0] == pos2[0]) {
        // merge along z direction
        newScale = glm::vec2(brick1.getScale()[0], brick1.getScale()[1] + brick2.getScale()[1]);
        newPos = glm::vec3(pos1[0], pos1[1], std::min(pos1[2], pos2[2]));
    } else {
        // merge along x direction
        newScale = glm::vec2(brick1.getScale()[0] + brick2.getScale()[0], brick1.getScale()[1]);
        newPos = glm::vec3(std::min(pos1[0], pos2[0]), pos1[1], pos1[2]);
    }
    // update grid
    newBrick.setPos(newPos);
    newBrick.setScale(newScale);
    newBrick.setType(BRICK);
    grid.setBrick(newBrick);
}

void BobNode::generateInitialMaximalLayout(const std::set<Brick, cmpBrickIds> &brickSet) {
    ///TODO: replace with more efficient way to get all bricks into vector (upon initialization of adjList probably)
    /// right now, use to make getting random key for adjList bc maps take O(n) time to get n^th key each time
    /// -> rather than O(n) to just init this vector and pop/push_back on queries

    std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds> adjList = std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds>();
    updateAdjBricks(brickSet, adjList);

    std::vector<Brick> bricks = std::vector<Brick>();
    for (Brick b: brickSet) {
        bricks.push_back(b);
    }

    for (std::map<Brick, std::set<Brick, cmpBrickIds>, cmpBrickIds>::iterator it=adjList.begin(); it!=adjList.end(); ++it) {
        Brick b = it->first;
        std::set<Brick, cmpBrickIds> adjBricks = it->second;
        print("BRICK ID:", b.getId());
        for(Brick b2: adjBricks) {
            print("neighbor:", b2.getId());
        }
        MGlobal::displayInfo("\n\n");
    }

    while(adjList.size() > 0) {
        int randIdx1 = std::rand() % bricks.size();
        print("RAND IDX1:", randIdx1);
        print("BRICK SET SIZE:", bricks.size());
        if(randIdx1 > bricks.size()) {
            MGlobal::displayInfo("ERROR: IDX OUT OF RANGE FOR BRICKS!!!! \n\n\n\n");
        }
        return;
        //        Brick brick1 = bricks[randIdx1];
        //        if (adjList.count(brick1) > 0) {
        ////            std::set<Brick, cmpBrickIds> adjBricks = adjList[brick1];

        ////            int randIdx2 = std::rand() % adjBricks.size();
        ////            print("RAND IDX2:", randIdx2);
        ////            print("BRICK SET SIZE:", adjBricks.size());

        ////            auto it = std::begin(adjBricks);
        ////            // 'advance' the iterator n times -> seems inefficient but there are at most 4 adjacent bricks so O(1)
        ////            std::advance(it, randIdx2);
        ////            Brick brick2 = *it;

        ////            // add new brick to grid
        ////            Brick newBrick = Brick();
        ////            mergeBricks(brick1, brick2, newBrick);

        ////            // add newBrick to adjList
        ////            updateAdjBricks({newBrick}, adjList);
        ////            // update bricks vector - I guess this isn't much better than iterating over a map
        ////            bricks.erase(bricks.begin() + randIdx1);
        ////            bricks.erase(bricks.begin() + randIdx2);
        ////            bricks.push_back(newBrick);
        //        }
    }
}

MStatus BobNode::compute(const MPlug& plug, MDataBlock& data)

{
    MStatus returnStatus;
    MGlobal::displayInfo("COMPUTE!");
    if(plug == BobNode::oneXoneArr || plug == BobNode::oneXtwoArr || plug == BobNode::oneXthreeArr || plug == BobNode::oneXfourArr
                                   || plug == BobNode::oneXsixArr || plug == BobNode::oneXeightArr || plug == BobNode::twoXtwoArr
                                   || plug == BobNode::twoXthreeArr || plug == BobNode::twoXfourArr || plug == BobNode::twoXsixArr
                                   || plug == BobNode::twoXeightArr) {

        MGlobal::displayInfo("OUTPUT MESH AFFECTED");
        // GET INPUT HANDLES
        MDataHandle inputMeshHandle = data.inputValue(BobNode::inputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting input mesh handle!\n");

        MDataHandle colorContraintHandle = data.inputValue(BobNode::colorConstraint, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting color contraint handle!\n");

        MDataHandle iterationHandle = data.inputValue(BobNode::iteration, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting iteration handle!\n");

        // GET OUTPUT HANDLES
        MDataHandle outputMeshHandle = data.outputValue(BobNode::outputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting output mesh handle!\n");

        MDataHandle stabilityStatusHandle = data.outputValue(BobNode::stabilityStatus, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting stability status handle!\n");

        // INITIALIZE INPUTS
        MString colorContraintInput = colorContraintHandle.asString();
        int iterationInput = iterationHandle.asInt();
        MObject inputMeshObj = inputMeshHandle.asMesh();

        // INITIALIZE OUTPUTS
        MString stabStatus = stabilityStatusHandle.asString();

        MObject thisNode = thisMObject();
        MFnDependencyNode fnNode(thisNode);
        MString nodeName = fnNode.name();

        if (stabStatus == MString("Initializing...")) {


            // VOXELIZE INPUT MESH
            Voxelizer voxelizer = Voxelizer();

            // 1. Compute the bounding box around the mesh vertices
            MBoundingBox boundingBox = voxelizer.getBoundingBox(inputMeshObj);

            // Initialize voxel grid
            grid.initialize(boundingBox);

            // 2. Determine which voxel centerpoints are contained within the mesh
            std::vector<MFloatPoint> voxels = voxelizer.getVoxels(inputMeshObj, boundingBox);

            // 3. Create a mesh data container, which will store our new voxelized mesh
            MFnMeshData meshDataFn;
            MObject newOutputMeshData = meshDataFn.create(&returnStatus);
            McheckErr(returnStatus, "ERROR in creating voxelized output mesh data!\n");

            // 4. Create a cubic polygon for each voxel and populate the MeshData object
            voxelizer.createVoxelMesh(voxels, newOutputMeshData, grid);

            // inefficient. may need to rework data structure usage
            std::set<Brick, cmpBrickIds> brickSet = std::set<Brick, cmpBrickIds>();
            for (std::map<int, Brick>::iterator it=grid.allBricks.begin(); it!=grid.allBricks.end(); ++it) {
                Brick b = it->second;
                MString id = "";
                id += b.getId();
                MGlobal::displayInfo("ID " + id);
                brickSet.insert(b);
            }
            generateInitialMaximalLayout(brickSet);

            // TEST: uncomment this if we want to test voxels
            // outputMeshHandle.setMObject(newOutputMeshData);

            /// TEST CODE FOR DIFFERENT BRICK TYPES
            ///
            ///

            //            MBoundingBox boundingBox = MBoundingBox(MPoint(0, 0, 0), MPoint(15, 4, 15));
            //            grid.initialize(boundingBox);
            //            Brick brick1 = Brick(glm::vec3(0, 4, 0), BRICK, glm::vec2(1, 1));
            //            Brick brick2 = Brick(glm::vec3(1, 4, 0), BRICK, glm::vec2(1, 2));
            //            Brick brick3 = Brick(glm::vec3(2, 4, 0), BRICK, glm::vec2(1, 3));
            //            Brick brick4 = Brick(glm::vec3(3, 4, 0), BRICK, glm::vec2(1, 4));
            //            Brick brick5 = Brick(glm::vec3(4, 4, 0), BRICK, glm::vec2(1, 6));
            //            Brick brick6 = Brick(glm::vec3(5, 4, 0), BRICK, glm::vec2(1, 8));
            //            grid.setBrick(brick1);
            //            grid.setBrick(brick2);
            //            grid.setBrick(brick3);
            //            grid.setBrick(brick4);
            //            grid.setBrick(brick5);
            //            grid.setBrick(brick6);

            //            Brick brick7 = Brick(glm::vec3(5, 4, 0), BRICK, glm::vec2(1, 1));
            //            Brick brick8 = Brick(glm::vec3(5, 4, 1), BRICK, glm::vec2(2, 1));
            //            Brick brick9 = Brick(glm::vec3(5, 4, 2), BRICK, glm::vec2(3, 1));
            //            Brick brick10 = Brick(glm::vec3(5, 4, 3), BRICK, glm::vec2(4, 1));
            //            Brick brick11 = Brick(glm::vec3(5, 4, 4), BRICK, glm::vec2(6, 1));
            //            Brick brick12 = Brick(glm::vec3(5, 4, 5), BRICK, glm::vec2(8, 1));
            //            grid.setBrick(brick7);
            //            grid.setBrick(brick8);
            //            grid.setBrick(brick9);
            //            grid.setBrick(brick10);
            //            grid.setBrick(brick11);
            //            grid.setBrick(brick12);
            ///
            ///
            ///
            ///
            returnStatus = setupBrickDataHandles(data);

            /// code for updating node gui
            // set status to "Initialized"
            MGlobal::displayInfo("INIT");
            MPlug stabilityPlug = fnNode.findPlug("stabilityStatus");
            stabilityPlug.setString("Initialized");
            MGlobal::executeCommand("setAttr -type \"string\" " + nodeName + ".stabilityStatus \"Initialized\";");

        } else if (stabStatus == MString("Computing...")) {
            MGlobal::displayInfo("COMPUTING");
            // computing code - num iterations based on iterateUntilStable attr
            // set status to "Stable" or "Unstable" based on analysis
            // lock iterate button if mesh is stable
            // NOTE; do we want to also keep track of a number to just show us how many iterations we've performed?

            MPlug stabilityPlug = fnNode.findPlug("stabilityStatus");
            stabilityPlug.setString("Stable");
            // re-enable the iterate button - UNLESS layout is already stable
            MGlobal::executeCommand("button -e -enable true IterateButton;");
        } else {
            MGlobal::displayInfo("OTHER STABILITY STATUS");
        }
        //TODO: generateSingleConnectedComponent using mesh, interationInput, and colorContraintInput

        return MS::kSuccess;
    }
    return MS::kFailure;
}


MStatus BobNode::setupBrickDataHandles(MDataBlock& data) {
    MStatus returnStatus;
    MGlobal::displayInfo("SET UP DATA HANDLES");
    // STEP 1: GET OUTPUT HANDLES
    MDataHandle oneXoneDataHandle = data.outputValue(BobNode::oneXoneArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting oneXone handle");

    MDataHandle oneXtwoDataHandle = data.outputValue(BobNode::oneXtwoArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting oneXtwo handle");

    MDataHandle oneXthreeDataHandle = data.outputValue(BobNode::oneXthreeArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting oneXthree handle");

    MDataHandle oneXfourDataHandle = data.outputValue(BobNode::oneXfourArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting oneXfour handle");

    MDataHandle oneXsixDataHandle = data.outputValue(BobNode::oneXsixArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting oneXsize handle");

    MDataHandle oneXeightDataHandle = data.outputValue(BobNode::oneXeightArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting oneXeight handle");

    MDataHandle twoXtwoDataHandle = data.outputValue(BobNode::twoXtwoArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting twoXtwo handle");

    MDataHandle twoXthreeDataHandle = data.outputValue(BobNode::twoXthreeArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting twoXthree handle");

    MDataHandle twoXfourDataHandle = data.outputValue(BobNode::twoXfourArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting twoXfour handle");

    MDataHandle twoXsixDataHandle = data.outputValue(BobNode::twoXsixArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting twoXsize handle");

    MDataHandle twoXeightDataHandle = data.outputValue(BobNode::twoXeightArr, &returnStatus);
    McheckErr(returnStatus, "ERROR in getting twoXeight handle");


    // STEP 2: SETUP ATTRS DATA ARRAYS

    /// 1x1 bricks
    MFnArrayAttrsData oneXoneAAD;
    MObject oneXoneObject = oneXoneAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x1 object!\n");
    MVectorArray oneXonePositionArray = oneXoneAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x1 position array!\n");
    MDoubleArray oneXoneIdArray = oneXoneAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x1 id array!\n");

    /// 1x2 bricks
    MFnArrayAttrsData oneXtwoAAD;
    MObject oneXtwoObject = oneXtwoAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x2 object!\n");
    MVectorArray oneXtwoPositionArray = oneXtwoAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x2 position array!\n");
    MVectorArray oneXtwoRotationArray = oneXtwoAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x2 rotation array!\n");
    MVectorArray oneXtwoScaleArray = oneXtwoAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x2 scale array!\n");
    MDoubleArray oneXtwoIdArray = oneXtwoAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x2 id array!\n");

    /// 1x3 bricks
    MFnArrayAttrsData oneXthreeAAD;
    MObject oneXthreeObject = oneXthreeAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x3 object!\n");
    MVectorArray oneXthreePositionArray = oneXthreeAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x3 position array!\n");
    MVectorArray oneXthreeRotationArray = oneXthreeAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x3 rotation array!\n");
    MVectorArray oneXthreeScaleArray = oneXthreeAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x3 scale array!\n");
    MDoubleArray oneXthreeIdArray = oneXthreeAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x3 id array!\n");

    /// 1x4 bricks
    MFnArrayAttrsData oneXfourAAD;
    MObject oneXfourObject = oneXfourAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x4 object!\n");
    MVectorArray oneXfourPositionArray = oneXfourAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x4 position array!\n");
    MVectorArray oneXfourRotationArray = oneXfourAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x4 rotation array!\n");
    MVectorArray oneXfourScaleArray = oneXfourAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x4 scale array!\n");
    MDoubleArray oneXfourIdArray = oneXfourAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x4 id array!\n");

    /// 1x6 bricks
    MFnArrayAttrsData oneXsixAAD;
    MObject oneXsixObject = oneXsixAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x6 object!\n");
    MVectorArray oneXsixPositionArray = oneXsixAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x6 position array!\n");
    MVectorArray oneXsixRotationArray = oneXsixAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x6 rotation array!\n");
    MVectorArray oneXsixScaleArray = oneXsixAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x6 scale array!\n");
    MDoubleArray oneXsixIdArray = oneXsixAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x6 id array!\n");

    /// 1x8 bricks
    MFnArrayAttrsData oneXeightAAD;
    MObject oneXeightObject = oneXeightAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x8 object!\n");
    MVectorArray oneXeightPositionArray = oneXeightAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x8 position array!\n");
    MVectorArray oneXeightRotationArray = oneXeightAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x8 rotation array!\n");
    MVectorArray oneXeightScaleArray = oneXeightAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x8 scale array!\n");
    MDoubleArray oneXeightIdArray = oneXeightAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 1x8 id array!\n");


    /// 2x2 bricks
    MFnArrayAttrsData twoXtwoAAD;
    MObject twoXtwoObject = twoXtwoAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x2 object!\n");
    MVectorArray twoXtwoPositionArray = twoXtwoAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x2 position array!\n");
    MVectorArray twoXtwoRotationArray = twoXtwoAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x2 rotation array!\n");
    MVectorArray twoXtwoScaleArray = twoXtwoAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x2 scale array!\n");
    MDoubleArray twoXtwoIdArray = twoXtwoAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x2 id array!\n");

    /// 2x3 bricks
    MFnArrayAttrsData twoXthreeAAD;
    MObject twoXthreeObject = twoXthreeAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x3 object!\n");
    MVectorArray twoXthreePositionArray = twoXthreeAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x3 position array!\n");
    MVectorArray twoXthreeRotationArray = twoXthreeAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x3 rotation array!\n");
    MVectorArray twoXthreeScaleArray = twoXthreeAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x3 scale array!\n");
    MDoubleArray twoXthreeIdArray = twoXthreeAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x3 id array!\n");

    /// 2x4 bricks
    MFnArrayAttrsData twoXfourAAD;
    MObject twoXfourObject = twoXfourAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x4 object!\n");
    MVectorArray twoXfourPositionArray = twoXfourAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x4 position array!\n");
    MVectorArray twoXfourRotationArray = twoXfourAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x4 rotation array!\n");
    MVectorArray twoXfourScaleArray = twoXfourAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x4 scale array!\n");
    MDoubleArray twoXfourIdArray = twoXfourAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x4 id array!\n");

    /// 2x6 bricks
    MFnArrayAttrsData twoXsixAAD;
    MObject twoXsixObject = twoXsixAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x6 object!\n");
    MVectorArray twoXsixPositionArray = twoXsixAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x6 position array!\n");
    MVectorArray twoXsixRotationArray = twoXsixAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x6 rotation array!\n");
    MVectorArray twoXsixScaleArray = twoXsixAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x6 scale array!\n");
    MDoubleArray twoXsixIdArray = twoXsixAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x6 id array!\n");

    /// 2x8 bricks
    MFnArrayAttrsData twoXeightAAD;
    MObject twoXeightObject = twoXeightAAD.create(&returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x8 object!\n");
    MVectorArray twoXeightPositionArray = twoXeightAAD.vectorArray("position", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x8 position array!\n");
    MVectorArray twoXeightRotationArray = twoXeightAAD.vectorArray("rotation", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x8 rotation array!\n");
    MVectorArray twoXeightScaleArray = twoXeightAAD.vectorArray("scale", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x8 scale array!\n");
    MDoubleArray twoXeightIdArray = twoXeightAAD.doubleArray("id", &returnStatus);
    McheckErr(returnStatus, "ERROR in creating 2x8 id array!\n");

    // STEP 3: POPULATE ARRAYS
    for (std::map<int, Brick>::iterator it=grid.allBricks.begin(); it!=grid.allBricks.end(); ++it) {
        Brick b = it->second;
        if(b.type != EMPTY) {
            glm::vec3 brickPos = b.getPos();
            glm::vec2 brickScale = b.getScale();
            if (brickScale[0] == 1 && brickScale[1] == 1) {
                oneXonePositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXoneIdArray.append(b.getId());
            } else if ((brickScale[0] == 1 && brickScale[1] == 2) ||(brickScale[0] == 2 && brickScale[1] == 1)) {
                oneXtwoPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXtwoIdArray.append(b.getId());
                if (brickScale[1] == 1) {
                    oneXtwoRotationArray.append(MVector(0, 90, 0));
                    oneXtwoScaleArray.append(MVector(-1, 1, 1));
                } else {
                    oneXtwoRotationArray.append(MVector(0, 0, 0));
                    oneXtwoScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 1 && brickScale[1] == 3) ||(brickScale[0] == 3 && brickScale[1] == 1)) {
                oneXthreePositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXthreeIdArray.append(b.getId());
                if (brickScale[1] == 1) {
                    oneXthreeRotationArray.append(MVector(0, 90, 0));
                    oneXthreeScaleArray.append(MVector(-1, 1, 1));
                } else {
                    oneXthreeRotationArray.append(MVector(0, 0, 0));
                    oneXthreeScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 1 && brickScale[1] == 4) ||(brickScale[0] == 4 && brickScale[1] == 1)) {
                oneXfourPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXfourIdArray.append(b.getId());
                if (brickScale[1] == 1) {
                    oneXfourRotationArray.append(MVector(0, 90, 0));
                    oneXfourScaleArray.append(MVector(-1, 1, 1));
                } else {
                    oneXfourRotationArray.append(MVector(0, 0, 0));
                    oneXfourScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 1 && brickScale[1] == 6) ||(brickScale[0] == 6 && brickScale[1] == 1)) {
                oneXsixPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXsixIdArray.append(b.getId());
                if (brickScale[1] == 1) {
                    oneXsixRotationArray.append(MVector(0, 90, 0));
                    oneXsixScaleArray.append(MVector(-1, 1, 1));
                } else {
                    oneXsixRotationArray.append(MVector(0, 0, 0));
                    oneXsixScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 1 && brickScale[1] == 8) ||(brickScale[0] == 8 && brickScale[1] == 1)) {
                oneXeightPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXeightIdArray.append(b.getId());
                if (brickScale[1] == 1) {
                    oneXeightRotationArray.append(MVector(0, 90, 0));
                    oneXeightScaleArray.append(MVector(-1, 1, 1));
                } else {
                    oneXeightRotationArray.append(MVector(0, 0, 0));
                    oneXeightScaleArray.append(MVector(1, 1, 1));
                }
            }

            // 2x_ bricks
            else if (brickScale[0] == 2 && brickScale[1] == 2) {
                oneXtwoPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                oneXtwoIdArray.append(b.getId());
            } else if ((brickScale[0] == 2 && brickScale[1] == 3) ||(brickScale[0] == 3 && brickScale[1] == 2)) {
                twoXthreePositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                twoXthreeIdArray.append(b.getId());
                if (brickScale[1] == 2) {
                    twoXthreeRotationArray.append(MVector(0, 90, 0));
                    twoXthreeScaleArray.append(MVector(-1, 1, 1));
                } else {
                    twoXthreeRotationArray.append(MVector(0, 0, 0));
                    twoXthreeScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 2 && brickScale[1] == 4) ||(brickScale[0] == 4 && brickScale[1] == 2)) {
                twoXfourPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                twoXfourIdArray.append(b.getId());
                if (brickScale[1] == 2) {
                    twoXfourRotationArray.append(MVector(0, 90, 0));
                    twoXfourScaleArray.append(MVector(-1, 1, 1));
                } else {
                    twoXfourRotationArray.append(MVector(0, 0, 0));
                    twoXfourScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 2 && brickScale[1] == 6) ||(brickScale[0] == 6 && brickScale[1] == 2)) {
                twoXsixPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                twoXsixIdArray.append(b.getId());
                if (brickScale[1] == 2) {
                    twoXsixRotationArray.append(MVector(0, 90, 0));
                    twoXsixScaleArray.append(MVector(-1, 1, 1));
                } else {
                    twoXsixRotationArray.append(MVector(0, 0, 0));
                    twoXsixScaleArray.append(MVector(1, 1, 1));
                }
            } else if ((brickScale[0] == 2 && brickScale[1] == 8) ||(brickScale[0] == 8 && brickScale[1] == 2)) {
                twoXeightPositionArray.append(MVector(brickPos.x, brickPos.y, brickPos.z));
                twoXeightIdArray.append(b.getId());
                if (brickScale[1] == 2) {
                    twoXeightRotationArray.append(MVector(0, 90, 0));
                    twoXeightScaleArray.append(MVector(-1, 1, 1));
                } else {
                    twoXeightRotationArray.append(MVector(0, 0, 0));
                    twoXeightScaleArray.append(MVector(1, 1, 1));
                }
            }
        }
    }

    print("ALL BRICKS SIZE:", grid.allBricks.size());
    print("1x1 array size:", oneXonePositionArray.length());

    oneXoneDataHandle.setMObject(oneXoneObject);
    oneXtwoDataHandle.setMObject(oneXtwoObject);
    oneXthreeDataHandle.setMObject(oneXthreeObject);
    oneXfourDataHandle.setMObject(oneXfourObject);
    oneXsixDataHandle.setMObject(oneXsixObject);
    oneXeightDataHandle.setMObject(oneXeightObject);

    twoXtwoDataHandle.setMObject(twoXtwoObject);
    twoXthreeDataHandle.setMObject(twoXthreeObject);
    twoXfourDataHandle.setMObject(twoXfourObject);
    twoXsixDataHandle.setMObject(twoXsixObject);
    twoXeightDataHandle.setMObject(twoXeightObject);

    return returnStatus;
}

// code to initialize the plugin //
MStatus initializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj, "MyPlugin", "1.0", "Any");

    status = plugin.registerNode("BOBNode", BobNode::id,
                                 BobNode::creator, BobNode::initialize);
    if (!status) {
        MGlobal::displayInfo("ERROR INIT NODE \n\n\n");
        status.perror("registerNode");
        return status;
    }

    // code for setting up the menu items
    //    MString guiPath = plugin.loadPath() + MString("/brick-optimization-builder/src/BOBNodeGUI.mel");
    MString guiPath = MString("/Users/kathrynmiller/Documents/MayaPlugins/BOBPlugin/brick-optimization-builder/src/BOBNodeGUI.mel");
    //    MString guiPath = MString("/Users/dzungnguyen/OneDrive - PennO365/classes/cis660/brick-optimization-builder/src/BOBNodeGUI.mel");

    MGlobal::displayInfo("PATH: " + guiPath);
    MString quoteInStr = "\\\"";
    MString eval = MString("eval(\"source " + quoteInStr + guiPath + quoteInStr + "\");");
    MString menu = MString("menu - parent MayaWindow - l \"BOBNode\" BOBNode;");
    MString addNodeCmd = MString("menuItem - label \"Create BOBNode\" - parent MayaWindow|BOBNode - command \"createBOBNode()\" BOBNodeItem;");

    MString createMenu = eval + "\n" + menu + "\n" + addNodeCmd;

    MGlobal::executeCommand(createMenu.asChar());

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj );

    status = plugin.deregisterNode(BobNode::id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}
