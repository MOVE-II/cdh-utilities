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
    xfer = nullptr;
    txBuffer = nullptr;
    spiDeviceName = spiDevName;
}

/**
 * Allocate memory necessary for the call to the sendData method
 *
 * returns number of elements in the xfer array
 */
int DataSender::allocateSendDataMemory(uint32_t numBytes) {
    int numTransfers = (numBytes + maxBytesPerTransfer - 1) / maxBytesPerTransfer;
    xfer = (struct spi_ioc_transfer*) malloc(numTransfers * sizeof(struct spi_ioc_transfer));
    size_t bufferSize = 0;
    if(numTransfers == 1) {
        bufferSize = numBytes * sizeof(uint8_t);
        txBuffer = (uint8_t*) malloc(bufferSize);
    } else {
        bufferSize = maxBytesPerTransfer * sizeof(uint8_t);
        txBuffer = (uint8_t*) malloc(bufferSize);
    }
    memset(txBuffer, 0xDE, bufferSize);
    memset(xfer, 0, sizeof(*xfer) * numTransfers);
    for(int i = 0; i < numTransfers; i++) {
        xfer[i].len = maxBytesPerTransfer;
        xfer[i].tx_buf = (unsigned long) txBuffer;
        xfer[i].bits_per_word = 8;
        xfer[i].cs_change = 0;
        xfer[i].speed_hz = 325500;
    }
    xfer[numTransfers-1].len = numBytes % maxBytesPerTransfer;
    return numTransfers;
}

/**
 * Free memory allocated in the allocateSendDataMemory method
 */
void DataSender::freeSendDataMemory() {
    delete xfer;
    xfer = nullptr;
    delete txBuffer;
    txBuffer = nullptr;
}

/**
 * Send specified amount of bytes to the SPI device
 * In case something bad happens, an exception with the error message as string is thrown
 *
 * numBytes: Number of bytes to send
 * repetitions: How often to send the data
 */
void DataSender::sendData(int numBytes, int repetitions) {
    int fileDescriptor = open(spiDeviceName.c_str(), O_RDWR);
    if(fileDescriptor<0) {
        string error = "Device " + spiDeviceName + " could not be accessed!";
        throw error;
    }
    int numTransfers = allocateSendDataMemory(numBytes);
    int success = 0;
    for(int i = 0; i < repetitions; i++) {
        success = ioctl(fileDescriptor, SPI_IOC_MESSAGE(numTransfers), xfer);
        if(success < 0)
            break;
    }
    freeSendDataMemory();
    if(success < 0) {
        string error = "SPI data transfer could not be completed!";
        cerr << "IOCTL return code: " << success << endl;
        throw error;
    }
}

DataSender::~DataSender() {
}
