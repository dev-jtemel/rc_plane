#!/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import sys

TIMESTAMPS = []
MOTOR = []
AILERON = []
ELEVATOR = []
RUDDER = []

def twos8(value):
    return -(value & 0x80) | (value & 0x7F)

with open(sys.argv[1]) as file:
    for line in file:
        line = line.rstrip()

        TIMESTAMPS.append(int(line[0:6], 16))
        #STATE.append(int(line[14:16], 16))
        MOTOR.append(int(line[12:14], 16))
        AILERON.append(twos8(int(line[10:12], 16)))
        ELEVATOR.append(twos8(int(line[8:10], 16)))
        RUDDER.append(twos8(int(line[6:8], 16)))



# Parameters
length = 0.3  # Length of the rectangle
width = 0.1  # Width of the rectangle
height = 0.02  # Height of the rectangle

elength = 0.42 # Length of the rectangle
ewidth = 0.1  # Width of the rectangle
eheight = 0.02  # Height of the rectangle

rlength = 0.02  # Length of the rectangle
rwidth = 0.1  # Width of the rectangle
rheight = 0.2  # Height of the rectangle

# Simulation settings
dt = 0.01  # Time step
num_steps = 21  # Number of simulation steps

# Create figure and 3D axes
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_axis_off()

ax.grid(False)

# Define the vertices of the rectangle
vertices = np.array([
    [0, 0, 0], [length, 0, 0], [length, width, 0], [0, width, 0],
    [0, 0, height], [length, 0, height], [length, width, height], [0, width, height]
])

vertices2 = np.array([
    [0.5, 0, 0], [length + 0.5, 0, 0], [length + 0.5, width, 0], [0.5, width, 0],
    [0.5, 0, height], [length + 0.5, 0, height], [length + 0.5, width, height], [0.5, width, height]
])

vertices3 = np.array([
    [0, 0 , 0], [elength, 0 , 0], [elength, ewidth , 0], [0, ewidth  , 0],
    [0, 0 , eheight], [elength, 0 , eheight], [elength, ewidth , eheight], [0, ewidth , eheight]
])

vertices4 = np.array([
    [0+ 0.5, 0, 0], [elength+ 0.5, 0 , 0], [elength+ 0.5, ewidth , 0], [0+ 0.5, ewidth  , 0],
    [0+ 0.5, 0, eheight], [elength+ 0.5, 0 , eheight], [elength+ 0.5, ewidth , eheight], [0+ 0.5, ewidth , eheight]
])

vertices5 = np.array([
    [0, 0, 0], [rlength, 0 , 0], [rlength, rwidth , 0], [0, rwidth  , 0],
    [0, 0, rheight], [rlength, 0 , rheight], [rlength, rwidth , rheight], [0, rwidth , rheight]
])

# Define the faces of the rectangle
faces = np.array([
    [0, 1, 2, 3], [4, 5, 6, 7], [0, 1, 5, 4], [1, 2, 6, 5],
    [2, 3, 7, 6], [3, 0, 4, 7], [0, 3, 2, 5], [1, 4, 7, 6]
])

angles = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ]
angles2 = [ 0, 2, 8, 10, 20, 20, 8, 7, 8, 9, 10, -11, -12, -13, -14, -15, -16, -17, -18, -19, -20 ]
offset2 = np.array([
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        [0.20, 0.5, 0],
                        ])
offset3 = np.array([
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        [0.40, 0.5, 0],
                        ])

# Simulation loop
for step in range(len(TIMESTAMPS)):
    # Clear the previous frame
    ax.cla()

    # Calculate rotation matrix
    rotation_matrix = np.array([[1, 0, 0],
                                [0, np.cos(np.radians(AILERON[step])), -np.sin(np.radians(AILERON[step]))],
                                [0, np.sin(np.radians(AILERON[step])), np.cos(np.radians(AILERON[step]))]])

    rotation_matrix1 = np.array([[1, 0, 0],
                                [0, np.cos(np.radians(-AILERON[step])), -np.sin(np.radians(-AILERON[step]))],
                                [0, np.sin(np.radians(-AILERON[step])), np.cos(np.radians(-AILERON[step]))]])

    rotation_matrix2 = np.array([
                                [1, 0, 0],
                                [0, np.cos(np.radians(ELEVATOR[step])), -np.sin(np.radians(ELEVATOR[step]))],
                                [0, np.sin(np.radians(ELEVATOR[step])), np.cos(np.radians(ELEVATOR[step]))],
                                ])
    rotation_matrix3 = np.array([
                                [np.cos(np.radians(RUDDER[step])), -np.sin(np.radians(RUDDER[step])), 0],
                                [np.sin(np.radians(RUDDER[step])), np.cos(np.radians(RUDDER[step])), 0],
                                [0, 0, 1],
                                ])

    # Rotate the vertices
    rotated_vertices = np.dot(vertices, rotation_matrix)
    rotated_vertices1 = np.dot(vertices2, rotation_matrix1)
    rotated_vertices2 = np.dot(vertices3, rotation_matrix2)
    rotated_vertices4 = np.dot(vertices5, rotation_matrix3)

    rotated_vertices2 += offset2
    rotated_vertices4 += offset3

    # Plot the rectangle
    ax.add_collection3d(Poly3DCollection([rotated_vertices[face] for face in faces], alpha=1))
    ax.add_collection3d(Poly3DCollection([rotated_vertices1[face] for face in faces], alpha=1))
    ax.add_collection3d(Poly3DCollection([rotated_vertices2[face] for face in faces], alpha=1, color='red'))
    ax.add_collection3d(Poly3DCollection([rotated_vertices4[face] for face in faces], alpha=1, color='green'))

    for axis in [ax.xaxis, ax.yaxis, ax.zaxis]:
        axis.set_ticklabels([])
        axis._axinfo['axisline']['linewidth'] = 0
        axis._axinfo['axisline']['color'] = "0"
        axis._axinfo['grid']['linewidth'] = 0
        axis._axinfo['grid']['linestyle'] = "--"
        axis._axinfo['grid']['color'] = "#FFFFFF"
        axis._axinfo['tick']['inward_factor'] = 0.0
        axis._axinfo['tick']['outward_factor'] = 0.0
        axis.set_pane_color((1, 1, 1))

    # Pause to control the animation speed
    plt.pause(0.01)

ax.xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
ax.yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
ax.zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
ax.xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
ax.yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
ax.zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
# make the grid lines transparent
ax.xaxis._axinfo["grid"]['color'] =  (1,1,1,0)
ax.yaxis._axinfo["grid"]['color'] =  (1,1,1,0)
ax.zaxis._axinfo["grid"]['color'] =  (1,1,1,0)
plt.show()
