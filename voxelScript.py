
##########################################################
# Sample usage.
##########################################################

# Copy the following lines and run them in Maya's Python Script Editor:

import maya.cmds as cmds

# Create a sphere which will act as our input shape.
cmds.polySphere( r=5.0, sx=20, sy=20, name='sphere1' )
cmds.move( -20, 0, 0, 'sphere1' ) # move it over to the side.

# Create the voxelization node.
cmds.createNode( 'BOBNode', name="voxel")

# Create a target shape.
cmds.createNode( 'transform', name='target1' )
cmds.createNode( 'mesh', name='target1Shape', parent='target1' )
cmds.sets( 'target1Shape', add='initialShadingGroup' )

# Connect the attributes.
cmds.connectAttr( 'sphere1Shape.outMesh', 'voxel.inputMesh' )
cmds.connectAttr( 'voxel.outputMesh', 'target1Shape.inMesh' )

