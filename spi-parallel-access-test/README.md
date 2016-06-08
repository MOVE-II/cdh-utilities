# SPI Parallel Access Test

This directory contains tests for the SPI interface.
The main goal is to test whether the SPI driver correctly handles concurrently sent messages to different SPI devices.
To be more specific, we want to validate whether multiple slave selects can become active at the same time and whether (long) messages are cut in parts, in order to send a message to a different slave.
The test suite consists of two test applications:

* SPI\_pat\_master:

   This test binary acts as an SPI master and sends multiple messages of varying length to multiple SPI devices concurrently.

* SPI\_pat\_slave-mock:

   This test binary has to be run on an additional platform with the slave selects of the SPI master attached to GPIO pins.
   It will validate whether the slave selects are activated as expected.

## SPI\_pat\_master

### Usage
```bash
SPI_pat_master num_bytes repetitions num_threads spi_deviceX..
```

* *num_bytes*

    Amount of bytes to send to the spi device.

* *repetitions*

    Repeatedly send the data for the specified amount of times.

* *num_threads*

    Amount of threads that shall send data to spi devices. All threads send the same amount of data (i.e. same number of bytes per packet and same amount of repetitions).

* *spi_deviceX..*

    Path to the spi device files to send the data to. The paths to *num_threads* spi devices have to be specified.

## SPI\_pat\_slave-mock

### Usage
```bash
SPI_pat_slave-mock gpio_pin_id
```

Sending signal 1 (SIGHUP) to the process, causes it to printing the current state to stdout. Can be done with
```bash
kill -1 [SPI_pat_slave-mock pid]
```

* *gpio_pin_id*

    ID of the GPIO pin, under which it can be found under /sys/class/gpio/[gpio_pin_id].
