#!/bin/env python3

import matplotlib.pyplot as plt

TIMESTAMPS = []
STATE = []
MOTOR = []
AILERON = []
ELEVATOR = []
RUDDER = []

def twos8(value):
    return -(value & 0x80) | (value & 0x7F)

with open("./test.log") as file:
    for line in file:
        line = line.rstrip()

        TIMESTAMPS.append(int(line[0:6], 16))
        STATE.append(int(line[14:16], 16))
        MOTOR.append(int(line[12:14], 16))
        AILERON.append(twos8(int(line[10:12], 16)))
        ELEVATOR.append(twos8(int(line[8:10], 16)))
        RUDDER.append(twos8(int(line[6:8], 16)))


fig, axs = plt.subplots(4)
fig.suptitle('RCPLANE Log Viewer')

axs[0].plot(TIMESTAMPS, MOTOR)
axs[1].plot(TIMESTAMPS, AILERON)
axs[2].plot(TIMESTAMPS, ELEVATOR)
axs[3].plot(TIMESTAMPS, RUDDER)

plt.show()
