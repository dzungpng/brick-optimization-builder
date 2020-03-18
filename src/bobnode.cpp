#include "bobnode.h"
#include <stdio.h>
#include <maya/MFnArrayAttrsData.h>


void* BobNode::creator()
{
    return new BobNode;
}

MStatus BobNode::initialize()
{

    /// INPUT ATTRIBUTES
    MFnTypedAttribute inputMeshAttr; /// Input mesh (already voxelized by the voxelizerNode)
    MFnTypedAttribute colorContraintAttr; /// HARD or SOFT
    MFnNumericAttribute iterAttr; /// max Iterations or until stable
    MFnNumericAttribute untilStableAttr; /// bool for iterating until stable or just once

    /// OUTPUT ATTRIBUTES
    MFnTypedAttribute statusAttr; /// Either stable or unstable
    MFnTypedAttribute outputMeshAttr; /// Output stablized mesh

    statusAttr.setWritable(true);
    outputMeshAttr.setWritable(false);
    outputMeshAttr.setStorable(false);
    outputMeshAttr.setHidden(true);

    MStatus returnStatus;

    /// CREATE ATTRIBUTES
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

    MString defaultStatus = "Initializing...";
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


    /// ADD ATTRIBUTES
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

    /// ADD ATTRIBUTE AFFECTS
    returnStatus = attributeAffects(BobNode::stabilityStatus, BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to output mesh!\n");

    returnStatus = attributeAffects(BobNode::iterateUntilStable, BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for stability status to output mesh!\n");


    return MS::kSuccess;
}


MStatus BobNode::compute(const MPlug& plug, MDataBlock& data)

{
    MStatus returnStatus;
    MGlobal::displayInfo("COMPUTE!");
    if(plug == BobNode::outputMesh) {
        MGlobal::displayInfo("OUTPUT MESH AFFECTED");
        /// GET INPUT HANDLES
        MDataHandle inputMeshHandle = data.inputValue(BobNode::inputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting input mesh handle!\n");

        MDataHandle colorContraintHandle = data.inputValue(BobNode::colorConstraint, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting color contraint handle!\n");

        MDataHandle iterationHandle = data.inputValue(BobNode::iteration, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting iteration handle!\n");

        /// GET OUTPUT HANDLES
        MDataHandle outputMeshHandle = data.outputValue(BobNode::outputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting output mesh handle!\n");

        MDataHandle stabilityStatusHandle = data.outputValue(BobNode::stabilityStatus, &returnStatus);
        McheckErr(returnStatus, "ERROR in getting stability status handle!\n");

        /// INITIALIZE INPUTS
        MString colorContraintInput = colorContraintHandle.asString();
        int iterationInput = iterationHandle.asInt();
        MObject inputMeshObj = inputMeshHandle.asMesh();

        MString stabStatus = stabilityStatusHandle.asString();


        MObject thisNode = thisMObject();
        MFnDependencyNode fnNode(thisNode);
        MString nodeName = fnNode.name();

        if (stabStatus == MString("Initializing...")) {

            /// VOXELIZE INPUT MESH
            Voxelizer voxelizer = Voxelizer();

            /// 1. Compute the bounding box around the mesh vertices
            MBoundingBox boundingBox = voxelizer.getBoundingBox(inputMeshObj);

            if (grid ==  nullptr) {
                // initialize grid to mesh dimensions
                grid = new Grid(glm::vec3(boundingBox.max()[0] - boundingBox.min()[0],
                                          boundingBox.max()[1] - boundingBox.min()[1],
                                          boundingBox.max()[2] - boundingBox.min()[2]),
                                          glm::vec3());
            }

            /// 2. Determine which voxel centerpoints are contained within the mesh
            std::vector<MFloatPoint> voxels = voxelizer.getVoxels(inputMeshObj, boundingBox);

            /// 3. Create a mesh data container, which will store our new voxelized mesh
            MFnMeshData meshDataFn;
            MObject newOutputMeshData = meshDataFn.create(&returnStatus);
            McheckErr(returnStatus, "ERROR in creating voxelized output mesh data!\n");

            /// 4. Create a cubic polygon for each voxel and populate the MeshData object
            voxelizer.createVoxelMesh(voxels, newOutputMeshData);

            /// 5. Set the output data
            outputMeshHandle.setMObject(newOutputMeshData);

            // run initialization code
            // set status to "Initialized"
            MGlobal::displayInfo("INIT");
            MPlug stabilityPlug = fnNode.findPlug("stabilityStatus");
            stabilityPlug.setString("Initialized");
            //MGlobal::executeCommand("setAttr -type \"string\" " + nodeName + ".stabilityStatus \"Initialized\";");

        } else if (stabStatus == MString("Computing...")) {
            MGlobal::displayInfo("COMPUTING");
            // computing code - num iterations based on iterateUntilStable attr
            // set status to "Stable" or "Unstable" based on analysis
            // lock iterate button if mesh is stable
            /// NOTE; do we want to also keep track of a number to just show us how many iterations we've performed?

            MPlug stabilityPlug = fnNode.findPlug("stabilityStatus");
            stabilityPlug.setString("Stable");
            // re-enable the iterate button - UNLESS layout is already stable
            MGlobal::executeCommand("button -e -enable true IterateButton;");
        } else {
            MGlobal::displayInfo("OTHER STABILITY STATUS");
        }


        ///TODO: generateSingleConnectedComponent using mesh, interationInput, and colorContraintInput

        return MS::kSuccess;
    }
    return MS::kFailure;
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
    MString guiPath = MString("/Users/dzungnguyen/OneDrive - PennO365/classes/cis660/brick-optimization-builder/src/BOBNodeGUI.mel");
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
