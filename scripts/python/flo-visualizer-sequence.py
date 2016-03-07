#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Optical Flow Visualizer
__author__ = "Douglas Uba"

import glob
import matplotlib.image as mpimg
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np


# This function reads Optical Flow Middlebury files (.flo)
def readFLO(path):
    f = open(path, 'rb')
    
    # Read magic number ("PIEH" in ASCII = float 202021.25)
    magic = np.fromfile(f, np.float32, count=1)

    if magic != 202021.25:
        raise Exception('Invalid .flo file') 

    # Read width
    f.seek(4)
    w = np.fromfile(f, np.int32, count=1)
    
    # Read height
    f.seek(8)
    h = np.fromfile(f, np.int32, count=1)
    
    # Read (u,v) coordinates
    f.seek(12)
    data = np.fromfile(f, np.float32, count=w*h*2)

    # Close file (.flo)
    f.close()

    # Reshape data into 3D array (columns, rows, bands)
    dataM = np.resize(data, (h, w, 2))

    # Extract u and v coordinates
    u = dataM[:,:,0]
    v = dataM[:,:,1]
    
    return w,h,u,v

# Dimensions
w=0
h=0

# Input images
imgDir = '../../data/eval-data-gray/Backyard/'
imgFormat = 'png'
imgPaths = glob.glob(imgDir + "*." + imgFormat)

# Read images
imgs=[]
for i in xrange(0, len(imgPaths)):
    imgs.append(mpimg.imread(imgPaths[i]))

# Flow vector files
uvDir='../../data/eval-data-gray/Backyard/results/'
uvPaths= glob.glob(uvDir + "*.flo")

# Read flow vectors 
us=[]
vs=[]
for i in xrange(0, len(uvPaths)):
    w,h,u,v = readFLO(uvPaths[i])
    us.append(u)
    vs.append(v)
    
# Create grid
x, y = np.meshgrid(np.arange(0, w, 1), np.arange(0, h, 1))

# Create figure
fig = plt.figure()

# Plot first image
im = plt.imshow(imgs[0], cmap='gray')

# Reduce factor for vectors
n=9

# Reduce grid
x = x[::n,::n]
y = y[::n,::n]

# Reduce flow vectors 
for i in xrange(0, len(uvPaths)):
    us[i] = us[i][::n,::n]
    vs[i] = vs[i][::n,::n]
    
# Plot first vectors
Q = plt.quiver(x, y, us[0], vs[0], pivot='mid', color='red', \
               angles='xy', headlength=4, antialiased=True, alpha=0.8)
           
# Animation loop
i = 1
def updatefig(*args):
    global i, j
    im.set_array(imgs[i])
    Q.set_UVC(us[i-1], vs[i-1])
    i += 1
    if i == len(imgs):
        i = 1
    return im
    
anim = animation.FuncAnimation(fig, updatefig, interval=200, blit=False)
anim.save('animation.mp4')

plt.show()
