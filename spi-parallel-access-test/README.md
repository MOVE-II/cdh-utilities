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
SPI_pat_master spi_device num_bytes
```

* *spi_device*

    Path to the spi device file to send the data to.

* *num_bytes*

    Amount of bytes to send to the spi device.
