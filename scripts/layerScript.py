from maya.app import renderSetup
import maya.cmds as cmds
import maya.mel as mel
import maya.app.general.createImageFormats as createImageFormats


class Render:
    def __init__(self):
        self.rs = renderSetup.model.renderSetup.instance()
       
    
    def setUpRenderer(self):
        cmds.setAttr("defaultRenderGlobals.currentRenderer", "mayaHardware2", type="string")
        cmds.setAttr("hardwareRenderingGlobals.renderMode", 2)
        
        cmds.setAttr('defaultRenderGlobals.imageFormat', 8)
        cmds.setAttr('defaultRenderGlobals.outFormatControl', 0)
        
        
    def renderAndSave(self, image_save_path):    
        #cmds.ogsRender(camera='top') #noRenderView=False, currentFrame=True)
        #cmds.render('top', x=768, y=576 )
        mel.eval('renderWindowRender redoPreviousRender renderView')
        mel.eval('updateRenderOverride')
        editor = 'renderView'
        formatManager = createImageFormats.ImageFormats()
        formatManager.pushRenderGlobalsForDesc("JPEG")
        # cmds.renderWindowEditor(editor, currentCamera='top', e=True, writeImage='/Volumes/Seagate/bob_test/testImage.jpg')
        cmds.renderWindowEditor(editor, currentCamera='top', e=True, writeImage=image_save_path)
        formatManager.popRenderGlobals()
        
    
    def renderLayers(self, num_max_layers):
        renderSetup.model.renderSetup.initialize()
        ov = renderSetup.model.override
        
        for i in range(num_max_layers):
            layer_name = 'layer' + str(i).zfill(2)
            collection_name = 'collection' + str(i)
            layer_obj = self.rs.createRenderLayer(layer_name)
            collec_obj = layer_obj.createCollection(collection_name)
        
            collec_obj.getSelector().staticSelection.set(['grid', layer_name])
        
            over_obj = collec_obj.createOverride('MyFirstOverride', ov.AbsOverride.kTypeId) #absolute   
            over_obj.finalize('defaultRenderQuality.shadingSamples')
        
            self.rs.switchToLayer(layer_obj)
            self.renderAndSave('/Volumes/Seagate/bob_test/'+ layer_name + '.jpg')


if __name__ == '__main__':
    r = Render()
    cmds.polyPlane(subdivisionsX=100, subdivisionsY=100, width=100, height=100, name='grid')
    r.setUpRenderer()
    r.renderLayers(28)
    


