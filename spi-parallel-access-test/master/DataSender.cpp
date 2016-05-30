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
#include <iostream>
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
void DataSender::sendData(int numBytes) {
    int fileDescriptor = open(spiDeviceName.c_str(), O_RDWR);
    if(fileDescriptor<0) {
        string error = "Device " + spiDeviceName + " could not be accessed!";
        throw error;
    }
    //TODO: Continue with setting spi mode with ioctl
}

DataSender::~DataSender() {
}
