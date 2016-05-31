/**
 * Copyright (C) 2016  Jonas Sticha (jonas.sticha@tum.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "DataSender.h"

DataSender::DataSender(string spiDevName) {
    spiDeviceName = spiDevName;
}

/**
 * Send specified amount of bytes to the SPI device
 * In case something bad happens, an exception with the error message as string is thrown
 *
 * numBytes: Number of bytes to send
 */
void DataSender::sendData(uint32_t numBytes) {
    int fileDescriptor = open(spiDeviceName.c_str(), O_RDWR);
    if(fileDescriptor<0) {
        string error = "Device " + spiDeviceName + " could not be accessed!";
        throw error;
    }

    struct spi_ioc_transfer xfer;
    memset(&xfer, 0, sizeof(xfer));
    uint8_t* buf;
    buf = (uint8_t*) malloc(sizeof(uint8_t)*numBytes);
    memset(buf, 0xDE, numBytes);
    xfer.tx_buf = (unsigned long) buf;
    xfer.bits_per_word = 8;
    xfer.len = numBytes;

    int success = ioctl(fileDescriptor, SPI_IOC_MESSAGE(1), &xfer);
    delete(buf);
    if(success < 0) {
        string error = "SPI data transfer could not be completed!";
        throw error;
    }
}

DataSender::~DataSender() {
}
