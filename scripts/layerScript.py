from maya.app import renderSetup
import maya.cmds as cmds
import maya.mel as mel
import maya.app.general.createImageFormats as createImageFormats


class Render:
    def __init__(self, layer_folder):
        self.rs = renderSetup.model.renderSetup.instance()
        if layer_folder.strip() == '':
            raise ValueError('layer folder is empty! Please specify a layer folder')
        if layer_folder[-1] == '/':
            self.layer_folder = layer_folder
        else: 
            self.layer_folder = layer_folder + '/'
    
    def setUpRenderer(self):
        cmds.setAttr("defaultRenderGlobals.currentRenderer", "mayaHardware2", type="string")
        cmds.setAttr("hardwareRenderingGlobals.renderMode", 2)
        cmds.setAttr("defaultResolution.width", 800)
        cmds.setAttr("defaultResolution.height", 800)
        
        cmds.setAttr('defaultRenderGlobals.imageFormat', 8)
        cmds.setAttr('defaultRenderGlobals.outFormatControl', 0)
        cmds.setAttr('frontShape.renderable', 0)
        cmds.setAttr('perspShape.renderable', 0)
        cmds.setAttr('sideShape.renderable', 0)
        cmds.setAttr('topShape.renderable', 1)
        
        cmds.setAttr('top.translateX', 5.0)
        cmds.setAttr('top.translateZ', 10.0)
        
        
    def renderAndSave(self, image_save_path):            
        editor = 'renderView'
        cmds.renderWindowEditor(editor, currentCamera='top', e=True, writeImage=image_save_path)
        mel.eval('renderWindowRender redoPreviousRender renderView')
        mel.eval('updateRenderOverride')
        
        formatManager = createImageFormats.ImageFormats()
        formatManager.pushRenderGlobalsForDesc("JPEG")
        formatManager.popRenderGlobals()
        
    
    def renderLayers(self, num_max_layers):
        renderSetup.model.renderSetup.initialize()
        ov = renderSetup.model.override
        
        fill_num_slots = len(str(num_max_layers))
        
        for i in range(num_max_layers):
            layer_name = 'layer' + str(i).zfill(fill_num_slots)
            collection_name = 'collection' + str(i)
            layer_obj  = self.rs.createRenderLayer(layer_name)
            collec_obj = layer_obj.createCollection(collection_name)
        
            collec_obj.getSelector().staticSelection.set(['grid', layer_name])
        
            over_obj = collec_obj.createOverride('MyFirstOverride', ov.AbsOverride.kTypeId)    
            over_obj.finalize('defaultRenderQuality.shadingSamples')
        
            self.rs.switchToLayer(layer_obj)
            self.renderAndSave(self.layer_folder + layer_name + '.jpg')


if __name__ == '__main__':
    save_folder = cmds.getAttr('BOBNode1.jpgPath')
    r = Render(save_folder)
    cmds.polyPlane(subdivisionsX=100, subdivisionsY=100, width=100, height=100, name='grid')
    r.setUpRenderer()
    max_layers = cmds.getAttr('BOBNode1.maxLayer')
    r.renderLayers(max_layers)
    


