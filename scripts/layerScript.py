from maya.app import renderSetup
import maya.cmds as cmds
import maya.mel as mel
import maya.app.general.createImageFormats as createImageFormats


def setUpScene():
    rs = renderSetup.model.renderSetup.instance()
    renderSetup.model.renderSetup.initialize()
    ov = renderSetup.model.override
    
    layer_obj = rs.createRenderLayer('MyFirstLayer')
    collec_obj = layer_obj.createCollection('MyFirstCollection')
    over_obj = collec_obj.createOverride('MyFirstOverride', ov.AbsOverride.kTypeId) #absolute   
    
    collec_obj.getSelector().staticSelection.set(['grid', 'layer07'])
    over_obj.finalize('defaultRenderQuality.shadingSamples')
    
    rs.switchToLayer(layer_obj)
    

def setUpRenderer():
    cmds.setAttr("defaultRenderGlobals.currentRenderer", "mayaHardware2", type="string")
    cmds.setAttr("hardwareRenderingGlobals.renderMode", 2)
    
    cmds.setAttr('defaultRenderGlobals.imageFormat', 8)
    cmds.setAttr('defaultRenderGlobals.outFormatControl', 0)
    
    
def renderAndSave():    
    #cmds.ogsRender(camera='top') #noRenderView=False, currentFrame=True)
    #cmds.render('top', x=768, y=576 )
    mel.eval('renderWindowRender redoPreviousRender renderView')
    mel.eval('updateRenderOverride')
    editor = 'renderView'
    formatManager = createImageFormats.ImageFormats()
    formatManager.pushRenderGlobalsForDesc("JPEG")
    cmds.renderWindowEditor(editor, currentCamera='top', e=True, writeImage='/Volumes/Seagate/bob_test/testImage.jpg')
    formatManager.popRenderGlobals()

if __name__ == '__main__':
    cmds.polyPlane(subdivisionsX=100, subdivisionsY=100, width=100, height=100, name='grid')
    setUpScene()
    setUpRenderer()
    renderAndSave()
    


