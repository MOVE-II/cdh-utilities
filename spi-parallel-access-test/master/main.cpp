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

#include <iostream>
#include "DataSender.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 3 && argc != 4) {
        cerr << "Wrong number of arguments!" << endl;
        cout << "Usage: " << argv[0] << " spi_device num_bytes [repetitions]" << endl;
        return 1;
    }
    string spiDevice = argv[1];
    uint32_t numBytes = (uint32_t) atol(argv[2]);
    if(numBytes > 4096) {
        cerr << "Maximum size for an SPI data transfer is 4096 bytes!" << endl;
        return 1;
    }
    int repetitions = 1;
    if(argc == 4) {
        repetitions = atoi(argv[3]);
    }
    DataSender dataSender(spiDevice);
    try {
        dataSender.sendData(numBytes, repetitions);
    } catch (string error) {
        cerr << "ERROR in sendData: " << error << endl;
    }

    return 0;
}