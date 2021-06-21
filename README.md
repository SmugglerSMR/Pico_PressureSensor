# Arduino version for pressure sencor instalation
Followed from this link:
https://www.tomshardware.com/how-to/program-raspberry-pi-pico-with-arduino-ide

# Install arduino, preferebly snap.
Arduino preferences
```
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

Go to Tools >>  Board >> Boards Manager. 
Install the Raspberry Pi Pico / RP2040

Go to Tools >> Board >> Raspberry Pi RP2040 Boards and select Raspberry Pi Pico.

# Equations:
```
Output = (Out_max - Out_min) / (P_max - P_min) * (Pressure - P_min) + Output_min
```

# TODO: Implement to csv file flash writing
This is an example for C:
https://github.com/raspberrypi/pico-examples/blob/master/flash/program/flash_program.c
Example for MicroPython:
https://www.raspberrypi.org/forums/viewtopic.php?f=146&t=304757
