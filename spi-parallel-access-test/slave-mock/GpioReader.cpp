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
#include <unistd.h>
#include "GpioReader.h"


GpioReader::GpioReader(string pinId) {
    this->pinId = pinId;
    fileDescriptor = -1;
    initializeGpioPin();
}

void GpioReader::throwStringException(string error) {
    throw error;
}

void GpioReader::initializeGpioPin() {
    string gpioPath = "/sys/class/gpio/" + pinId + "/";
    int fd = open((gpioPath + "direction").c_str(), O_WRONLY);
    if(fd < 0) {
        throwStringException("Unable to open the direction file in " + gpioPath);
    }
    ssize_t ret = write(fd, "in", 2);
    if(ret < 1) {
        throwStringException("Unable to write to direction file in " + gpioPath);
    }
    close(fd);
    fileDescriptor = open((gpioPath + "value").c_str(), O_RDONLY);
    if(fileDescriptor < 0) {
        throwStringException("Unable to open the value file in " + gpioPath);
    }
}

GpioReader::~GpioReader() {
    if(fileDescriptor >= 0)
        close(fileDescriptor);
}

int GpioReader::getPinStatus() {
    char value = ' ';
    lseek(fileDescriptor, 0, SEEK_SET);
    ssize_t ret = read(fileDescriptor, &value, 1);
    if(ret <= 0) {
        throwStringException("Unable to read value of pin " + pinId);
    }
    return value - '0';
}

string GpioReader::getPinId() {
    return pinId;
}
