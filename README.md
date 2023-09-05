# RC Plane
DIY RC Plane built and designed from scratch.
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [RC Plane](#rc-plane)
  - [Design](#design)
    - [Libraries](#libraries)
      - [Autopilot `rcplane.autopilot`](#autopilot-rcplaneautopilot)
      - [Common `rcplane.common`](#common-rcplanecommon)
      - [IO `rcplane.io`](#io-rcplaneio)
      - [SoM `rcplane.som`](#som-rcplanesom)
      - [MCU `rcplane.mcu`](#mcu-rcplanemcu)
  - [Configuration](#configuration)
  - [Build](#build)
    - [Microcontroller](#microcontroller)
    - [SoM](#som)
    - [PC](#pc)
  - [Running the Software](#running-the-software)
  - [Testing](#testing)
  - [Debugging](#debugging)
  - [Requirements](#requirements)
    - [Physical](#physical)

<!-- /code_chunk_output -->


[![CMake](https://github.com/dev-jtemel/rc_plane/actions/workflows/cmake.yml/badge.svg)](https://github.com/dev-jtemel/rc_plane/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/dev-jtemel/rc_plane/actions/workflows/codeql.yml/badge.svg)](https://github.com/dev-jtemel/rc_plane/actions/workflows/codeql.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/dev-jtemel/rc_plane/badge/main)](https://www.codefactor.io/repository/github/dev-jtemel/rc_plane/overview/main)

![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)


## Design
The design of the system relies on two main hardware components: the MCU (for hardware interfacing, speed and RC communication) and the SoM (for autopilot systems and logging).

### Libraries
Libraries are broken down by subdirectories in the `include/` dir and by namespaces. The current libraries are:

#### Autopilot `rcplane.autopilot`
This library handles all manipulation of autopilot systems and defines the systems supported. Currently there are two systems: ManualAutopilot (which is a dummy implementation to give user full control over the aircraft) and StabilizedAutopilot (which stabilizes the pitch and roll of the aircraft when the user is **not** manipulating the controls). This library handles applying the applicable autopilot system based on the state of the user's RC controller.

#### Common `rcplane.common`
Contains all relevant information for both the MCU and the SoM, synchronizing and ensuring that shared data structures and flags are consistent across devices. The
serial communication (i.e. Packets) are defined here.

Note: Timestamp buffer overflows at 4294967295 milliseconds (4294967 seconds; 71583minutes; 1193 hours), well exceeding any possible flight times. Also note that the timestamp resets upon an initial read of the serial port from the MCU since the microcontroller resets on an initial read.

#### IO `rcplane.io`
Handles all io communication, such as over serial or logging. This library also maintains managing and distributing the configuration parameters of the suite.

#### SoM `rcplane.som`
Not a library but the main entry point for the SoM suite.

#### MCU `rcplane.mcu`
Not a library but the main entry point for the MCU suite.

The architecture of the SoM and library to library linkage is as follows:
![som_class_diagram](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/dev-jtemel/rc_plane/main/puml/som_class_diagram.puml)

The interaction between the MCU and SoM can be seen here:
![mcu_som_communication](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/dev-jtemel/rc_plane/main/puml/mcu_som_communication.puml)


## Configuration
To combat changing requirements of the software, major parameters are configured in a JSON configuration file (primary file is located at `configs/config.json`).
You can trace where the configuration parameter is used in the source code by following the nesting as namespaces and/or class names.

Available parameters are defined below. Format: `"parameter name": Description of the parameter. <c++ equivalent type> : default_value : [available_options]`.
```json
{
  "rcplane": {
    "common": {
      "handshake_termination_string": Message to receive from the MCU to flush the serial buffer. <string> : rcplane\r\n
    },
    "io": {
      "journal": {
        "severity": Severity to output console logs. <string> : trace : [trace, debug, info, warning, error, fatal]
      },
      "serial_controller": {
        "tty_dev": TTY Device to communicate to the MCU with. <string> : /dev/ttyACM0,
        "baud_rate": Baudrate of the tty device: <uint32_t> : 115200,
        "read_timeout_ms": Timeout in milliseconds for a read operation. <uint32_t> : 500,
        "write_timeout_ms": Timeout in milliseconds for a write operation. <uint32_t> : 500
      },
      "telemetry": {
        "debug_mq_name": Name of the POSIX MQ to send DebugMessages. <string> : rcplane_mq_debug,
        "attitude_mq_name": Name of the POSIX MQ to send AttitudeMessages. <string> : rcplane_mq_attitude,
        "debug_mq_size": Size of the debug MQ. <uint32_t> : 10
      }
    },
    "autopilot": {
      "max_throttle": Upper bound of throttle output. <uint8_t> : 255,
      "max_aileron_deflection": Maximum degrees the aileron can deflect. <int8_t> : 35,
      "max_elevator_deflection": Maximum degrees the elevator can deflect. <int8_t> : 40,
      "max_rudder_deflection": Maximum degrees the rudder can deflect. <int8_t> : 20,
      "stabilize": {
        "kp": Proportional gain for the PID controller. <double> : 0.8,
        "ki": Integral gain for the PID controller. <double> : 0.05,
        "kd": Derivate gain for the PID controller. <double> : 0.02,
        "max_integral_error": The upper/lower bound for the PID integral error. <double> :4
      }
    },
    "som": {
      "som_controller": {
        "handshake_attempts": Number of times to attempt a handshake before aborting. <uint32_t> : 2,
        "main_loop_delay_ms": Milliseconds to wait before starting the main SoM loop. <uint32_t> :4000
      }
    }
  }
}
```

## Build
To compile the entire project, use the `full_build.sh` script. The following sections layout how to
compile and flash each section of software.

### Microcontroller
Using the `builder.sh` script, deploy the following commands *after* the microcontroller's USB port is connected to your computer **and** the 9V power supply is turned on:

```shell
./builder.sh
9 # Install dependencies (only needed on first clone)
1 # Cycle to MCU
2 # Compile the MCU library
3 # Flash the microcontroller
8 # Optional to view serial data output (CTRL+A CTRL+X to exit).
```

### SoM
Provide the IP of the SOM to the builder and execute the following commands:

```shell
./builder.sh -i <IP>
9 # Install dependencies
1 # Cycle to SOM
2 # Compile the SOM library
3 # Flash SOM
```

### PC
Using the `builder.sh` script, deploy the following commands:

```shell
./builder.sh
1 # Cycle to PC
2 # Compile the PC library
```

## Running the Software
Turn on all power supplies and connect the MCU to the SoM via a serial connection. Then start the SoM controller followed by the PC controller:

```shell
./builder.sh
1 # Cycle to SOM or PC
4 # Run the controller (mcu must be connected to respective device)
```

## Testing
To run the test suite use the following commands:
```shell
./builder.sh
1 # Cycle to PC
6 # Compile Test Suite
7 # Run Test Suite
```

Coverage reports are generated after a test suite run. You can view them with the following command:
```shell
firefox coverage/coverage.html
```

Alternatively, you can run the top level test script instead. This does not build coverage reports.
```shell
./run_tests.sh
```

## Debugging
To run the debugger, connect the MCU and use the following command:
```shell
./builder.sh
1 # Cycle to PC
2 # Compile PC
5 # Run gdb debugger
```

## Requirements
This section lays out the requirements, both virtual and physical, needed for the rc plane.

### Physical
The following components are used in the build:
- (1x) Raspberry PI 4B+ (or similar)
- (1x) Arduino Mega 2560 Rev3 (or similar)
- (1x) FS-IA10B 2.4GHz 10 Channel DC Receiver
- (1x) Skynet FS 2.4GHz Transmitter
- (5x) Tower Pro 9G micro servos
- (1x) 40A ESC
- (1x) 3S 6000mAh LiPo (or similar)
- (1x) Brushless DC Motor
- (1x) USB-A GNSS Receiver
- (3x) 220 Ohm resistors
- (1) Green LED
- (1) Red LED
