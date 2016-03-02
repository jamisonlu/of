#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Optical Flow Visualizer
__author__ = "Douglas Uba"

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

# Read vectors file
w,h,u,v = readFLO('../../data/lkc2f/uv.flo')

# Create grid
x, y = np.meshgrid(np.arange(0, w, 1), np.arange(0, h, 1))

# Create figure
fig = plt.figure()

# Read images
imgs=[]
imgs.append(mpimg.imread('../../data/input/satellitea.jpg'))
imgs.append(mpimg.imread('../../data/input/satelliteb.jpg'))

# Plot first image
im = plt.imshow(imgs[0], cmap='Greys')

# Reduce factor for vectors
n=9

x = x[::n,::n]
y = y[::n,::n]
u = u[::n,::n]
v = v[::n,::n]

# Plot vectors
plt.quiver(x, y, u, v, pivot='mid', color='dodgerblue', \
           angles='xy', headlength=4, antialiased=True)
           
# Animation loop
i = 0
def updatefig(*args):
    global i
    im.set_array(imgs[i])
    i += 1
    if i == len(imgs):
        i = 0
    return im
    
anim = animation.FuncAnimation(fig, updatefig, interval=800, blit=False)
       
plt.show()
