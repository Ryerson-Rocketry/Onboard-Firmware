# Onboard-Firmware

This firmware was written using Platform.io, an extension to
vscode for microcontrollers programming, please use it to be
able to compile and upload the code properly.

This reposotory also uses git submodules to point to the 
require libraries needed, please clone this repo, do not 
download it, then run the following:

```bash
git submodule init
git submodule update
```


## Current status

The firmware at the moment successfuly does the folllwing:

* Initlizie and read sensors:
  *  accelerometer
  * MS5611 barometer
  * Generic NMEA GPS
* Write readings to builtin SD card
* Transmit the collected data after encoding

The following tasks need to be done:

* For the GPS:
  * look into GPS modules with male headers similar to the adafruit ultimate gps
 * also test the adafruit gps itself(as we have a spare)
* Order a PCB and field test the system
* rfd900+ frequency switching (2 stage implementation)
* test mpu6050 imu and integrate 
*Bay thermodynamics(for two stage)

