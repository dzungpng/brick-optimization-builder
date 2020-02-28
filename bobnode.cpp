#include "bobnode.h"

#define McheckErr(stat,msg)			\
    if ( MS::kSuccess != stat ) {	\
        cerr << msg;				\
        return MS::kFailure;		\
}

MTypeId BobNode::id(0x80000);

void* BobNode::creator()
{
    return new BobNode;
}

MStatus BobNode::initialize()
{

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
