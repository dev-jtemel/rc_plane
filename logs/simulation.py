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

# Create figure and 3D axes
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

class Object:
    def __init__(self, length, width, height, offset = [0, 0, 0], color='red'):
        self._vertices = np.array([
            [0, 0, 0], [length, 0 , 0], [length, width, 0], [0, width, 0],
            [0, 0, height], [length, 0, height], [length, width, height], [0, width, height]
        ])

        self._offset = np.array([offset for _ in range(8)])

        self._color = color 
        self._faces = np.array([
            [0, 1, 2, 3], [4, 5, 6, 7], [0, 1, 5, 4], [1, 2, 6, 5],
            [2, 3, 7, 6], [3, 0, 4, 7], [0, 3, 2, 5], [1, 4, 7, 6]
        ])
        self._rotation_matrix = np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]])

    def rot_x(self, angle):
        self._rotation_matrix = np.array([
                                    [1, 0, 0],
                                    [0, np.cos(np.radians(angle)), -np.sin(np.radians(angle))],
                                    [0, np.sin(np.radians(angle)), np.cos(np.radians(angle))]
                                ])


    def rot_z(self, angle):
        self._rotation_matrix = np.array([
                                    [np.cos(np.radians(angle)), -np.sin(np.radians(angle)), 0],
                                    [np.sin(np.radians(angle)), np.cos(np.radians(angle)), 0],
                                    [0, 0, 1],
                                ])

    def step(self):
        rotated_vertices = np.dot(self._vertices, self._rotation_matrix) + self._offset
        alpha = 0.2 if self._color == 'red' else 1
        return Poly3DCollection([rotated_vertices[face] for face in self._faces], facecolor=self._color, color=self._color, alpha=alpha)


body = Object(0.1, 0.75, 0.1, offset=[0.45, 0.0, 0.0])
wings = Object(1, 0.15, 0.01, offset=[0.0, 0.1, 0.09])
h_stab = Object(0.5, 0.05, 0.01, offset=[0.25, 0.65, 0.09])
v_stab = Object(0.02, 0.03, 0.2, offset=[0.49, 0.65, 0.09])

aileron_left = Object(0.4, 0.05, 0.01, offset=[0.0, 0.25, 0.09], color='blue')
aileron_right = Object(0.4, 0.05, 0.01, offset=[0.6, 0.25, 0.09], color='blue')

elevator_left = Object(0.15, 0.05, 0.01, offset=[0.25, 0.7, 0.09], color='blue')
elevator_right = Object(0.15, 0.05, 0.01, offset=[0.6, 0.7, 0.09], color='blue')
rudder = Object(0.02, 0.03, 0.2, offset=[0.49, 0.68, 0.09], color='blue')

for step in range(len(TIMESTAMPS)):
    ax.cla()

    ax.add_collection3d(body.step())
    ax.add_collection3d(wings.step())
    ax.add_collection3d(h_stab.step())
    ax.add_collection3d(v_stab.step())

    aileron_right.rot_x(AILERON[step])
    aileron_left.rot_x(-AILERON[step])
    elevator_right.rot_x(ELEVATOR[step])
    elevator_left.rot_x(ELEVATOR[step])
    rudder.rot_z(RUDDER[step])

    ax.add_collection3d(aileron_left.step())
    ax.add_collection3d(aileron_right.step())
    ax.add_collection3d(rudder.step())
    ax.add_collection3d(elevator_left.step())
    ax.add_collection3d(elevator_right.step())

    plt.pause(0.1)

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

    ax.view_init(elev=-ELEVATOR[step], azim=90 + RUDDER[step]/2)

plt.show()
