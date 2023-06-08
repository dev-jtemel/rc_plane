#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"
SOMIP="192.168.0.13"
SIMULATION="OFF"

OPTIONS="\
  Compile-MCU \
  Compile-SOM \
  Compile-PC \
  Flash-MCU \
  Flash-SOM \
  Run-SOM \
  Run-PC \
  Read-Serial-MCU \
  Install-Dependencies \
  Help \
  Quit\
"
usage() {
  echo "Usage:"
  echo "  ./builder.sh -d [dev] -s"
  echo ""
  echo "  where:"
  echo "    -d [dev] : The tty port the microcontroller is connected to."
  echo "               Default: /dev/ttyACM0"
  echo "    -s       : Use simulated communication."
  echo "    -i       : SoM IP address."
  echo "               Default: 192.168.0.13"
  echo ""
  echo "COMMANDS:"
  echo "  1) Compile-MCU          : Compile the MCU code."
  echo "  2) Compile-SOM          : Compile the SOM code."
  echo "  3) Compile-PC           : Compile the flight deck for the PC."
  echo "  4) Flash-MCU            : Flash the MCU code to the connected microcontroller."
  echo "  5) Flash-SOM            : Flash the SOM code to the connected SoM."
  echo "  6) Run-SOM              : Run the controller on the SOM."
  echo "  7) Run-PC               : Run the flight deck on the PC."
  echo "  8) Read-Serial-MCU      : Read the serial output of the microcontroller."
  echo "  9) Install-Dependencies : Install the required dependencies."
  echo "  10) Help                : Display this message."
  echo "  12) Quit                : Exit the builder script."
}

while getopts "sd:i:h" opt; do
    case "$opt" in
        d) DEV="$OPTARG";
            ;;
        s) SIMULATION="ON";
            ;;
        i) SOMIP="$OPTARG";
            ;;
        h) usage; exit 0
            ;;
        ?) usage; exit 1
            ;;
    esac
done

while true;
do
  echo "DEV = ${DEV} | SIMULATION = ${SIMULATION} | SOMIP = ${SOMIP}"

  select opt in ${OPTIONS};
  do
    if [ "$opt" = "Compile-MCU" ];
    then
      bash scripts/compile-mcu.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Compile-SOM" ];
    then
      ssh pi@${SOMIP} 'cd rc_plane && git pull && ./scripts/compile-som.sh /home/pi/rc_plane'
      break
    elif [ "$opt" = "Compile-PC" ];
    then
      bash scripts/compile-pc.sh "$ROOT_DIR" "$SIMULATION"
      break
    elif [ "$opt" = "Flash-MCU" ];
    then
      bash scripts/flash-mcu.sh "$ROOT_DIR" "$DEV"
      break
    elif [ "$opt" = "Flash-SOM" ];
    then
      ssh pi@${SOMIP} 'cp ~/rc_plane/build_som/som/som-controller ~/bin'
      break
    elif [ "$opt" = "Run-SOM" ];
    then
      ssh -t pi@${SOMIP} '~/bin/som-controller'
      break
    elif [ "$opt" = "Run-PC" ];
    then
      bash scripts/run-pc.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Read-Serial-MCU" ];
    then
      bash scripts/read-serial-mcu.sh "$ROOT_DIR" "$DEV"
      break
    elif [ "$opt" = "Install-Dependencies" ];
    then
      bash scripts/install-dependencies.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Help" ];
    then
      usage
      break
    else
      break 2
    fi
  done
done
