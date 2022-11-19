#!/usr/bin/env python3
import sys
import os
import datetime
import glob
from collections import namedtuple
from struct import unpack

import numpy as np
from PIL import Image

# EDITABLE START

# Change folder to be a foler with correct permissions

mainDir = "/home/matthew/Desktop/a/"

# EDITABLE END

# Sets up directorys

curTime = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

workingDir = os.path.join(mainDir, curTime)

os.mkdir(workingDir)
os.mkdir(os.path.join(workingDir,"Pages"))
os.mkdir(os.path.join(workingDir,"Images"))

CupsRas3 = namedtuple(
    # Documentation at https://www.cups.org/doc/spec-raster.html
    'CupsRas3',
    'MediaClass MediaColor MediaType OutputType AdvanceDistance AdvanceMedia Collate CutMedia Duplex HWResolutionH '
    'HWResolutionV ImagingBoundingBoxL ImagingBoundingBoxB ImagingBoundingBoxR ImagingBoundingBoxT '
    'InsertSheet Jog LeadingEdge MarginsL MarginsB ManualFeed MediaPosition MediaWeight MirrorPrint '
    'NegativePrint NumCopies Orientation OutputFaceUp PageSizeW PageSizeH Separations TraySwitch Tumble cupsWidth '
    'cupsHeight cupsMediaType cupsBitsPerColor cupsBitsPerPixel cupsBitsPerLine cupsColorOrder cupsColorSpace '
    'cupsCompression cupsRowCount cupsRowFeed cupsRowStep cupsNumColors cupsBorderlessScalingFactor cupsPageSizeW '
    'cupsPageSizeH cupsImagingBBoxL cupsImagingBBoxB cupsImagingBBoxR cupsImagingBBoxT cupsInteger1 cupsInteger2 '
    'cupsInteger3 cupsInteger4 cupsInteger5 cupsInteger6 cupsInteger7 cupsInteger8 cupsInteger9 cupsInteger10 '
    'cupsInteger11 cupsInteger12 cupsInteger13 cupsInteger14 cupsInteger15 cupsInteger16 cupsReal1 cupsReal2 '
    'cupsReal3 cupsReal4 cupsReal5 cupsReal6 cupsReal7 cupsReal8 cupsReal9 cupsReal10 cupsReal11 cupsReal12 '
    'cupsReal13 cupsReal14 cupsReal15 cupsReal16 cupsString1 cupsString2 cupsString3 cupsString4 cupsString5 '
    'cupsString6 cupsString7 cupsString8 cupsString9 cupsString10 cupsString11 cupsString12 cupsString13 cupsString14 '
    'cupsString15 cupsString16 cupsMarkerType cupsRenderingIntent cupsPageSizeName'
)

def readRas3(rdata):
    if not rdata:
        raise ValueError('No data received')

    # Check for magic word (either big-endian or little-endian)
    magic = unpack('@4s', rdata[0:4])[0]
    if magic != b'RaS3' and magic != b'3SaR':
        raise ValueError("This is not in RaS3 format")
    rdata = rdata[4:]  # Strip magic word
    pages = []

    while rdata:  # Loop over all pages
        struct_data = unpack(
            '@64s 64s 64s 64s I I I I I II IIII I I I II I I I I I I I I II I I I I I I I I I I I I I '
            'I I I f ff ffff IIIIIIIIIIIIIIII ffffffffffffffff 64s 64s 64s 64s 64s 64s 64s 64s 64s 64s '
            '64s 64s 64s 64s 64s 64s 64s 64s 64s',
            rdata[0:1796]
        )
        data = [
            # Strip trailing null-bytes of strings
            b.decode().rstrip('\x00') if isinstance(b, bytes) else b
            for b in struct_data
        ]
        header = CupsRas3._make(data)

        # Read image data of this page into a bytearray
        imgdata = rdata[1796:1796 + (header.cupsWidth * header.cupsHeight * header.cupsBitsPerPixel // 8)]
        pages.append((header, imgdata))

        # Remove this page from the data stream, continue with the next page
        rdata = rdata[1796 + (header.cupsWidth * header.cupsHeight * header.cupsBitsPerPixel // 8):]

    return pages


pages = readRas3(sys.stdin.buffer.read())

for i, datatuple in enumerate(pages):
    (header, imgdata) = datatuple

    if header.cupsColorSpace != 0 or header.cupsNumColors != 1:
        raise ValueError('Invalid color space, only monocolor supported')
    
    npdata = np.frombuffer(imgdata, dtype=np.uint8)
    npixels = npdata.reshape((header.cupsHeight, header.cupsWidth))
    
    im = Image.fromarray(npixels, 'L')
    im = im.convert('1', dither=1)
    im.save(os.path.join(workingDir,"Images","%d.png" % (i+1)))
    npixels = np.array(im.getdata()).reshape((header.cupsWidth, header.cupsHeight))
    
    
    
    with open(os.path.join(workingDir,"Pages","%d.dm" % (i+1)), "w") as file:
        pix = im.load()
        imWidth, imHeight = im.size
        for row in range(imHeight):
            for col in range(imWidth):
                file.write("%d " % (1 if pix[col, row] == 0 else 0))
            file.write("\n")

with open(os.path.join(workingDir,curTime+".EV3P"), "w") as fileFinal:
    files2Merge = glob.glob(os.path.join(workingDir,"Pages","*.dm"))
    fileFinal.write(curTime+"\n")
    fileFinal.write(str(len(files2Merge))+"\n")
    for file in files2Merge:
        with open(file, "r") as f2m:
            dotMatrix = f2m.read()
            fileFinal.write(dotMatrix+"\n")