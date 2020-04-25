from maya.app import renderSetup
import maya.cmds as cmds
import maya.mel as mel
import maya.app.general.createImageFormats as createImageFormats


cmds.polyPlane(subdivisionsX=100, subdivisionsY=100, width=100, height=100, name='grid')

renderSetup.model.renderSetup.initialize()
rs = renderSetup.model.renderSetup.instance()
ov = renderSetup.model.override

layer_obj = rs.createRenderLayer('MyFirstLayer')
collec_obj = layer_obj.createCollection('MyFirstCollection')
over_obj = collec_obj.createOverride('MyFirstOverride', ov.AbsOverride.kTypeId) #absolute      

collec_obj.getSelector().staticSelection.set(['grid'])
over_obj.finalize('defaultRenderQuality.shadingSamples')

cmds.setAttr("defaultRenderGlobals.currentRenderer", "mayaHardware2", type="string")
cmds.setAttr("hardwareRenderingGlobals.renderMode", 2)

cmds.setAttr('defaultRenderGlobals.imageFormat', 8)
cmds.setAttr('defaultRenderGlobals.outFormatControl', 0)


rs.switchToLayer(layer_obj)

cmds.ogsRender(camera='top', noRenderView=True)

# mel.eval('renderWindowRender redoPreviousRender renderView')
editor = 'renderView'
formatManager = createImageFormats.ImageFormats()
formatManager.pushRenderGlobalsForDesc("JPEG")
cmds.renderWindowEditor(editor, e=True, writeImage='/Volumes/Seagate/bob_test/testImage.jpg')
formatManager.popRenderGlobals()



