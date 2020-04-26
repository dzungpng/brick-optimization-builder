import fitz 
import os, os.path
import math  
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


def exportLayers(save_folder=""):
    #save_folder = cmds.getAttr('BOBNode1.jpgPath')
    r = Render(save_folder)
    cmds.polyPlane(subdivisionsX=100, subdivisionsY=100, width=100, height=100, name='grid')
    r.setUpRenderer()
    max_layers = cmds.getAttr('BOBNode1.maxLayer')
    r.renderLayers(max_layers)


def exportToPDF(exportPath="", imagePath=""):
    # open render view
    #cmds.RenderViewWindow()
    # first create the layers
    exportLayers(imagePath)
    print "inside export" 
    doc = fitz.open()
    buffer = 30
    width = 842
    height = 595
    picWidth = (width - (3 * buffer)) / 2.0
    picHeight = (height - (3 * buffer)) / 2.0

    numLayers = len([name for name in os.listdir(imagePath) if (name.split('.')[-1] in ["png", "jpg", "jpeg"])])
    numPages = math.ceil(numLayers / 4.0)
    
    print "img path: %s" % imagePath
    print "num layers: %s" % str(numLayers)
    print "num pages: %s" % str(numPages)

    if(numPages == 0):
        cmds.error("Cannot create pdf with no layers")

    currImg = 1
    for p in range(int(numPages)):
        doc.insertPage(p, width=width, height=height)
        page = doc[p]
        for r in range(2):
            for c in range(2):
                if currImg <= numLayers:
                    
                    posX = (c * picWidth) + ((c + 1) * buffer)
                    posY = (r * picHeight) + ((r + 1) * buffer)

                    rect = fitz.Rect(posX, posY, posX + picWidth, posY + picHeight)
                        
                    pix = fitz.Pixmap(imagePath + "layer" + str(currImg) + ".png")
                    page.insertImage(rect, pixmap=pix, overlay=False)
                    
                    # insert text
                    text = "Layer " + str(currImg)
                    textPos = fitz.Point(posX + 15, posY - 8)
                    page.insertText(textPos, text, fontsize = 14, overlay=True)
                    
                    currImg += 1
    
    doc.save(exportPath)


