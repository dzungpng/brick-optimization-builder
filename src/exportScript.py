import fitz 
import os, os.path
import math

def exportToPDF(exportPath="", imagePath=""):
    print "in export function\n"
    doc = fitz.open()
    buffer = 30
    width = 842
    height = 595
    picWidth = (width - (3 * buffer)) / 2.0
    picHeight = (height - (3 * buffer)) / 2.0

    numLayers = len([name for name in os.listdir(imagePath) if (name.split('.')[-1] in ["png", "jpg", "jpeg"])])
    numPages = math.ceil(numLayers / 4.0)

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
                        
                    pix = fitz.Pixmap(imagePath + "/layer" + str(currImg) + ".png")
                    page.insertImage(rect, pixmap=pix, overlay=False)
                    
                    # insert text
                    text = "Layer " + str(currImg)
                    textPos = fitz.Point(posX + 15, posY - 8)
                    page.insertText(textPos, text, fontsize = 14, overlay=True)
                    
                    currImg += 1
    
    doc.save(exportPath)
