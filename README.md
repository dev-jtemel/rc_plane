# RC Plane
DIY RC Plane built and designed from scratch.

 - [Hardware](#hardware)
   - [Controller](#controller)
   - [Schematic](#schematic)
 - [Microcontroller](#microcontroller)
   - [Components](#components)
   - [Build and Flash the microcontroller](#build-and-flash-the-microcontroller)

## Hardware

This sections lays out the schematic of the hardware and the relation of the controls
of the flight controller to the plane.

### Controller
The following schematic lays out the control mapping of transmitter.

![controller](resources/controller.png)

### Schematic
The following schematic lays out the hardware design of the plane.

![schematic](resources/schematic.png)

For direct access to pin mapping, see the `pins.hpp` file in the `mcu` library.

##  Microcontroller
The microcontroller used for this build is the Arduino Mega 2560 Rev3, although most Arduino (or similar) microcontrollers would suffice for this project.

### Components
The following components are used directly by the microcontroller:
- (1x) 9V DC breadboard power supply (+9V battery)
- (1x) FS-IA10B 2.4GHz 10 Channel DC receiver
- (5x) Tower Pro 9G micro servos
- (1x) L923D motor controller
- (2x) Brushed DC motor
- (2x) 220 Ohm resistors
- (1) Green LED
- (1) Red LED

### Build and Flash the microcontroller
Using the `builder.sh` script, deploy the following commands *after* the microcontroller's USB port is connected to your computer **and** the 9V power supply is turned on.

```shell
./builder.sh
4 # Install dependencies
1 # Compile the MCU library
2 # Flash the microcontroller
3 # Optional to view serial data output (CTRL+A CTRL+X to exit).
```
