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

#ifndef SPI_PARALLEL_ACCESS_TEST_GPIOREADER_H
#define SPI_PARALLEL_ACCESS_TEST_GPIOREADER_H

#include <string>

using namespace std;

class GpioReader {
private:
    GpioReader();
    void initializeGpioPin();
    void throwStringException(string error);
    string pinId;
    int fileDescriptor;
public:
    GpioReader(string pinId);
    int getPinStatus();
    ~GpioReader();
};


#endif //SPI_PARALLEL_ACCESS_TEST_GPIOREADER_H
