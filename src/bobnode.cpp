#include "bobnode.h"

void* BobNode::creator()
{
    return new BobNode;
}

MStatus BobNode::initialize()
{
    // INPUT ATTRIBUTES
    MFnTypedAttribute inputMeshAttr; // Input mesh to be voxelized
    MFnTypedAttribute colorContraintAttr; // HARD or SOFT
    MFnNumericAttribute iterAttr; // Iterations until stable

    // OUTPUT ATTRIBUTES
    MFnTypedAttribute statusAttr; // Either stable or unstable
    MFnTypedAttribute outputMeshAttr; // Output stablized mesh

    MStatus returnStatus;

    // CREATE ATTRIBUTES
    BobNode::inputMesh = inputMeshAttr.create("inputMesh", "inMesh", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating input mesh attribute!\n");

    MString defaultColorConstraint = "HARD";
    BobNode::colorConstraint = colorContraintAttr.create(
                "colorContraint", "col", MFnData::kString, MFnStringData().create(defaultColorConstraint), &returnStatus);
    McheckErr(returnStatus, "ERROR in creating color contraint attribute!\n");

    BobNode::iteration = iterAttr.create("iterations", "itr", MFnNumericData::kInt, 1, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating iteration attribute!\n");

    BobNode::outputMesh = outputMeshAttr.create("outputMesh", "outMesh", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR in creating output mesh attribute!\n");

    MString defaultStatus = "Unstable";
    BobNode::stabilityStatus = statusAttr.create(
                "stabilityStatus", "stableStat", MFnData::kString, MFnStringData().create(defaultStatus), &returnStatus);
    McheckErr(returnStatus, "ERROR in creating stability status attribute!\n");

    // ADD ATTRIBUTES
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

    // ADD ATTRIBUTE AFFECTS
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
    return MS::kSuccess;
}

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
//	MString guiPath = plugin.loadPath() + MString("/LSystemGUI.mel");
//	MString quoteInStr = "\\\"";
//	MString eval = MString("eval(\"source " + quoteInStr + guiPath + quoteInStr + "\");");
//	MString menu = MString("menu - parent MayaWindow - l \"LSystems\" LSystems;");
//	MString addLSystemCmd = MString("menuItem - label \"LSystem Command\" - parent MayaWindow|LSystems - command \"createGUI()\" LSystemCommand;");
//	MString addNodeCmd = MString("menuItem - label \"LSystem Node\" - parent MayaWindow|LSystems - command \"setupNode()\" LSystemNodeItem;");

//	MString createMenu = eval + "\n" + menu + "\n" + addLSystemCmd + "\n" + addNodeCmd;

//	char buffer[2048];
//	sprintf_s(buffer, 2048, createMenu.asChar());
//	MGlobal::executeCommand(buffer, true);

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
