# moveio

Contains a C++ Wrapper Library for IO functions (SPI, GPIO, PinMapper).

## Usage

When the library is installed, it can be used during build by specifying "-lmoveio" or by using "target_link_libraries" directive in cmake (see [CMakeLists.txt](moveio/test/CMakeLists.txt) for the test programs).

All path and device names used in the code should be collected in a single header file, so that changing them is easy when switching from BeagleBone to final CDH.

### PinMapper setup file

The PinMapper class needs a file "/etc/moveio/mapping.cfg". It's format should be like:

```
USR0
gpio53 53
USR1
gpio54 54
<mapping name>
<gpio name as in sysfs> <gpio id for export>
```

PinMapper files:
* For [BeagleBone Black](mappings/beaglebone_mapping.cfg)
* For Atmel Devboard: -
* For Hyperion Devboard: -

### moveio/SPIDevice.h

Creating an SPIDevice works by providing the device path, and the used SPI mode.
```C++
SPIDevice device(spi_device, SPIDevice::MODE_2);

// set transfer speed for next transfer (default is maximum speed!)
device.setSpeed(100000);
```

read data:
```C++
char dataIn[16];
device.read(data, 16);
// ... do something with dataIn
```

write data:
```C++
char dataOut[100];
// ... fill dataOut
device.write(data, 100);
```

bidirectional transfer:
```C++
char dataIn[20], dataOut[20];
// ... fill dataOut
device.transfer(dataIn, dataOut, 20);
// ... do something with dataIn
```


## Building

Building the library on the beaglebone is straightforward. After "make install", the include and library files can be automatically found.

```bash
cd moveio/build
cmake ..
make
sudo make install
```
