#include "bobnode.h"
#include <stdio.h>


void* BobNode::creator()
{
    return new BobNode;
}

MStatus BobNode::initialize()
{
    /// INPUT ATTRIBUTES
    MFnTypedAttribute inputMeshAttr; /// Input mesh (already voxelized by the voxelizerNode)
    MFnTypedAttribute colorContraintAttr; /// HARD or SOFT
    MFnNumericAttribute iterAttr; /// Iterations until stable

    /// OUTPUT ATTRIBUTES
    MFnTypedAttribute statusAttr; /// Either stable or unstable
    MFnTypedAttribute outputMeshAttr; /// Output stablized mesh

    statusAttr.setWritable(true);
    outputMeshAttr.setWritable(false);
    outputMeshAttr.setStorable(false);

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
    //iterAttr.setHidden(true);
    McheckErr(returnStatus, "ERROR in creating iteration attribute!\n");

    BobNode::outputMesh = outputMeshAttr.create("outputMesh", "outMesh", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating output mesh attribute!\n");

    MString defaultStatus = "Uninitialized";
    BobNode::stabilityStatus = statusAttr.create(
                "stabilityStatus", "stableStat", MFnData::kString, MFnStringData().create(defaultStatus), &returnStatus);
    McheckErr(returnStatus, "ERROR in creating stability status attribute!\n");

    /// ADD ATTRIBUTES
    returnStatus = addAttribute(BobNode::inputMesh);
    McheckErr(returnStatus, "ERROR in adding input mesh attribute!\n");

    returnStatus = addAttribute(BobNode::colorConstraint);
    McheckErr(returnStatus, "ERROR in adding color constraint attribute!\n");

    returnStatus = addAttribute(BobNode::iteration);
    McheckErr(returnStatus, "ERROR in adding iteration attribute!\n");

    returnStatus = addAttribute(BobNode::stabilityStatus);
    McheckErr(returnStatus, "ERROR in adding statbility status attribute!\n")

    returnStatus = addAttribute(BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in creating output mesh attribute!\n");

    /// ADD ATTRIBUTE AFFECTS
    returnStatus = attributeAffects(BobNode::inputMesh, BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for input mesh to output mesh!\n");

    returnStatus = attributeAffects(BobNode::inputMesh, BobNode::stabilityStatus);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for input mesh to stability status!\n");

    returnStatus = attributeAffects(BobNode::iteration, BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for iteration to output mesh!\n");

    returnStatus = attributeAffects(BobNode::iteration, BobNode::stabilityStatus);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for iteration to stability status!\n");

    returnStatus = attributeAffects(BobNode::colorConstraint, BobNode::outputMesh);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for color contraint to output mesh!\n");

    returnStatus = attributeAffects(BobNode::colorConstraint, BobNode::stabilityStatus);
    McheckErr(returnStatus, "ERROR in adding attributeAffects for color contraint to stability status!\n");

    return MS::kSuccess;
}


MStatus BobNode::compute(const MPlug& plug, MDataBlock& data)

{
    MStatus returnStatus;
    if(plug == BobNode::outputMesh) {
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

        /// VOXELIZE INPUT MESH
        Voxelizer voxelizer = Voxelizer();

        /// 1. Compute the bounding box around the mesh vertices
        MBoundingBox boundingBox = voxelizer.getBoundingBox(inputMeshObj);

        /// 2. Determine which voxel centerpoints are contained within the mesh
        std::vector<MFloatPoint> voxels = voxelizer.getVoxels(inputMeshObj, boundingBox);
//        std::string voxels_size = std::to_string(voxels.size());
//        MGlobal::displayInfo(voxels_size.c_str());


        /// 3. Create a mesh data container, which will store our new voxelized mesh
        MFnMeshData meshDataFn;
        MObject newOutputMeshData = meshDataFn.create(&returnStatus);
        McheckErr(returnStatus, "ERROR in creating voxelized output mesh data!\n");

        /// 4. Create a cubic polygon for each voxel and populate the MeshData object
        voxelizer.createVoxelMesh(voxels, newOutputMeshData);

        /// 5. Set the output data
        outputMeshHandle.setMObject(newOutputMeshData);

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

//	// code for setting up the menu items
    MString guiPath = plugin.loadPath() + MString("/brick-optimization-builder/src/BOBNodeGUI.mel");
    MGlobal::displayInfo("PATH: " + guiPath);
    MString quoteInStr = "\\\"";
    MString eval = MString("eval(\"source " + quoteInStr + guiPath + quoteInStr + "\");");
    MString menu = MString("menu - parent MayaWindow - l \"BOBNode\" BOBNode;");
    MString addNodeCmd = MString("menuItem - label \"Create BOBNode\" - parent MayaWindow|BOBNode - command \"createBOBNode()\" BOBNodeItem;");

    MString createMenu = eval + "\n" + menu + "\n" + addNodeCmd;

//    char buffer[2048];
//    sprintf_s(buffer, 2048, createMenu.asChar());
//    MGlobal::executeCommand(buffer, true);
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
